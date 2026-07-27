import React, { useEffect, useState } from 'react';
import { HackConfig } from '../types';
import { Skull, AlertTriangle, ShieldCheck } from 'lucide-react';

interface BombTimerProps {
  config: HackConfig;
  onExplode: () => void;
}

export default function BombTimer({ config, onExplode }: BombTimerProps) {
  const [isPlanted, setIsPlanted] = useState(false);
  const [timeLeft, setTimeLeft] = useState(40.0);
  const [defuseProgress, setDefuseProgress] = useState(0);
  const [isDefusing, setIsDefusing] = useState(false);
  const [hasKit, setHasKit] = useState(true);

  // Trigger random plant/defuse cycles for simulator visual play
  useEffect(() => {
    let interval: number;
    let defuseTimeout: number;

    const startSimulationCycle = () => {
      // Plant the bomb
      setIsPlanted(true);
      setTimeLeft(40.0);
      setDefuseProgress(0);
      setIsDefusing(false);

      // Defuse trigger after 15 seconds
      defuseTimeout = window.setTimeout(() => {
        setIsDefusing(true);
        const kitState = Math.random() > 0.5;
        setHasKit(kitState);
      }, 15000);

      // Countdown loop
      interval = window.setInterval(() => {
        setTimeLeft((prev) => {
          if (prev <= 0.1) {
            clearInterval(interval);
            onExplode();
            // Start next sequence in 10 seconds
            setTimeout(startSimulationCycle, 10000);
            return 0;
          }
          return Number((prev - 0.1).toFixed(1));
        });

        // Defusal progression simulation
        setDefuseProgress((prevDefuse) => {
          if (!isDefusing) return 0;
          const increment = hasKit ? 2.5 : 1.25; // 5 seconds vs 10 seconds defusal
          const nextVal = prevDefuse + increment;
          if (nextVal >= 100) {
            setIsDefusing(false);
            setIsPlanted(false);
            onExplode(); // triggered finished simulation callback
            setTimeout(startSimulationCycle, 8000); // loop next
            return 0;
          }
          return nextVal;
        });
      }, 100);
    };

    startSimulationCycle();

    return () => {
      clearInterval(interval);
      clearTimeout(defuseTimeout);
    };
  }, [isDefusing, hasKit]);

  if (!config.bomb_timer || !isPlanted) return null;

  const progressPercentage = (timeLeft / 40.0) * 100;
  const isCritical = timeLeft < 10.0;

  return (
    <div className="bg-neutral-900 border border-neutral-800 p-4 rounded-lg relative overflow-hidden backdrop-blur-md">
      {/* Header */}
      <div className="flex items-center justify-between border-b border-neutral-800 pb-1.5 mb-3 font-mono text-[11px] text-neutral-400">
        <span className="flex items-center gap-1.5 font-bold text-neutral-200">
          <Skull size={12} className={isCritical ? 'text-red-500 animate-bounce' : 'text-purple-500'} />
          C4 PATLAMA DETEKTÖRÜ
        </span>
        <span className={`font-extrabold flex items-center gap-1 ${isCritical ? 'text-red-500 animate-pulse' : 'text-yellow-500'}`}>
          <AlertTriangle size={10} />
          {isCritical ? 'ACİL_DURUM' : 'BOMBA KURULDU'}
        </span>
      </div>

      <div className="space-y-3 font-mono">
        {/* Main large countdown clock */}
        <div className="text-center py-2 bg-neutral-950 rounded border border-neutral-800/50">
          <p className="text-[10px] text-neutral-500 tracking-wider">PATLAMAYA KALAN SÜRE</p>
          <p className={`text-3xl font-extrabold tracking-widest ${isCritical ? 'text-red-500 animate-pulse' : 'text-yellow-400'}`}>
            {timeLeft.toFixed(1)}s
          </p>
        </div>

        {/* Progress bar representing timeline */}
        <div className="w-full h-2 bg-neutral-950 rounded overflow-hidden border border-neutral-800">
          <div 
            className={`h-full transition-all duration-100 ${isCritical ? 'bg-red-600' : 'bg-purple-600'}`}
            style={{ width: `${progressPercentage}%` }}
          />
        </div>

        {/* Defusal overlay indicator */}
        {isDefusing && (
          <div className="bg-neutral-950/90 border border-blue-950/50 p-2 rounded text-[10px]">
            <div className="flex justify-between items-center text-blue-400 mb-1">
              <span className="font-bold flex items-center gap-1">
                <ShieldCheck size={11} className="animate-spin-slow" />
                BOMBA İMHA EDİLİYOR...
              </span>
              <span>{hasKit ? 'İmha Kiti Var (5sn)' : 'İmha Kiti Yok (10sn)'}</span>
            </div>
            
            <div className="w-full h-1.5 bg-neutral-900 rounded overflow-hidden">
              <div 
                className="h-full bg-blue-500 transition-all duration-100"
                style={{ width: `${defuseProgress}%` }}
              />
            </div>
          </div>
        )}
      </div>
    </div>
  );
}
