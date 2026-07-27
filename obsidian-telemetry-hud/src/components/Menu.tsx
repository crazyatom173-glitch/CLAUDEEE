import React from 'react';
import { HackConfig } from '../types';
import { Eye, Shield, Target, Flame, Activity, Compass, Users, Zap, Volume2, Info } from 'lucide-react';

interface MenuProps {
  config: HackConfig;
  onChange: (key: keyof HackConfig, value: any) => void;
  onLoadPreset: (theme: 'obsidian' | 'toxin' | 'crimson' | 'cyberpunk') => void;
}

export default function Menu({ config, onChange, onLoadPreset }: MenuProps) {
  return (
    <div className="bg-neutral-900 border border-neutral-800 rounded-lg overflow-hidden flex flex-col backdrop-blur-md">
      {/* Menu Header */}
      <div className="bg-gradient-to-r from-purple-950 via-neutral-900 to-purple-950 p-4 border-b border-neutral-800 flex items-center justify-between">
        <div>
          <h2 className="font-sans font-extrabold tracking-tight text-white text-base">
            OBSIDIAN TELEMETRİ SİSTEMİ
          </h2>
          <p className="text-[10px] font-mono text-purple-400">
            F2 DECK INTERACTION MODE // SÜRÜM 2.4.0
          </p>
        </div>
        <div className="flex items-center gap-2">
          <span className="w-2.5 h-2.5 rounded-full bg-green-500 animate-pulse border border-black" />
          <span className="text-[10px] font-mono text-green-500 uppercase tracking-widest font-bold">CANLI_VERİ</span>
        </div>
      </div>

      {/* Preset Launcher Buttons */}
      <div className="p-3 bg-neutral-950 border-b border-neutral-800 flex flex-wrap gap-2 items-center justify-between">
        <span className="text-[9px] font-mono text-neutral-500 font-bold uppercase">Hazır Tema Şablonları:</span>
        <div className="flex gap-1">
          <button
            onClick={() => onLoadPreset('obsidian')}
            className={`px-2.5 py-1 text-[10px] font-mono rounded font-bold uppercase transition-all duration-200 border ${
              config.vibe_theme === 'obsidian'
                ? 'bg-purple-900/30 text-purple-300 border-purple-500 shadow-[0_0_8px_rgba(168,85,247,0.3)]'
                : 'bg-neutral-900 text-neutral-400 border-neutral-800 hover:text-white'
            }`}
          >
            Obsidian
          </button>
          <button
            onClick={() => onLoadPreset('toxin')}
            className={`px-2.5 py-1 text-[10px] font-mono rounded font-bold uppercase transition-all duration-200 border ${
              config.vibe_theme === 'toxin'
                ? 'bg-green-900/30 text-green-300 border-green-500 shadow-[0_0_8px_rgba(34,197,94,0.3)]'
                : 'bg-neutral-900 text-neutral-400 border-neutral-800 hover:text-white'
            }`}
          >
            Toxin
          </button>
          <button
            onClick={() => onLoadPreset('crimson')}
            className={`px-2.5 py-1 text-[10px] font-mono rounded font-bold uppercase transition-all duration-200 border ${
              config.vibe_theme === 'crimson'
                ? 'bg-red-900/30 text-red-300 border-red-500 shadow-[0_0_8px_rgba(239,68,68,0.3)]'
                : 'bg-neutral-900 text-neutral-400 border-neutral-800 hover:text-white'
            }`}
          >
            Crimson
          </button>
          <button
            onClick={() => onLoadPreset('cyberpunk')}
            className={`px-2.5 py-1 text-[10px] font-mono rounded font-bold uppercase transition-all duration-200 border ${
              config.vibe_theme === 'cyberpunk'
                ? 'bg-pink-900/30 text-pink-300 border-pink-500 shadow-[0_0_8px_rgba(236,72,153,0.3)]'
                : 'bg-neutral-900 text-neutral-400 border-neutral-800 hover:text-white'
            }`}
          >
            Cyberpunk
          </button>
        </div>
      </div>

      {/* Main Configurations Container */}
      <div className="p-4 space-y-5 overflow-y-auto max-h-[600px] bg-neutral-900/50">
        
        {/* ESP Section */}
        <div className="space-y-3">
          <div className="flex items-center gap-2 border-b border-neutral-800 pb-1.5">
            <Eye size={14} className="text-purple-400" />
            <h3 className="font-mono text-[11px] font-bold text-neutral-200 uppercase tracking-wider">
              GELİŞMİŞ GÖRSEL ALGILAMA (ESP)
            </h3>
          </div>
          
          <div className="grid grid-cols-2 gap-x-4 gap-y-2">
            {/* Master Toggle */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.esp_enabled}
                onChange={(e) => onChange('esp_enabled', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span className={config.esp_enabled ? 'text-purple-400 font-bold' : ''}>ESP Aktif</span>
            </label>

            {/* ESP Box */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.esp_enabled}
                checked={config.esp_box}
                onChange={(e) => onChange('esp_box', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.esp_enabled ? 'opacity-40' : ''}>Sınır Kutusunu Çiz (Box)</span>
            </label>

            {/* Skeleton Map */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.esp_enabled}
                checked={config.esp_skeleton}
                onChange={(e) => onChange('esp_skeleton', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.esp_enabled ? 'opacity-40' : ''}>Kemik İskeletini Çiz</span>
            </label>

            {/* Head Dot */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.esp_enabled}
                checked={config.esp_head_dot}
                onChange={(e) => onChange('esp_head_dot', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.esp_enabled ? 'opacity-40' : ''}>Kritik Kafa Noktası</span>
            </label>

            {/* Health Bar */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.esp_enabled}
                checked={config.esp_health_bar}
                onChange={(e) => onChange('esp_health_bar', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.esp_enabled ? 'opacity-40' : ''}>Sağlık & Zırh Barları</span>
            </label>

            {/* Render Glow Overlay */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.esp_enabled}
                checked={config.esp_glow}
                onChange={(e) => onChange('esp_glow', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.esp_enabled ? 'opacity-40' : ''}>Dış Işıma Efekti (Glow)</span>
            </label>

            {/* Name Tags */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.esp_enabled}
                checked={config.esp_name}
                onChange={(e) => onChange('esp_name', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.esp_enabled ? 'opacity-40' : ''}>Hedef İsim Etiketleri</span>
            </label>

            {/* Display Weapon */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.esp_enabled}
                checked={config.esp_weapon}
                onChange={(e) => onChange('esp_weapon', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.esp_enabled ? 'opacity-40' : ''}>Taşıdığı Silah Bilgisi</span>
            </label>

            {/* Distance Indicator */}
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.esp_enabled}
                checked={config.esp_distance}
                onChange={(e) => onChange('esp_distance', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.esp_enabled ? 'opacity-40' : ''}>Telemetrik Mesafe</span>
            </label>
          </div>

          {/* Sub options dropdown */}
          <div className="grid grid-cols-2 gap-3 mt-2 bg-neutral-950/40 p-2.5 border border-neutral-800/60 rounded">
            <div>
              <span className="block text-[9px] font-mono text-neutral-500 uppercase font-bold mb-1">
                ESP KUTU TİPİ
              </span>
              <select
                disabled={!config.esp_enabled}
                value={config.esp_box_type}
                onChange={(e) => onChange('esp_box_type', e.target.value)}
                className="w-full bg-neutral-900 border border-neutral-800 rounded px-2 py-1 text-xs text-neutral-200 focus:ring-0 disabled:opacity-30"
              >
                <option value="full">Tam Kare (Full Box)</option>
                <option value="corner">Taktik Köşeler</option>
                <option value="3d">Yarı-3D İzometrik</option>
              </select>
            </div>
            <div>
              <span className="block text-[9px] font-mono text-neutral-500 uppercase font-bold mb-1">
                SAĞLIK BARI STİLİ
              </span>
              <select
                disabled={!config.esp_enabled}
                value={config.esp_health_bar_style}
                onChange={(e) => onChange('esp_health_bar_style', e.target.value)}
                className="w-full bg-neutral-900 border border-neutral-800 rounded px-2 py-1 text-xs text-neutral-200 focus:ring-0 disabled:opacity-30"
              >
                <option value="vertical">Dikey Sol</option>
                <option value="horizontal">Yatay Alt</option>
              </select>
            </div>
          </div>
        </div>

        {/* Dynamic Aimbot Control panel */}
        <div className="space-y-3">
          <div className="flex items-center gap-2 border-b border-neutral-800 pb-1.5">
            <Target size={14} className="text-purple-400" />
            <h3 className="font-mono text-[11px] font-bold text-neutral-200 uppercase tracking-wider">
              OTOMATİK HEDEF HİZALAYICI (AIMBOT)
            </h3>
          </div>

          <div className="grid grid-cols-2 gap-2">
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.aimbot_enabled}
                onChange={(e) => onChange('aimbot_enabled', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span className={config.aimbot_enabled ? 'text-purple-400 font-bold' : ''}>Aimbot Kilitlemesi</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.aimbot_enabled}
                checked={config.aimbot_rcs}
                onChange={(e) => onChange('aimbot_rcs', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.aimbot_enabled ? 'opacity-40' : ''}>Geri Tepme Kontrolü (RCS)</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.aimbot_enabled}
                checked={config.aimbot_target_team}
                onChange={(e) => onChange('aimbot_target_team', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.aimbot_enabled ? 'opacity-40' : ''}>Takım Arkadaşlarına Kilitle</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.aimbot_enabled}
                checked={config.aimbot_visible_check}
                onChange={(e) => onChange('aimbot_visible_check', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.aimbot_enabled ? 'opacity-40' : ''}>Sadece Görünür Hedefler</span>
            </label>
          </div>

          <div className="bg-neutral-950/40 p-3 border border-neutral-800/60 rounded space-y-3">
            {/* FOV Slider */}
            <div>
              <div className="flex justify-between text-[10px] font-mono text-neutral-400 mb-1">
                <span>KİLİTLEME GÖRÜŞ ALANI (FOV) LİMİTİ</span>
                <span className="text-purple-400 font-bold">{config.aimbot_fov.toFixed(0)}px</span>
              </div>
              <input
                type="range"
                min="20"
                max="250"
                disabled={!config.aimbot_enabled}
                value={config.aimbot_fov}
                onChange={(e) => onChange('aimbot_fov', Number(e.target.value))}
                className="w-full h-1 bg-neutral-800 rounded-lg appearance-none cursor-pointer accent-purple-500 disabled:opacity-30"
              />
            </div>

            {/* Smooth factor slider */}
            <div>
              <div className="flex justify-between text-[10px] font-mono text-neutral-400 mb-1">
                <span>HASSASİYET YUMUŞATMA FAKTÖRÜ</span>
                <span className="text-purple-400 font-bold">{config.aimbot_smooth.toFixed(1)}x</span>
              </div>
              <input
                type="range"
                min="1.0"
                max="15.0"
                step="0.5"
                disabled={!config.aimbot_enabled}
                value={config.aimbot_smooth}
                onChange={(e) => onChange('aimbot_smooth', Number(e.target.value))}
                className="w-full h-1 bg-neutral-800 rounded-lg appearance-none cursor-pointer accent-purple-500 disabled:opacity-30"
              />
            </div>

            <div className="grid grid-cols-2 gap-3">
              <div>
                <span className="block text-[9px] font-mono text-neutral-500 uppercase font-bold mb-1">
                  Hedef Kemik Odağı
                </span>
                <select
                  disabled={!config.aimbot_enabled}
                  value={config.aimbot_bone}
                  onChange={(e) => onChange('aimbot_bone', e.target.value)}
                  className="w-full bg-neutral-900 border border-neutral-800 rounded px-2 py-1 text-xs text-neutral-200 focus:ring-0 disabled:opacity-30"
                >
                  <option value="HEAD">Kemik: 6 [KAFA]</option>
                  <option value="CHEST">Kemik: 4 [GÖĞÜS]</option>
                  <option value="PELVIS">Kemik: 0 [PELVİS]</option>
                </select>
              </div>
              <div>
                <span className="block text-[9px] font-mono text-neutral-500 uppercase font-bold mb-1">
                  RCS Matris Gücü
                </span>
                <select
                  disabled={!config.aimbot_enabled || !config.aimbot_rcs}
                  value={config.aimbot_rcs_strength}
                  onChange={(e) => onChange('aimbot_rcs_strength', Number(e.target.value))}
                  className="w-full bg-neutral-900 border border-neutral-800 rounded px-2 py-1 text-xs text-neutral-200 focus:ring-0 disabled:opacity-30"
                >
                  <option value="0.5">50% Telafi</option>
                  <option value="0.8">80% Varsayılan</option>
                  <option value="1.0">100% Tam Kilitleme</option>
                </select>
              </div>
            </div>
          </div>
        </div>

        {/* Triggerbot and Hop Sections */}
        <div className="grid grid-cols-2 gap-4">
          
          {/* Triggerbot Panel */}
          <div className="bg-neutral-950/40 p-3 border border-neutral-800/60 rounded space-y-2.5">
            <div className="flex items-center gap-1.5 border-b border-neutral-800 pb-1">
              <Flame size={12} className="text-purple-400" />
              <h4 className="font-mono text-[9px] font-bold text-neutral-200 uppercase tracking-wider">
                OTOMATİK ATEŞLEME
              </h4>
            </div>

            <label className="flex items-center gap-2 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.triggerbot_enabled}
                onChange={(e) => onChange('triggerbot_enabled', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span className={config.triggerbot_enabled ? 'text-purple-400 font-bold' : ''}>Aktif Ateş</span>
            </label>

            <div>
              <div className="flex justify-between text-[8px] font-mono text-neutral-500 mb-0.5">
                <span>GECİKME SÜRESİ</span>
                <span className="text-white">{config.triggerbot_delay}ms</span>
              </div>
              <input
                type="range"
                min="0"
                max="150"
                step="5"
                disabled={!config.triggerbot_enabled}
                value={config.triggerbot_delay}
                onChange={(e) => onChange('triggerbot_delay', Number(e.target.value))}
                className="w-full h-1 bg-neutral-800 rounded-lg appearance-none cursor-pointer accent-purple-500 disabled:opacity-30"
              />
            </div>

            <div>
              <div className="flex justify-between text-[8px] font-mono text-neutral-500 mb-0.5">
                <span>İSABET ORANI</span>
                <span className="text-white">{config.triggerbot_hitchance}%</span>
              </div>
              <input
                type="range"
                min="20"
                max="100"
                disabled={!config.triggerbot_enabled}
                value={config.triggerbot_hitchance}
                onChange={(e) => onChange('triggerbot_hitchance', Number(e.target.value))}
                className="w-full h-1 bg-neutral-800 rounded-lg appearance-none cursor-pointer accent-purple-500 disabled:opacity-30"
              />
            </div>
          </div>

          {/* Bunnyhop Panel */}
          <div className="bg-neutral-950/40 p-3 border border-neutral-800/60 rounded space-y-2.5">
            <div className="flex items-center gap-1.5 border-b border-neutral-800 pb-1">
              <Zap size={12} className="text-purple-400" />
              <h4 className="font-mono text-[9px] font-bold text-neutral-200 uppercase tracking-wider">
                OTOMATİK ZIPLAMA (BHOP)
              </h4>
            </div>

            <label className="flex items-center gap-2 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.bhop_enabled}
                onChange={(e) => onChange('bhop_enabled', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span className={config.bhop_enabled ? 'text-purple-400 font-bold' : ''}>BHOP Aktif</span>
            </label>

            <label className="flex items-center gap-2 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.bhop_enabled}
                checked={config.bhop_auto_strafe}
                onChange={(e) => onChange('bhop_auto_strafe', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.bhop_enabled ? 'opacity-40' : ''}>Otomatik Hava Yönü</span>
            </label>

            <label className="flex items-center gap-2 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                disabled={!config.bhop_enabled}
                checked={config.bhop_legit_mode}
                onChange={(e) => onChange('bhop_legit_mode', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0 disabled:opacity-30"
              />
              <span className={!config.bhop_enabled ? 'opacity-40' : ''}>İnsansı Yumuşak Bhop</span>
            </label>
          </div>
        </div>

        {/* Miscellaneous Features */}
        <div className="space-y-3">
          <div className="flex items-center gap-2 border-b border-neutral-800 pb-1.5">
            <Activity size={14} className="text-purple-400" />
            <h3 className="font-mono text-[11px] font-bold text-neutral-200 uppercase tracking-wider">
              EK TELEMETRİ GÖSTERGELERİ (WIDGETS)
            </h3>
          </div>

          <div className="grid grid-cols-2 gap-x-4 gap-y-2">
            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.radar_enabled}
                onChange={(e) => onChange('radar_enabled', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span>Canlı Vektör Radarı</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.spectator_list}
                onChange={(e) => onChange('spectator_list', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span>İzleyici Kontrol Listesi</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.bomb_timer}
                onChange={(e) => onChange('bomb_timer', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span>Bomba (C4) Patlama Saati</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.noflash_enabled}
                onChange={(e) => onChange('noflash_enabled', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span>Körlük Engelleyici (Anti-Flash)</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.watermark}
                onChange={(e) => onChange('watermark', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span>Filigran Logosu Göster</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.fps_counter}
                onChange={(e) => onChange('fps_counter', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span>FPS & Ping Göstergesi</span>
            </label>

            <label className="flex items-center gap-2.5 cursor-pointer text-xs text-neutral-300 hover:text-white select-none">
              <input
                type="checkbox"
                checked={config.audio_sfx}
                onChange={(e) => onChange('audio_sfx', e.target.checked)}
                className="rounded border-neutral-800 bg-neutral-950 text-purple-600 focus:ring-0"
              />
              <span className="flex items-center gap-1">
                <Volume2 size={12} className="text-purple-400" />
                Siber Ses Efektleri (SFX)
              </span>
            </label>
          </div>

          <div className="grid grid-cols-2 gap-3 bg-neutral-950/40 p-2.5 border border-neutral-800/60 rounded">
            <div>
              <span className="block text-[9px] font-mono text-neutral-500 uppercase font-bold mb-1">
                RADAR ÖLÇEK KATSAYISI
              </span>
              <input
                type="range"
                min="0.1"
                max="1.0"
                step="0.05"
                disabled={!config.radar_enabled}
                value={config.radar_scale}
                onChange={(e) => onChange('radar_scale', Number(e.target.value))}
                className="w-full h-1 bg-neutral-800 rounded-lg appearance-none cursor-pointer accent-purple-500 disabled:opacity-30"
              />
            </div>
            <div>
              <span className="block text-[9px] font-mono text-neutral-500 uppercase font-bold mb-1">
                ANTİ-FLASH ETKİ LİMİTİ
              </span>
              <input
                type="range"
                min="0"
                max="100"
                disabled={!config.noflash_enabled}
                value={config.noflash_max_alpha}
                onChange={(e) => onChange('noflash_max_alpha', Number(e.target.value))}
                className="w-full h-1 bg-neutral-800 rounded-lg appearance-none cursor-pointer accent-purple-500 disabled:opacity-30"
              />
            </div>
          </div>
        </div>
      </div>

      {/* Menu Footer */}
      <div className="bg-neutral-950 p-3.5 border-t border-neutral-800 flex justify-between items-center text-[10px] font-mono text-neutral-500">
        <span className="flex items-center gap-1">
          <Info size={12} className="text-purple-400" />
          F2: TIKLAMA GEÇİŞ SİMÜLASYONU
        </span>
        <span className="text-purple-400/80 font-bold uppercase tracking-wider">
          AKADEMİK VE TANITIM ALANI
        </span>
      </div>
    </div>
  );
}
