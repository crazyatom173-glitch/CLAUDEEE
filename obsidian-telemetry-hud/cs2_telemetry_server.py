import os
import sys
import json
import time
import math
import socket
from flask import Flask, jsonify, request
from flask_cors import CORS

# Try importing pymem
try:
    import pymem
    import pymem.process
except ImportError:
    print("WARNING: 'pymem' library not found. Running in mock telemetry mode.")
    print("To read real CS2 memory, please run: pip install pymem Flask flask-cors")
    pymem = None

app = Flask(__name__)
CORS(app)

DUMPER_PATH = r"C:\Users\crazy\Downloads\cs2-dumper-main\cs2-dumper-main\output"
OFFSETS_FILE = os.path.join(DUMPER_PATH, "offsets.json")
CLIENT_DLL_FILE = os.path.join(DUMPER_PATH, "client_dll.json")

# Default offsets
offsets = {
    "dwLocalPlayerPawn": 0x1651B60,
    "dwLocalPlayerController": 0x162A480,
    "dwEntityList": 0x254F2D0,
    "dwViewMatrix": 0x165B880
}

fields = {
    "m_iHealth": 0x32C,
    "m_iTeamNum": 0x3BF,
    "m_vOldOrigin": 0x127C,
    "m_hPlayerPawn": 0x7E4,
    "m_sSanitizedPlayerName": 0x740
}

# Try loading from cs2-dumper output
if os.path.exists(OFFSETS_FILE) and os.path.exists(CLIENT_DLL_FILE):
    try:
        with open(OFFSETS_FILE, "r") as f:
            offset_data = json.load(f)
            client_offsets = offset_data.get("client.dll", {})
            for k in offsets.keys():
                if k in client_offsets:
                    offsets[k] = client_offsets[k]
                    
        with open(CLIENT_DLL_FILE, "r") as f:
            dll_data = json.load(f)
            classes = dll_data.get("client.dll", {}).get("classes", {})
            
            def extract_field(class_name, field_name, key):
                if class_name in classes:
                    field_val = classes[class_name].get("fields", {}).get(field_name)
                    if field_val is not None:
                        fields[key] = field_val

            extract_field("C_BaseEntity", "m_iHealth", "m_iHealth")
            extract_field("C_BaseEntity", "m_iTeamNum", "m_iTeamNum")
            extract_field("C_BasePlayerPawn", "m_vOldOrigin", "m_vOldOrigin")
            extract_field("CCSPlayerController", "m_hPlayerPawn", "m_hPlayerPawn")
            extract_field("CBasePlayerController", "m_sSanitizedPlayerName", "m_sSanitizedPlayerName")
            
        print("Offsets successfully loaded from cs2-dumper output.")
    except Exception as e:
        print(f"Error parsing dumper files, using built-in offsets: {e}")
else:
    print("Dumper output files not found at path, using default offsets.")

# Pymem process handlers
pm = None
client_base = 0

def connect_cs2():
    global pm, client_base
    if pymem is None:
        return False
    try:
        pm = pymem.Pymem("cs2.exe")
        client = pymem.process.module_from_name(pm.process_handle, "client.dll")
        if client:
            client_base = client.lpBaseOfDll
            return True
    except Exception:
        pass
    return False

# Math projection: 3D to 2D Screen
def world_to_screen(pos, matrix, width, height):
    clip_x = pos[0] * matrix[0] + pos[1] * matrix[1] + pos[2] * matrix[2] + matrix[3]
    clip_y = pos[0] * matrix[4] + pos[1] * matrix[5] + pos[2] * matrix[6] + matrix[7]
    clip_z = pos[0] * matrix[8] + pos[1] * matrix[9] + pos[2] * matrix[10] + matrix[11]
    clip_w = pos[0] * matrix[12] + pos[1] * matrix[13] + pos[2] * matrix[14] + matrix[15]

    if clip_w < 0.1:
        return None

    ndc_x = clip_x / clip_w
    ndc_y = clip_y / clip_w

    screen_x = (width / 2) + (ndc_x * width / 2)
    screen_y = (height / 2) - (ndc_y * height / 2)
    return screen_x, screen_y

# CS2 Netcon command dispatcher
def send_cs2_command(command):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(0.3)
        s.connect(("127.0.0.1", 2121))
        s.sendall((command + "\n").encode("utf-8"))
        s.close()
        return True
    except Exception as e:
        print(f"Netcon command connection failed (Make sure CS2 is run with '-netconport 2121'): {e}")
        return False

@app.route('/api/command', methods=['POST'])
def run_command():
    data = request.get_json() or {}
    cmd = data.get("command", "")
    if cmd:
        success = send_cs2_command(cmd)
        return jsonify({"status": "success" if success else "failed"})
    return jsonify({"status": "error", "message": "Command parameter missing"})

@app.route('/api/telemetry', methods=['GET'])
def get_telemetry():
    global pm, client_base
    if pm is None or not pm.process_handle:
        connect_cs2()
        
    if pm is None or client_base == 0:
        return jsonify({
            "status": "mock",
            "message": "CS2 process (cs2.exe) not running. Showing simulated nodes.",
            "targets": []
        })

    try:
        matrix_bytes = pm.read_bytes(client_base + offsets["dwViewMatrix"], 64)
        view_matrix = [float(x) for x in struct_unpack('16f', matrix_bytes)]
        
        local_player_controller = pm.read_longlong(client_base + offsets["dwLocalPlayerController"])
        local_pawn = pm.read_longlong(client_base + offsets["dwLocalPlayerPawn"])
        
        local_pos = (0, 0, 0)
        if local_pawn:
            px = pm.read_float(local_pawn + fields["m_vOldOrigin"])
            py = pm.read_float(local_pawn + fields["m_vOldOrigin"] + 4)
            pz = pm.read_float(local_pawn + fields["m_vOldOrigin"] + 8)
            local_pos = (px, py, pz)

        entity_list = pm.read_longlong(client_base + offsets["dwEntityList"])
        if not entity_list:
            return jsonify({"status": "error", "message": "Failed to read entity list.", "targets": []})

        targets = []
        for i in range(1, 64):
            try:
                list_entry = pm.read_longlong(entity_list + ((i & 0x7FFF) >> 9) * 8 + 16)
                if not list_entry:
                    continue
                    
                controller = pm.read_longlong(list_entry + 120 * (i & 0x1FF))
                if not controller or controller == local_player_controller:
                    continue

                health = pm.read_int(controller + fields["m_iHealth"])
                if health <= 0 or health > 100:
                    continue
                    
                team = pm.read_int(controller + fields["m_iTeamNum"])
                
                name_bytes = pm.read_bytes(controller + fields["m_sSanitizedPlayerName"], 64)
                name = name_bytes.split(b'\x00')[0].decode('utf-8', errors='ignore')
                if not name:
                    name = f"Bot_{i}"

                pawn_handle = pm.read_uint(controller + fields["m_hPlayerPawn"])
                if not pawn_handle:
                    continue
                    
                pawn_entry = pm.read_longlong(entity_list + 0x8 * ((pawn_handle & 0x7FFF) >> 9) + 16)
                if not pawn_entry:
                    continue
                    
                pawn = pm.read_longlong(pawn_entry + 120 * (pawn_handle & 0x1FF))
                if not pawn or pawn == local_pawn:
                    continue

                tx = pm.read_float(pawn + fields["m_vOldOrigin"])
                ty = pm.read_float(pawn + fields["m_vOldOrigin"] + 4)
                tz = pm.read_float(pawn + fields["m_vOldOrigin"] + 8)

                dx = tx - local_pos[0]
                dy = ty - local_pos[1]
                dz = tz - local_pos[2]
                distance = math.sqrt(dx*dx + dy*dy + dz*dz) * 0.0254

                screen = world_to_screen((tx, ty, tz), view_matrix, 800, 600)
                
                if screen:
                    targets.append({
                        "id": i,
                        "name": name,
                        "x": (screen[0] / 800) * 100,
                        "y": (screen[1] / 600) * 100,
                        "vx": 0,
                        "vy": 0,
                        "health": health,
                        "armor": 100,
                        "team": "Counter-Terrorist" if team == 3 else "Terrorist",
                        "spotted": True,
                        "weapon": "AK-47" if team == 2 else "M4A1",
                        "distance": distance
                    })
            except Exception:
                pass

        return jsonify({
            "status": "connected",
            "message": "Real-time CS2 memory hook engaged.",
            "targets": targets
        })
    except Exception as e:
        pm = None
        return jsonify({"status": "error", "message": str(e), "targets": []})

import struct
def struct_unpack(fmt, data):
    return struct.unpack(fmt, data)

if __name__ == '__main__':
    print("=" * 60)
    print(" OBSIDIAN CS2 TELEMETRY LINK SERVER")
    print("=" * 60)
    print("Starting Flask listener on http://localhost:5000")
    print("Connect your HUD React app to receive real-time player data.")
    print("Press Ctrl+C to stop.")
    app.run(port=5000, host="0.0.0.0", debug=False)
