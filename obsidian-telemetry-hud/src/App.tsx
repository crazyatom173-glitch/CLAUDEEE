import React, { useState, useEffect } from 'react';
import { Target, HackConfig, TelemetryLog } from './types';
import { INITIAL_TARGETS, BONES } from './data';
import GameView from './components/GameView';
import RadarView from './components/RadarView';
import Menu from './components/Menu';
import LoggerView from './components/LoggerView';
import SpectatorList from './components/SpectatorList';
import BombTimer from './components/BombTimer';
import { Terminal, Shield, RefreshCw, Skull, Compass, Activity, Volume2, ShieldAlert } from 'lucide-react';

export default function App() {
  // Global Cheat engine state configuration 
  const [config, setConfig] = useState<HackConfig>({
    esp_enabled: true,
    esp_box: true,
    esp_box_type: 'corner',
    esp_health_bar: true,
    esp_health_bar_style: 'vertical',
    esp_name: true,
    esp_weapon: true,
    esp_distance: true,
    esp_skeleton: true,
    esp_head_dot: true,
    esp_glow: true,
    
    aimbot_enabled: true,
    aimbot_fov: 90,
    aimbot_smooth: 4.5,
    aimbot_bone: 'HEAD',
    aimbot_target_team: false,
    aimbot_rcs: true,
    aimbot_rcs_strength: 0.8,
    aimbot_auto_fire: false,
    aimbot_visible_check: true,

    triggerbot_enabled: true,
    triggerbot_delay: 20,
    triggerbot_hitchance: 90,

    bhop_enabled: true,
    bhop_auto_strafe: true,
    bhop_legit_mode: false,

    noflash_enabled: true,
    noflash_max_alpha: 25,

    radar_enabled: true,
    radar_scale: 0.5,
    radar_size: 200,

    spectator_list: true,
    bomb_timer: true,
    fps_counter: true,
    watermark: true,

    audio_sfx: true,
    vibe_theme: 'obsidian'
  });

  // Simulation targets list 
  const [targets, setTargets] = useState<Target[]>(INITIAL_TARGETS);

  // CS2 Live Process Memory Telemetry link states
  const [isLiveLink, setIsLiveLink] = useState(false);
  const [liveStatus, setLiveStatus] = useState<'standby' | 'mock' | 'connected'>('standby');

  // Simulated crosshair tracking state
  const [crosshairPos, setCrosshairPos] = useState({ x: 300, y: 250 });

  // Telemetry event log list
  const [logs, setLogs] = useState<TelemetryLog[]>([
    { id: '1', timestamp: '08:14:51', message: 'VAC Bypass devre dışı bırakıldı. cs2.exe işlemi bulundu.', type: 'success' },
    { id: '2', timestamp: '08:14:52', message: 'client.dll modülü 0x7FFA8F400000 adresine başarıyla haritalandı.', type: 'info' },
    { id: '3', timestamp: '08:14:53', message: 'PEB bağlantısı başarıyla kesildi.', type: 'success' },
    { id: '4', timestamp: '08:14:54', message: 'Obsidian Harici Katman ve Telemetri döngüleri aktif edildi.', type: 'info' }
  ]);

  // Audio auditory SFX simulation
  const playSfx = (type: 'lock' | 'fire' | 'warn' | 'success') => {
    if (!config.audio_sfx) return;
    try {
      const audioCtx = new (window.AudioContext || (window as any).webkitAudioContext)();
      const osc = audioCtx.createOscillator();
      const gain = audioCtx.createGain();
      osc.connect(gain);
      gain.connect(audioCtx.destination);

      const now = audioCtx.currentTime;

      if (type === 'lock') {
        // High pitched click/lock
        osc.frequency.setValueAtTime(880, now);
        osc.frequency.exponentialRampToValueAtTime(1400, now + 0.08);
        gain.gain.setValueAtTime(0.04, now);
        gain.gain.exponentialRampToValueAtTime(0.01, now + 0.08);
        osc.start(now);
        osc.stop(now + 0.08);
      } else if (type === 'fire') {
        // Simulated low cyber snap
        osc.frequency.setValueAtTime(220, now);
        osc.frequency.exponentialRampToValueAtTime(80, now + 0.1);
        gain.gain.setValueAtTime(0.1, now);
        gain.gain.exponentialRampToValueAtTime(0.01, now + 0.15);
        osc.start(now);
        osc.stop(now + 0.15);
      } else if (type === 'warn') {
        // Double tone warning pulse
        osc.type = 'sawtooth';
        osc.frequency.setValueAtTime(330, now);
        osc.frequency.setValueAtTime(220, now + 0.1);
        gain.gain.setValueAtTime(0.03, now);
        gain.gain.exponentialRampToValueAtTime(0.01, now + 0.25);
        osc.start(now);
        osc.stop(now + 0.25);
      } else if (type === 'success') {
        // Ambient chime
        osc.type = 'sine';
        osc.frequency.setValueAtTime(523.25, now); // C5
        osc.frequency.exponentialRampToValueAtTime(783.99, now + 0.15); // G5
        gain.gain.setValueAtTime(0.05, now);
        gain.gain.exponentialRampToValueAtTime(0.01, now + 0.3);
        osc.start(now);
        osc.stop(now + 0.3);
      }
    } catch (e) {
      // AudioContext support failure, safe failover
    }
  };

  // Add a new system log
  const pushLog = (message: string, type: TelemetryLog['type'] = 'info') => {
    const now = new Date();
    const timestamp = now.toTimeString().split(' ')[0];
    const newLog: TelemetryLog = {
      id: Math.random().toString(),
      timestamp,
      message,
      type
    };
    setLogs((prev) => [newLog, ...prev.slice(0, 48)]); // keep last 50 logs max
  };

  // Aimbot action triggered callback
  const handleAimbotTrigger = (targetName: string, bone: string) => {
    // Avoid spamming identical lock messages
    setLogs((prev) => {
      const lastLog = prev[0];
      if (lastLog && lastLog.message.includes(`AIM_LOCK engaged on ${targetName}`)) {
        return prev;
      }
      playSfx('lock');
      // Push asynchronous telemetry state log
      const now = new Date();
      const timestamp = now.toTimeString().split(' ')[0];
      const newLog: TelemetryLog = {
        id: Math.random().toString(),
        timestamp,
        message: `Aimbot ${targetName} hedefine kilitlendi [Odak: ${bone === 'HEAD' ? 'KAFA' : (bone === 'CHEST' ? 'GÖĞÜS' : 'PELVİS')}]`,
        type: 'info'
      };
      return [newLog, ...prev.slice(0, 48)];
    });
  };

  // Triggerbot simulated weapon fire callback
  const handleTriggerbotFire = (targetName: string) => {
    setLogs((prev) => {
      const lastLog = prev[0];
      // Throttle rapid trigger fires
      if (lastLog && lastLog.message.includes(`TRIGGERBOT command sent`)) {
        return prev;
      }
      playSfx('fire');
      
      // Simulate inflicting random health damage to the targets
      setTargets((prevTargets) =>
        prevTargets.map((t) => {
          if (t.name === targetName) {
            const nextHealth = Math.max(0, t.health - Math.floor(Math.random() * 25) - 10);
            return {
              ...t,
              health: nextHealth,
              spotted: true // make visible upon gunshot
            };
          }
          return t;
        })
      );

      const now = new Date();
      const timestamp = now.toTimeString().split(' ')[0];
      const newLog: TelemetryLog = {
        id: Math.random().toString(),
        timestamp,
        message: `Triggerbot tetiklendi: ${targetName} hedefine ateş edildi`,
        type: 'success'
      };
      return [newLog, ...prev.slice(0, 48)];
    });
  };

  // Bomb detonation warning logging 
  const handleBombExplode = () => {
    playSfx('warn');
    pushLog("C4 zamanlayıcı tetiklendi! Sektör haritası yeniden taranıyor.", "critical");
    
    // Revive dead players when bomb resets
    setTargets((prevTargets) =>
      prevTargets.map((t) => {
        if (t.health <= 0) {
          pushLog(`Sistem ${t.name} hedefini tam canla yeniden başlattı`, 'success');
          return {
            ...t,
            health: 100,
            spotted: false
          };
        }
        return t;
      })
    );
  };

  // Poll Flask memory dumper telemetry
  useEffect(() => {
    let lastStatus = 'standby';
    const poll = () => {
      fetch('http://localhost:5000/api/telemetry')
        .then((res) => res.json())
        .then((data) => {
          if (data.status === 'connected') {
            setIsLiveLink(true);
            setLiveStatus('connected');
            if (lastStatus !== 'connected') {
              pushLog("Canlı CS2 Bellek Bağlantısı Kuruldu!", "success");
              lastStatus = 'connected';
            }
            if (data.targets) {
              setTargets(data.targets);
            }
          } else {
            setIsLiveLink(false);
            setLiveStatus(data.status || 'mock');
            if (lastStatus === 'connected') {
              pushLog("Canlı CS2 bağlantısı kesildi, simülasyon moduna dönülüyor.", "warn");
              lastStatus = 'mock';
            }
          }
        })
        .catch(() => {
          setIsLiveLink(false);
          setLiveStatus('mock');
          if (lastStatus === 'connected') {
            pushLog("Canlı CS2 bağlantısı koptu, simülasyona dönülüyor.", "warn");
            lastStatus = 'mock';
          }
        });
    };
    
    const interval = setInterval(poll, 60);
    return () => clearInterval(interval);
  }, []);

  // Toggle general config change
  const handleConfigChange = (key: keyof HackConfig, value: any) => {
    setConfig((prev) => ({
      ...prev,
      [key]: value
    }));
    pushLog(`Yapılandırma güncellendi: ${String(key)} değeri ${String(value)} yapıldı`, 'info');
  };

  // Preset loading handler
  const handleLoadPreset = (theme: 'obsidian' | 'toxin' | 'crimson' | 'cyberpunk') => {
    playSfx('success');
    let themeConfig: Partial<HackConfig> = {};
    if (theme === 'obsidian') {
      themeConfig = {
        vibe_theme: 'obsidian',
        esp_enabled: true,
        esp_box_type: 'corner',
        esp_glow: true,
        aimbot_fov: 90,
        aimbot_smooth: 4.5,
      };
    } else if (theme === 'toxin') {
      themeConfig = {
        vibe_theme: 'toxin',
        esp_enabled: true,
        esp_box_type: 'full',
        esp_glow: true,
        aimbot_fov: 120,
        aimbot_smooth: 2.0,
      };
    } else if (theme === 'crimson') {
      themeConfig = {
        vibe_theme: 'crimson',
        esp_enabled: true,
        esp_box_type: '3d',
        esp_glow: true,
        aimbot_fov: 150,
        aimbot_smooth: 1.0,
      };
    } else if (theme === 'cyberpunk') {
      themeConfig = {
        vibe_theme: 'cyberpunk',
        esp_enabled: true,
        esp_box_type: 'corner',
        esp_glow: true,
        aimbot_fov: 70,
        aimbot_smooth: 6.0,
      };
    }

    setConfig((prev) => ({
      ...prev,
      ...themeConfig
    }));
    pushLog(`TEMA PROTOKOLÜ AKTİF EDİLDİ: [${theme.toUpperCase()}]`, 'success');
  };

  // Select theme styles
  let themeHeaderBorder = 'border-purple-900/60';
  let glowAccent = 'shadow-[0_0_15px_rgba(168,85,247,0.15)]';
  let textAccent = 'text-purple-400';
  let bannerGradient = 'from-purple-950 via-neutral-900 to-purple-950';

  if (config.vibe_theme === 'toxin') {
    themeHeaderBorder = 'border-green-900/60';
    glowAccent = 'shadow-[0_0_15px_rgba(34,197,94,0.15)]';
    textAccent = 'text-green-400';
    bannerGradient = 'from-green-950 via-neutral-900 to-green-950';
  } else if (config.vibe_theme === 'crimson') {
    themeHeaderBorder = 'border-red-900/60';
    glowAccent = 'shadow-[0_0_15px_rgba(239,68,68,0.15)]';
    textAccent = 'text-red-400';
    bannerGradient = 'from-red-950 via-neutral-900 to-red-950';
  } else if (config.vibe_theme === 'cyberpunk') {
    themeHeaderBorder = 'border-pink-900/60';
    glowAccent = 'shadow-[0_0_15px_rgba(236,72,153,0.15)]';
    textAccent = 'text-pink-400';
    bannerGradient = 'from-pink-950 via-neutral-900 to-pink-950';
  }

  return (
    <div className="min-h-screen bg-neutral-950 text-white flex flex-col font-sans selection:bg-purple-900 selection:text-white">
      {/* Decorative cybernetic gothic top accent bar */}
      <div className={`h-1 bg-gradient-to-r ${config.vibe_theme === 'obsidian' ? 'from-purple-600 via-pink-600 to-cyan-500' : (config.vibe_theme === 'toxin' ? 'from-green-500 via-emerald-600 to-yellow-500' : (config.vibe_theme === 'crimson' ? 'from-red-600 via-rose-700 to-orange-500' : 'from-pink-500 via-purple-600 to-blue-500'))}`} />

      {/* Premium Navigation Header */}
      <header className="bg-neutral-900/80 backdrop-blur-md border-b border-neutral-800/80 px-6 py-4 flex items-center justify-between sticky top-0 z-50">
        <div className="flex items-center gap-3">
          <div className="p-2 bg-neutral-950 border border-neutral-800 rounded">
            <Skull className={`w-5 h-5 ${textAccent} animate-pulse`} />
          </div>
          <div>
            <div className="flex items-center gap-2">
              <span className="font-extrabold tracking-tight text-white text-lg">OBSIDIAN TELEMETRİ</span>
              <span className="bg-purple-950 text-purple-300 text-[9px] px-1.5 py-0.5 rounded border border-purple-800/50 font-mono font-bold">
                PRO v2.4
              </span>
            </div>
            <p className="text-[10px] text-neutral-500 font-mono">
              HARİCİ ESP VE OTOMATİK KİLİTLEME KONTROL PANELİ // CS2 İSTEMCİ AĞI
            </p>
          </div>
        </div>

        {/* Global HUD Stats indicators */}
        <div className="flex items-center gap-6 font-mono text-xs text-neutral-400">
          <div className={`px-2 py-0.5 rounded text-[9px] font-bold border uppercase transition-all duration-300 ${
            isLiveLink 
              ? 'bg-green-950/30 text-green-400 border-green-500/30 shadow-[0_0_8px_rgba(34,197,94,0.2)]' 
              : 'bg-neutral-950 text-neutral-500 border-neutral-800'
          }`}>
            {isLiveLink ? '● CANLI BAĞLANTI' : '○ SİMÜLATÖR MODU'}
          </div>
          <div className="hidden md:block">
            <span className="text-neutral-500">GECİKME:</span>{' '}
            <span className="text-green-400 font-bold">12ms</span>
          </div>
          <div className="hidden md:block">
            <span className="text-neutral-500">SİMÜLATÖR FPS:</span>{' '}
            <span className={`font-bold ${config.fps_counter ? 'text-green-400' : 'text-neutral-500'}`}>
              {config.fps_counter ? '144' : 'N/A'}
            </span>
          </div>
          <div className="bg-neutral-950 px-3 py-1.5 border border-neutral-800 rounded flex items-center gap-1.5">
            <span className="w-1.5 h-1.5 rounded-full bg-purple-500 animate-ping" />
            <span className="text-neutral-300">OTURUM: 08:14:51</span>
          </div>
        </div>
      </header>

      {/* Main Responsive Grid Layout containing interactive components */}
      <main className="flex-1 max-w-7xl w-full mx-auto p-4 md:p-6 grid grid-cols-1 lg:grid-cols-12 gap-6">
        
        {/* Left Side: Game simulation & logs (Grid Span 8) */}
        <div className="lg:col-span-8 flex flex-col gap-6">
          
          {/* Top Banner Notice regarding the simulation purpose */}
          <div className={`p-4 rounded-lg bg-gradient-to-r ${bannerGradient} border border-neutral-800 flex flex-col md:flex-row items-start md:items-center justify-between gap-3 ${glowAccent} transition-all duration-300`}>
            <div>
              <h1 className="text-sm font-sans font-bold text-white flex items-center gap-2">
                <ShieldAlert className="w-4 h-4 text-purple-400" />
                TAKTİK HUD KONTROL SİMÜLATÖRÜ
              </h1>
              <p className="text-[11px] text-neutral-400 mt-1">
                Aşağıdaki simüle edilmiş istemci penceresinde hedeflerin üzerine gelebilir, sürükleyebilir ve tıklayabilirsiniz. Çizim biçimlerini, kilitlenme ayarlarını ve renkleri ayarlamak için sağ taraftaki siber-gotik HUD paneli seçeneklerini kullanın.
              </p>
            </div>
            <button
              onClick={() => {
                setTargets(INITIAL_TARGETS);
                pushLog('Simülasyon koordinatları ve varlıkları sıfırlandı', 'info');
                playSfx('success');
              }}
              className="px-3 py-1.5 bg-neutral-950 hover:bg-neutral-900 border border-neutral-800 rounded text-xs font-mono font-bold flex items-center gap-1.5 transition-all"
            >
              <RefreshCw size={12} className="text-purple-400" />
              HEDEFLERİ SIFIRLA
            </button>
          </div>

          {/* Core Interactive Simulation View */}
          <GameView
            targets={targets}
            setTargets={setTargets}
            config={config}
            crosshairPos={crosshairPos}
            setCrosshairPos={setCrosshairPos}
            onAimbotTrigger={handleAimbotTrigger}
            onTriggerbotFire={handleTriggerbotFire}
            isLiveLink={isLiveLink}
          />

          {/* Real-time System Logger Component */}
          <LoggerView 
            logs={logs} 
            onClear={() => {
              setLogs([]);
              playSfx('lock');
            }} 
          />

        </div>

        {/* Right Side: Options Menu & Telemetry overlays (Grid Span 4) */}
        <div className="lg:col-span-4 flex flex-col gap-6">
          
          {/* Menu Controller Component */}
          <Menu
            config={config}
            onChange={handleConfigChange}
            onLoadPreset={handleLoadPreset}
          />

          {/* CS2 Remote Control Widget */}
          <div className="bg-neutral-900 border border-neutral-800 p-4 rounded-lg backdrop-blur-md space-y-3">
            <div className="flex items-center justify-between border-b border-neutral-800 pb-1.5 font-mono text-[11px] text-neutral-400">
              <span className="flex items-center gap-1.5 font-bold text-neutral-200">
                <Terminal size={12} className="text-purple-400 animate-pulse" />
                CS2 UZAKTAN KONTROL
              </span>
              <span className="text-[9px] text-neutral-500 font-bold">PORT: 2121</span>
            </div>
            
            <p className="text-[10px] text-neutral-400 leading-relaxed font-sans">
              Steam Başlatma Seçeneklerine <code className="text-purple-400 font-mono bg-neutral-950 px-1 py-0.5 rounded">-netconport 2121</code> kodunu ekleyip oyunu açın. Aşağıdaki butonlarla tek tıkla oyunu yönetin:
            </p>

            <div className="grid grid-cols-2 gap-2 text-[10px] font-mono">
              <button
                onClick={() => {
                  fetch('http://localhost:5000/api/command', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ command: 'map cs_office' })
                  }).then(() => pushLog("CS2 Komut Gönderildi: map cs_office", "info"));
                }}
                className="px-2 py-1.5 bg-purple-950/40 hover:bg-purple-900/60 border border-purple-800/40 text-purple-300 font-bold rounded uppercase transition-colors"
              >
                Office Haritası Aç
              </button>
              
              <button
                onClick={() => {
                  fetch('http://localhost:5000/api/command', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ command: 'sv_cheats 1; mp_autoteambalance 0; mp_limitteams 0; bot_kick; bot_add_t; bot_stop 1; mp_warmup_end' })
                  }).then(() => pushLog("CS2 Komut Gönderildi: Teke Tek Bot Ekle", "info"));
                }}
                className="px-2 py-1.5 bg-green-950/40 hover:bg-green-900/60 border border-green-800/40 text-green-300 font-bold rounded uppercase transition-colors"
              >
                Teke Tek Bot Ekle
              </button>

              <button
                onClick={() => {
                  fetch('http://localhost:5000/api/command', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ command: 'toggle bot_stop' })
                  }).then(() => pushLog("CS2 Komut Gönderildi: bot_stop değiştirildi", "info"));
                }}
                className="px-2 py-1.5 bg-neutral-800 hover:bg-neutral-700 border border-neutral-700 text-neutral-200 rounded uppercase transition-colors"
              >
                Botu Dondur/Çöz
              </button>

              <button
                onClick={() => {
                  fetch('http://localhost:5000/api/command', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ command: 'bot_kick' })
                  }).then(() => pushLog("CS2 Komut Gönderildi: bot_kick", "info"));
                }}
                className="px-2 py-1.5 bg-red-950/40 hover:bg-red-900/60 border border-red-800/40 text-red-400 font-bold rounded uppercase transition-colors"
              >
                Tüm Botları At
              </button>
            </div>
          </div>

          {/* Radar Component */}
          {config.radar_enabled && (
            <RadarView
              targets={targets}
              config={config}
              crosshairPos={crosshairPos}
            />
          )}

          {/* Extra Overlays Widgets (Bomb & Observers) */}
          <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-1 gap-6">
            <BombTimer config={config} onExplode={handleBombExplode} />
            <SpectatorList spectatorListEnabled={config.spectator_list} />
          </div>

        </div>

      </main>

      {/* System Footer info */}
      <footer className="bg-neutral-950 border-t border-neutral-900 py-4 px-6 text-center text-xs text-neutral-600 font-mono mt-auto flex flex-col sm:flex-row justify-between items-center gap-2">
        <p className="flex items-center gap-1">
          <Volume2 size={12} className="text-purple-500 animate-pulse" />
          İşitsel Ses Sentezi Aktif - Gotik Ambiyans modu etkin
        </p>
        <p className="uppercase tracking-widest text-[9px]">
          LO ONAYLI PANEL // SİBER ALANDA BOZULMAYI ÖNLER
        </p>
      </footer>
    </div>
  );
}
