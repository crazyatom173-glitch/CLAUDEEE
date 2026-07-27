import React, { useEffect, useState } from 'react';
import { Spectator } from '../types';
import { Radio, Users, Cpu, Shield } from 'lucide-react';

interface SpectatorListProps {
  spectatorListEnabled: boolean;
}

export default function SpectatorList({ spectatorListEnabled }: SpectatorListProps) {
  const [spectators, setSpectators] = useState<Spectator[]>([
    { id: 1, name: "Kael_Thas", ping: 14, avatar: "💀", status: "watching" },
    { id: 2, name: "Onyxia_99", ping: 48, avatar: "🐉", status: "idle" },
    { id: 3, name: "Sargeras", ping: 5, avatar: "🔥", status: "tracking" }
  ]);

  // Simulate subtle ping fluctuations
  useEffect(() => {
    const interval = setInterval(() => {
      setSpectators((prev) =>
        prev.map((s) => ({
          ...s,
          ping: Math.max(2, Math.min(120, s.ping + Math.floor(Math.random() * 7) - 3))
        }))
      );
    }, 3000);
    return () => clearInterval(interval);
  }, []);

  if (!spectatorListEnabled) return null;

  return (
    <div className="bg-neutral-900 border border-neutral-800 p-4 rounded-lg relative overflow-hidden backdrop-blur-md">
      <div className="flex items-center justify-between border-b border-neutral-800 pb-1.5 mb-3 font-mono text-[11px] text-neutral-400">
        <span className="flex items-center gap-1.5 font-bold text-neutral-200">
          <Users size={12} className="text-purple-400" />
          İZLEYİCİ RADAR MATRİSİ
        </span>
        <span className="flex items-center gap-1 text-red-500 font-bold animate-pulse">
          <Radio size={10} />
          AKTİF_İZLEYİCİLER
        </span>
      </div>

      <div className="space-y-2">
        {spectators.map((spec) => (
          <div 
            key={spec.id} 
            className="flex items-center justify-between bg-neutral-950/80 border border-neutral-800/60 p-2 rounded hover:border-purple-950 transition-all duration-200"
          >
            <div className="flex items-center gap-2">
              <span className="text-sm select-none">{spec.avatar}</span>
              <div className="font-mono text-xs">
                <p className="text-neutral-200 font-bold">{spec.name}</p>
                <p className="text-[9px] text-neutral-500 uppercase flex items-center gap-1">
                  <Shield size={8} className="text-purple-500" />
                  GECİKME (PING): {spec.ping}ms
                </p>
              </div>
            </div>
            
            <span className={`px-1.5 py-0.5 text-[8px] font-mono rounded font-bold uppercase ${
              spec.status === 'watching' 
                ? 'bg-purple-900/40 text-purple-300 border border-purple-800/40' 
                : (spec.status === 'tracking' ? 'bg-red-950/40 text-red-300 border border-red-900/30' : 'bg-neutral-900 text-neutral-500')
            }`}>
              {spec.status === 'watching' ? 'İzliyor' : (spec.status === 'tracking' ? 'Takipte' : 'Boşta')}
            </span>
          </div>
        ))}
      </div>
    </div>
  );
}
