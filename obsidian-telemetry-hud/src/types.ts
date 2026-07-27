export interface Target {
  id: number;
  name: string;
  x: number; // percentage of viewport width
  y: number; // percentage of viewport height
  vx: number; // velocity x
  vy: number; // velocity y
  health: number;
  armor: number;
  team: 'Terrorist' | 'Counter-Terrorist';
  spotted: boolean;
  weapon: string;
  distance: number; // in meters
  bones: { [key: number]: { x: number; y: number } }; // bone ID to relative screen coords
}

export interface HackConfig {
  esp_enabled: boolean;
  esp_box: boolean;
  esp_box_type: 'full' | 'corner' | '3d';
  esp_health_bar: boolean;
  esp_health_bar_style: 'vertical' | 'horizontal';
  esp_name: boolean;
  esp_weapon: boolean;
  esp_distance: boolean;
  esp_skeleton: boolean;
  esp_head_dot: boolean;
  esp_glow: boolean;
  
  aimbot_enabled: boolean;
  aimbot_fov: number; // in degrees / visual pixels
  aimbot_smooth: number; // lerp weight (1 = instant, higher = slower/smoother)
  aimbot_bone: string; // "HEAD" | "CHEST" | "PELVIS"
  aimbot_target_team: boolean;
  aimbot_rcs: boolean;
  aimbot_rcs_strength: number;
  aimbot_auto_fire: boolean;
  aimbot_visible_check: boolean;

  triggerbot_enabled: boolean;
  triggerbot_delay: number; // in ms
  triggerbot_hitchance: number; // 0-100%

  bhop_enabled: boolean;
  bhop_auto_strafe: boolean;
  bhop_legit_mode: boolean;

  noflash_enabled: boolean;
  noflash_max_alpha: number;

  radar_enabled: boolean;
  radar_scale: number;
  radar_size: number;

  spectator_list: boolean;
  bomb_timer: boolean;
  fps_counter: boolean;
  watermark: boolean;

  audio_sfx: boolean;
  vibe_theme: 'obsidian' | 'toxin' | 'crimson' | 'cyberpunk';
}

export interface TelemetryLog {
  id: string;
  timestamp: string;
  message: string;
  type: 'info' | 'success' | 'warn' | 'error' | 'critical';
}

export interface Spectator {
  id: number;
  name: string;
  ping: number;
  avatar: string;
  status: 'watching' | 'idle' | 'tracking';
}
