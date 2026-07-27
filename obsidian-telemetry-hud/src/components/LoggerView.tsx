import React from 'react';
import { TelemetryLog } from '../types';
import { Skull, ShieldAlert, Terminal, CheckCircle2, RefreshCw } from 'lucide-react';

interface LoggerViewProps {
  logs: TelemetryLog[];
  onClear: () => void;
}

export default function LoggerView({ logs, onClear }: LoggerViewProps) {
  // Renders real-time telemetry events inside a custom stylized container
  return (
    <div className="bg-neutral-900 border border-neutral-800 rounded-lg overflow-hidden flex flex-col h-44 backdrop-blur-md">
      {/* Title bar */}
      <div className="bg-neutral-950 px-4 py-2 border-b border-neutral-800 flex items-center justify-between font-mono text-[11px]">
        <div className="flex items-center gap-1.5 font-bold text-neutral-300">
          <Terminal size={12} className="text-purple-500 animate-pulse" />
          SİSTEM TELEMETRİ GÜNLÜĞÜ [GÜVENLİ_BAĞLANTI]
        </div>
        <button
          onClick={onClear}
          className="text-[9px] text-neutral-500 hover:text-purple-400 font-bold uppercase transition-colors"
        >
          LOGLARI TEMİZLE
        </button>
      </div>

      {/* Log Feed */}
      <div className="flex-1 p-3 overflow-y-auto space-y-1.5 bg-neutral-950/60 font-mono text-[10px] select-text">
        {logs.length === 0 ? (
          <div className="h-full flex items-center justify-center text-neutral-600 gap-1.5">
            <RefreshCw size={12} className="animate-spin text-neutral-700" />
            SİSTEM TELEMETRİ VERİ AKIŞI BEKLENİYOR...
          </div>
        ) : (
          logs.map((log) => {
            let icon = <Terminal size={11} className="text-neutral-400" />;
            let textClass = 'text-neutral-300';
            let label = '[BİLGİ]';

            if (log.type === 'success') {
              icon = <CheckCircle2 size={11} className="text-green-500" />;
              textClass = 'text-green-400';
              label = '[TAMAM]';
            } else if (log.type === 'warn') {
              icon = <ShieldAlert size={11} className="text-yellow-500" />;
              textClass = 'text-yellow-400';
              label = '[UYARI]';
            } else if (log.type === 'error' || log.type === 'critical') {
              icon = <Skull size={11} className="text-red-500" />;
              textClass = 'text-red-400 font-bold';
              label = '[KRİT]';
            }

            return (
              <div key={log.id} className="flex items-start gap-2 hover:bg-neutral-900/40 p-1 rounded transition-colors">
                <span className="text-neutral-600 select-none">[{log.timestamp}]</span>
                <span className="font-bold text-purple-500 select-none">{label}</span>
                <span className={`flex-1 ${textClass}`}>{log.message}</span>
                {icon}
              </div>
            );
          })
        )}
      </div>
    </div>
  );
}
