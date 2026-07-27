import React from 'react';
import { Target, HackConfig } from '../types';
import { BONES } from '../data';
import { Compass } from 'lucide-react';

interface RadarViewProps {
  targets: Target[];
  config: HackConfig;
  crosshairPos: { x: number; y: number };
}

export default function RadarView({ targets, config, crosshairPos }: RadarViewProps) {
  const radarSize = config.radar_size;
  const scale = config.radar_scale;
  const center = radarSize / 2;

  // Render simulated radar targets based on coordinate systems
  return (
    <div className="flex flex-col items-center bg-neutral-900/90 border border-neutral-800 p-4 rounded-lg relative overflow-hidden backdrop-blur-md">
      {/* Title */}
      <div className="w-full flex items-center justify-between mb-3 border-b border-neutral-800 pb-1.5 font-mono text-[11px] text-neutral-400">
        <span className="flex items-center gap-1.5 font-bold text-neutral-200">
          <Compass size={12} className="text-purple-400 animate-spin-slow" />
          AKTİF SİBER RADAR
        </span>
        <span>ÖLÇEK: {scale.toFixed(1)}x</span>
      </div>

      {/* Radar circle wrapper */}
      <div 
        className="relative bg-neutral-950 border border-purple-950/50 rounded-full flex items-center justify-center overflow-hidden"
        style={{ width: `${radarSize}px`, height: `${radarSize}px` }}
      >
        {/* Radar concentric circular grid rings */}
        <div className="absolute inset-0 border border-neutral-900 rounded-full scale-[0.3] pointer-events-none" />
        <div className="absolute inset-0 border border-neutral-900 rounded-full scale-[0.6] pointer-events-none" />
        <div className="absolute inset-0 border border-purple-950/20 rounded-full scale-[0.8] pointer-events-none" />

        {/* Dynamic sweeping line effect */}
        <div className="absolute inset-0 origin-center bg-gradient-to-r from-transparent via-purple-500/5 to-transparent animate-sweep pointer-events-none" />

        {/* Crosshairs axis lines */}
        <div className="absolute w-full h-[1px] bg-neutral-900/80 pointer-events-none" />
        <div className="absolute h-full w-[1px] bg-neutral-900/80 pointer-events-none" />

        {/* Local player marker in the dead center */}
        <div className="absolute z-10 w-2.5 h-2.5 bg-purple-500 rounded-full shadow-[0_0_8px_rgba(168,85,247,0.8)] border border-white flex items-center justify-center">
          <div className="w-1 h-1 bg-white rounded-full" />
        </div>

        {/* Relative Radar Targets mapped mathematically */}
        {targets.map((target) => {
          // Calculate relative vector from theoretical center point (e.g. coordinates 50, 50)
          const relX = (target.x - 50) * scale * 2.2;
          const relY = (target.y - 50) * scale * 2.2;

          // Compute absolute distance from center
          const dist = Math.sqrt(relX * relX + relY * relY);
          
          // Clamp points that fall outside the radar circumference
          const maxRadius = center - 6;
          let drawX = relX;
          let drawY = relY;

          if (dist > maxRadius) {
            const angle = Math.atan2(relY, relX);
            drawX = Math.cos(angle) * maxRadius;
            drawY = Math.sin(angle) * maxRadius;
          }

          const isTeammate = target.team === 'Counter-Terrorist';
          
          let colorClass = 'bg-red-500 shadow-[0_0_6px_rgba(239,68,68,0.8)]';
          if (isTeammate) {
            colorClass = 'bg-blue-500 shadow-[0_0_6px_rgba(59,130,246,0.8)]';
          } else if (config.vibe_theme === 'toxin') {
            colorClass = 'bg-green-500 shadow-[0_0_6px_rgba(34,197,94,0.8)]';
          } else if (config.vibe_theme === 'obsidian') {
            colorClass = target.spotted ? 'bg-orange-500 shadow-[0_0_6px_rgba(249,115,22,0.8)]' : 'bg-purple-500 shadow-[0_0_6px_rgba(168,85,247,0.8)]';
          } else if (config.vibe_theme === 'cyberpunk') {
            colorClass = 'bg-pink-500 shadow-[0_0_6px_rgba(236,72,153,0.8)]';
          }

          return (
            <div
              key={target.id}
              className={`absolute w-2 h-2 rounded-full border border-black/80 transition-all duration-300 ${colorClass}`}
              style={{
                left: `${center + drawX - 4}px`,
                top: `${center + drawY - 4}px`,
              }}
              title={`${target.name} [${target.team}]`}
            />
          );
        })}
      </div>

      {/* Footer Info info */}
      <div className="w-full mt-3 grid grid-cols-2 gap-2 text-[9px] font-mono text-neutral-400">
        <div className="bg-neutral-950 p-1.5 border border-neutral-800 rounded">
          <p className="text-neutral-500">HEDEF SAYISI</p>
          <p className="text-white font-bold">{targets.length}</p>
        </div>
        <div className="bg-neutral-950 p-1.5 border border-neutral-800 rounded">
          <p className="text-neutral-500">LOKAL SEKTÖR</p>
          <p className="text-purple-400 font-bold">SEKTÖR_09_GB</p>
        </div>
      </div>
    </div>
  );
}
