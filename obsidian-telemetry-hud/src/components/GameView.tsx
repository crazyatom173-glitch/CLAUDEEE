import React, { useEffect, useRef, useState } from 'react';
import { Target, HackConfig } from '../types';
import { BONES } from '../data';
import { Eye, Shield, Target as CrosshairIcon, Crosshair } from 'lucide-react';

interface GameViewProps {
  targets: Target[];
  setTargets: React.Dispatch<React.SetStateAction<Target[]>>;
  config: HackConfig;
  crosshairPos: { x: number; y: number };
  setCrosshairPos: React.Dispatch<React.SetStateAction<{ x: number; y: number }>>;
  onAimbotTrigger: (targetName: string, bone: string) => void;
  onTriggerbotFire: (targetName: string) => void;
  isLiveLink?: boolean;
}

export default function GameView({
  targets,
  setTargets,
  config,
  crosshairPos,
  setCrosshairPos,
  onAimbotTrigger,
  onTriggerbotFire,
  isLiveLink = false
}: GameViewProps) {
  const containerRef = useRef<HTMLDivElement>(null);
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [hoveredTarget, setHoveredTarget] = useState<Target | null>(null);
  const [dimensions, setDimensions] = useState({ width: 800, height: 600 });
  const [flashIntensity, setFlashIntensity] = useState(0); // 0 to 1 for simulation

  // Handle ResizeObserver to get concrete dimensions of parent container
  useEffect(() => {
    if (!containerRef.current) return;
    const resizeObserver = new ResizeObserver((entries) => {
      for (let entry of entries) {
        const { width, height } = entry.contentRect;
        setDimensions({ width: width || 800, height: height || 500 });
      }
    });
    resizeObserver.observe(containerRef.current);
    return () => resizeObserver.disconnect();
  }, []);

  // Update canvas sizing
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    canvas.width = dimensions.width;
    canvas.height = dimensions.height;
  }, [dimensions]);

  // Handle flash effect interval simulation (flashes occasionally)
  useEffect(() => {
    const interval = setInterval(() => {
      // Simulate getting flashed
      setFlashIntensity(0.9);
      const fadeInterval = setInterval(() => {
        setFlashIntensity((prev) => {
          const maxAlphaLimit = config.noflash_enabled ? config.noflash_max_alpha / 100 : 1.0;
          const decay = 0.04;
          const next = prev - decay;
          if (next <= 0) {
            clearInterval(fadeInterval);
            return 0;
          }
          return Math.min(next, maxAlphaLimit);
        });
      }, 50);
    }, 18000); // gets flashed every 18 seconds

    return () => clearInterval(interval);
  }, [config.noflash_enabled, config.noflash_max_alpha]);

  // Game/Telemetry Simulation Loop
  useEffect(() => {
    if (isLiveLink) return;
    let animationFrameId: number;
    const updatePhysics = () => {
      setTargets((prevTargets) =>
        prevTargets.map((target) => {
          // Bounce on walls
          let nx = target.x + target.vx;
          let ny = target.y + target.vy;
          let nvx = target.vx;
          let nvy = target.vy;

          if (nx < 10 || nx > 90) {
            nvx = -nvx;
            nx = Math.max(10, Math.min(90, nx));
          }
          if (ny < 15 || ny > 85) {
            nvy = -nvy;
            ny = Math.max(15, Math.min(85, ny));
          }

          // Generate simulated skeleton bones based on target's calculated height
          const height = target.id === 3 ? 210 : (target.id === 4 ? 220 : 180);
          const w = height * 0.45;
          const pixelX = (nx / 100) * dimensions.width;
          const pixelY = (ny / 100) * dimensions.height;

          // Re-generate relative relative coordinates
          const bones = {
            [BONES.HEAD]: { x: pixelX, y: pixelY - height * 0.42 },
            [BONES.NECK]: { x: pixelX, y: pixelY - height * 0.32 },
            [BONES.CHEST]: { x: pixelX, y: pixelY - height * 0.20 },
            [BONES.STOMACH]: { x: pixelX, y: pixelY - height * 0.05 },
            [BONES.PELVIS]: { x: pixelX, y: pixelY + height * 0.08 },
            [BONES.LEFT_SHOULDER]: { x: pixelX - w * 0.35, y: pixelY - height * 0.25 },
            [BONES.LEFT_ELBOW]: { x: pixelX - w * 0.48, y: pixelY - height * 0.12 },
            [BONES.LEFT_HAND]: { x: pixelX - w * 0.52, y: pixelY },
            [BONES.RIGHT_SHOULDER]: { x: pixelX + w * 0.35, y: pixelY - height * 0.25 },
            [BONES.RIGHT_ELBOW]: { x: pixelX + w * 0.48, y: pixelY - height * 0.12 },
            [BONES.RIGHT_HAND]: { x: pixelX + w * 0.52, y: pixelY },
            [BONES.LEFT_KNEE]: { x: pixelX - w * 0.22, y: pixelY + height * 0.26 },
            [BONES.LEFT_FOOT]: { x: pixelX - w * 0.25, y: pixelY + height * 0.46 },
            [BONES.RIGHT_KNEE]: { x: pixelX + w * 0.22, y: pixelY + height * 0.26 },
            [BONES.RIGHT_FOOT]: { x: pixelX + w * 0.25, y: pixelY + height * 0.46 },
          };

          return {
            ...target,
            x: nx,
            y: ny,
            vx: nvx,
            vy: nvy,
            bones
          };
        })
      );
      animationFrameId = requestAnimationFrame(updatePhysics);
    };

    animationFrameId = requestAnimationFrame(updatePhysics);
    return () => cancelAnimationFrame(animationFrameId);
  }, [dimensions, setTargets, isLiveLink]);

  // Aimbot and triggerbot execution loop inside the simulated client view
  useEffect(() => {
    if (!config.aimbot_enabled && !config.triggerbot_enabled) return;

    const interval = setInterval(() => {
      // Find closest target within FOV
      let bestTarget: Target | null = null;
      let minDistanceToCrosshair = Infinity;

      targets.forEach((target) => {
        // Skip team if we shouldn't aim at team
        if (target.team === 'Counter-Terrorist' && !config.aimbot_target_team) return;
        if (config.aimbot_visible_check && !target.spotted) return;

        // Get target pixel coords
        const headBone = target.bones[BONES.HEAD];
        if (!headBone) return;

        const dx = headBone.x - crosshairPos.x;
        const dy = headBone.y - crosshairPos.y;
        const dist = Math.sqrt(dx * dx + dy * dy);

        // Check FOV limit (simulated in pixels)
        if (dist < config.aimbot_fov && dist < minDistanceToCrosshair) {
          minDistanceToCrosshair = dist;
          bestTarget = target;
        }
      });

      // Execute Aimbot if target found
      if (config.aimbot_enabled && bestTarget) {
        const target: Target = bestTarget;
        const boneKey = config.aimbot_bone === 'HEAD' ? BONES.HEAD : (config.aimbot_bone === 'CHEST' ? BONES.CHEST : BONES.PELVIS);
        const aimBone = target.bones[boneKey] || target.bones[BONES.HEAD];

        if (aimBone) {
          // Linear interpolation based on config.aimbot_smooth
          const speed = 1 / Math.max(1, config.aimbot_smooth);
          const nextX = crosshairPos.x + (aimBone.x - crosshairPos.x) * speed;
          const nextY = crosshairPos.y + (aimBone.y - crosshairPos.y) * speed;

          setCrosshairPos({ x: nextX, y: nextY });
          onAimbotTrigger(target.name, config.aimbot_bone);
        }
      }

      // Execute Triggerbot if crosshair is directly overlapping any target head/chest bone
      if (config.triggerbot_enabled) {
        targets.forEach((target) => {
          if (target.team === 'Counter-Terrorist' && !config.aimbot_target_team) return;
          
          const head = target.bones[BONES.HEAD];
          const chest = target.bones[BONES.CHEST];

          if (head) {
            const hDist = Math.sqrt(Math.pow(head.x - crosshairPos.x, 2) + Math.pow(head.y - crosshairPos.y, 2));
            if (hDist < 12) { // very close threshold
              setTimeout(() => {
                if (Math.random() * 100 <= config.triggerbot_hitchance) {
                  onTriggerbotFire(target.name);
                }
              }, config.triggerbot_delay);
            }
          }
          if (chest) {
            const cDist = Math.sqrt(Math.pow(chest.x - crosshairPos.x, 2) + Math.pow(chest.y - crosshairPos.y, 2));
            if (cDist < 18) {
              setTimeout(() => {
                if (Math.random() * 100 <= config.triggerbot_hitchance) {
                  onTriggerbotFire(target.name);
                }
              }, config.triggerbot_delay);
            }
          }
        });
      }
    }, 30);

    return () => clearInterval(interval);
  }, [config, targets, crosshairPos, setCrosshairPos, onAimbotTrigger, onTriggerbotFire]);

  // Handle manual canvas cursor targeting mouse move
  const handleMouseMove = (e: React.MouseEvent<HTMLDivElement>) => {
    if (!containerRef.current) return;
    const rect = containerRef.current.getBoundingClientRect();
    const mx = e.clientX - rect.left;
    const my = e.clientY - rect.top;
    
    // Smooth transition unless aimbot is currently locking onto target
    setCrosshairPos({ x: mx, y: my });

    // Check for target hovering
    let hovered: Target | null = null;
    targets.forEach((target) => {
      const head = target.bones[BONES.HEAD];
      if (head) {
        const height = target.id === 3 ? 210 : (target.id === 4 ? 220 : 180);
        const w = height * 0.45;
        // Bounding box approximation
        const minX = head.x - w / 2;
        const maxX = head.x + w / 2;
        const minY = head.y;
        const maxY = head.y + height;

        if (mx >= minX && mx <= maxX && my >= minY && my <= maxY) {
          hovered = target;
        }
      }
    });
    setHoveredTarget(hovered);
  };

  // Draw HUD rendering over simulated environment on actual canvas for performance
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    // Clear background
    ctx.clearRect(0, 0, dimensions.width, dimensions.height);

    // Draw grid map representing the 3D map bounds
    ctx.strokeStyle = 'rgba(120, 20, 20, 0.05)';
    ctx.lineWidth = 1;
    const gridSize = 40;
    for (let i = 0; i < dimensions.width; i += gridSize) {
      ctx.beginPath();
      ctx.moveTo(i, 0);
      ctx.lineTo(i, dimensions.height);
      ctx.stroke();
    }
    for (let j = 0; j < dimensions.height; j += gridSize) {
      ctx.beginPath();
      ctx.moveTo(0, j);
      ctx.lineTo(dimensions.width, j);
      ctx.stroke();
    }

    // Render each target
    targets.forEach((target) => {
      const head = target.bones[BONES.HEAD];
      const feet = target.bones[BONES.RIGHT_FOOT] || target.bones[BONES.LEFT_FOOT];
      if (!head || !feet) return;

      const height = target.id === 3 ? 210 : (target.id === 4 ? 220 : 180);
      const width = height * 0.45;
      const x = head.x - width / 2;
      const y = head.y;

      // Check if target is teammate vs enemy
      const isTeammate = target.team === 'Counter-Terrorist';
      
      // Determine core color based on vibe theme
      let primaryColor = 'rgb(239, 68, 68)'; // Tailwind red-500
      let secondColor = 'rgba(239, 68, 68, 0.05)';
      if (isTeammate) {
        primaryColor = 'rgb(59, 130, 246)'; // blue-500
        secondColor = 'rgba(59, 130, 246, 0.05)';
      } else if (config.vibe_theme === 'toxin') {
        primaryColor = 'rgb(34, 197, 94)'; // green-500
        secondColor = 'rgba(34, 197, 94, 0.05)';
      } else if (config.vibe_theme === 'obsidian') {
        primaryColor = target.spotted ? 'rgb(249, 115, 22)' : 'rgb(168, 85, 247)'; // Orange vs Purple
        secondColor = target.spotted ? 'rgba(249, 115, 22, 0.05)' : 'rgba(168, 85, 247, 0.05)';
      } else if (config.vibe_theme === 'cyberpunk') {
        primaryColor = 'rgb(236, 72, 153)'; // pink-500
        secondColor = 'rgba(236, 72, 153, 0.05)';
      }

      // If glow is enabled, render ambient box background shadow
      if (config.esp_glow) {
        ctx.shadowBlur = 10;
        ctx.shadowColor = primaryColor;
      } else {
        ctx.shadowBlur = 0;
      }

      // Draw ESP box
      if (config.esp_enabled && config.esp_box) {
        ctx.lineWidth = 1.0;
        ctx.strokeStyle = primaryColor;
        ctx.fillStyle = secondColor;

        if (config.esp_box_type === 'corner') {
          const cSize = Math.min(width, height) * 0.20;
          // Top Left Corner
          ctx.beginPath();
          ctx.moveTo(x + cSize, y); ctx.lineTo(x, y); ctx.lineTo(x, y + cSize);
          ctx.stroke();

          // Top Right Corner
          ctx.beginPath();
          ctx.moveTo(x + width - cSize, y); ctx.lineTo(x + width, y); ctx.lineTo(x + width, y + cSize);
          ctx.stroke();

          // Bottom Left Corner
          ctx.beginPath();
          ctx.moveTo(x, y + height - cSize); ctx.lineTo(x, y + height); ctx.lineTo(x + cSize, y + height);
          ctx.stroke();

          // Bottom Right Corner
          ctx.beginPath();
          ctx.moveTo(x + width - cSize, y + height); ctx.lineTo(x + width, y + height); ctx.lineTo(x + width, y + height - cSize);
          ctx.stroke();
          
          // Fill transparency slightly
          ctx.fillRect(x, y, width, height);
        } else if (config.esp_box_type === '3d') {
          // Render a simple pseudo-3D rotating box
          const offset = 8;
          ctx.beginPath();
          ctx.rect(x, y, width, height);
          ctx.stroke();
          ctx.beginPath();
          ctx.rect(x + offset, y - offset, width, height);
          ctx.stroke();
          // Connect corners
          ctx.beginPath();
          ctx.moveTo(x, y); ctx.lineTo(x + offset, y - offset);
          ctx.moveTo(x + width, y); ctx.lineTo(x + width + offset, y - offset);
          ctx.moveTo(x, y + height); ctx.lineTo(x + offset, y + height - offset);
          ctx.moveTo(x + width, y + height); ctx.lineTo(x + width + offset, y + height - offset);
          ctx.stroke();
        } else {
          // Full solid box
          ctx.beginPath();
          ctx.rect(x, y, width, height);
          ctx.stroke();
          ctx.fillRect(x, y, width, height);
        }
      }

      // Reset shadows for details
      ctx.shadowBlur = 0;

      // Draw skeleton linkages
      if (config.esp_enabled && config.esp_skeleton) {
        ctx.lineWidth = 1;
        ctx.strokeStyle = 'rgba(255, 255, 255, 0.7)';
        
        // Define links
        const links = [
          [BONES.HEAD, BONES.NECK],
          [BONES.NECK, BONES.CHEST],
          [BONES.CHEST, BONES.STOMACH],
          [BONES.STOMACH, BONES.PELVIS],
          
          [BONES.CHEST, BONES.LEFT_SHOULDER],
          [BONES.LEFT_SHOULDER, BONES.LEFT_ELBOW],
          [BONES.LEFT_ELBOW, BONES.LEFT_HAND],

          [BONES.CHEST, BONES.RIGHT_SHOULDER],
          [BONES.RIGHT_SHOULDER, BONES.RIGHT_ELBOW],
          [BONES.RIGHT_ELBOW, BONES.RIGHT_HAND],

          [BONES.PELVIS, BONES.LEFT_KNEE],
          [BONES.LEFT_KNEE, BONES.LEFT_FOOT],

          [BONES.PELVIS, BONES.RIGHT_KNEE],
          [BONES.RIGHT_KNEE, BONES.RIGHT_FOOT],
        ];

        ctx.beginPath();
        links.forEach(([b1, b2]) => {
          const pt1 = target.bones[b1];
          const pt2 = target.bones[b2];
          if (pt1 && pt2) {
            ctx.moveTo(pt1.x, pt1.y);
            ctx.lineTo(pt2.x, pt2.y);
          }
        });
        ctx.stroke();
      }

      // Draw head dot
      if (config.esp_enabled && config.esp_head_dot) {
        ctx.fillStyle = primaryColor;
        ctx.beginPath();
        ctx.arc(head.x, head.y, 3, 0, Math.PI * 2);
        ctx.fill();
        ctx.strokeStyle = '#ffffff';
        ctx.lineWidth = 0.8;
        ctx.stroke();
      }

      // Draw health and armor bars
      if (config.esp_enabled && config.esp_health_bar) {
        const barSize = 2.5;
        const spacing = 6;

        if (config.esp_health_bar_style === 'horizontal') {
          const hWidth = width * (target.health / 100);
          
          // Horizontal Health Bar under the box
          ctx.fillStyle = 'rgba(0, 0, 0, 0.6)';
          ctx.fillRect(x, y + height + spacing, width, barSize);
          
          // Dynamic health color
          ctx.fillStyle = target.health > 50 ? '#22c55e' : (target.health > 20 ? '#eab308' : '#ef4444');
          ctx.fillRect(x, y + height + spacing, hWidth, barSize);

          // Horizontal Armor Bar below health bar
          if (target.armor > 0) {
            const aWidth = width * (target.armor / 100);
            ctx.fillStyle = 'rgba(0, 0, 0, 0.6)';
            ctx.fillRect(x, y + height + spacing + barSize + 2, width, barSize);
            ctx.fillStyle = '#3b82f6'; // Armor Blue
            ctx.fillRect(x, y + height + spacing + barSize + 2, aWidth, barSize);
          }
        } else {
          const hHeight = height * (target.health / 100);

          // Vertical Health Bar left of the box
          ctx.fillStyle = 'rgba(0, 0, 0, 0.6)';
          ctx.fillRect(x - barSize - spacing, y, barSize, height);
          
          // Dynamic health color
          ctx.fillStyle = target.health > 50 ? '#22c55e' : (target.health > 20 ? '#eab308' : '#ef4444');
          ctx.fillRect(x - barSize - spacing, y + (height - hHeight), barSize, hHeight);

          // Armor Bar on the right side if present
          if (target.armor > 0) {
            const aHeight = height * (target.armor / 100);
            ctx.fillStyle = 'rgba(0, 0, 0, 0.6)';
            ctx.fillRect(x + width + spacing, y, barSize, height);
            ctx.fillStyle = '#3b82f6'; // Armor Blue
            ctx.fillRect(x + width + spacing, y + (height - aHeight), barSize, aHeight);
          }
        }
      }

      // Render Text indicators
      if (config.esp_enabled) {
        ctx.fillStyle = '#ffffff';
        ctx.font = 'bold 10px "JetBrains Mono", monospace';
        ctx.textAlign = 'left';

        let currentTextY = y - 6;

        // Display Name & Entity Index ID
        if (config.esp_name) {
          ctx.fillText(`[${target.id}] ${target.name}`, x, currentTextY);
          currentTextY -= 12;
        }

        // Display distance below the box
        let bottomTextY = y + height + 12;
        if (config.esp_weapon) {
          ctx.fillStyle = 'rgba(255, 255, 255, 0.8)';
          ctx.fillText(target.weapon, x, bottomTextY);
          bottomTextY += 12;
        }

        if (config.esp_distance) {
          ctx.fillStyle = 'rgba(147, 197, 253, 0.9)'; // blue-300
          ctx.fillText(`${target.distance.toFixed(1)}m`, x, bottomTextY);
        }
      }
    });

    // Render Aimbot FOV limit ring
    if (config.aimbot_enabled) {
      ctx.strokeStyle = 'rgba(239, 68, 68, 0.25)';
      ctx.lineWidth = 1;
      ctx.setLineDash([4, 4]);
      ctx.beginPath();
      ctx.arc(crosshairPos.x, crosshairPos.y, config.aimbot_fov, 0, Math.PI * 2);
      ctx.stroke();
      ctx.setLineDash([]); // clear dash
    }

    // Render local user flashbang simulation screen cover
    if (flashIntensity > 0) {
      ctx.fillStyle = `rgba(255, 255, 255, ${flashIntensity})`;
      ctx.fillRect(0, 0, dimensions.width, dimensions.height);
      
      // Draw GOTHIC lettering overlay warning during visual stun
      ctx.fillStyle = 'rgba(120, 20, 20, 0.8)';
      ctx.font = 'bold 24px "Space Grotesk", sans-serif';
      ctx.textAlign = 'center';
      ctx.fillText("FLASHLENDİN (KÖR EDİLDİN)", dimensions.width / 2, dimensions.height / 2);
    }

  }, [targets, config, crosshairPos, dimensions]);

  return (
    <div 
      id="game-view-container"
      ref={containerRef}
      onMouseMove={handleMouseMove}
      className="relative w-full h-[540px] bg-neutral-950 border border-neutral-800 rounded-lg overflow-hidden cursor-crosshair select-none"
    >
      {/* Background Ambience Layer */}
      <div className="absolute inset-0 bg-[radial-gradient(ellipse_at_center,_var(--tw-gradient-stops))] from-neutral-900/30 via-neutral-950/90 to-neutral-950 pointer-events-none" />
      
      {/* Simulation Interactive canvas */}
      <canvas ref={canvasRef} className="absolute inset-0 w-full h-full block" />

      {/* Target Status Floating Card on Hover */}
      {hoveredTarget && (
        <div 
          className="absolute z-30 p-3 bg-neutral-900/95 border border-neutral-700/80 rounded shadow-xl pointer-events-none text-xs text-neutral-300 w-52 font-mono backdrop-blur-md"
          style={{ 
            left: `${Math.min((hoveredTarget.x / 100) * dimensions.width + 20, dimensions.width - 230)}px`,
            top: `${Math.min((hoveredTarget.y / 100) * dimensions.height, dimensions.height - 180)}px`
          }}
        >
          <div className="flex justify-between border-b border-neutral-800 pb-1.5 mb-1.5">
            <span className="text-neutral-400 font-sans font-bold">HEDEF VERİLERİ</span>
            <span className={hoveredTarget.team === 'Counter-Terrorist' ? 'text-blue-400' : 'text-red-400'}>
              {hoveredTarget.team === 'Counter-Terrorist' ? 'CT' : 'T'}
            </span>
          </div>
          <div className="space-y-1">
            <p className="flex justify-between">
              <span>İsim:</span> <span className="text-white font-bold">{hoveredTarget.name}</span>
            </p>
            <p className="flex justify-between">
              <span>Sağlık:</span> 
              <span className={`font-bold ${hoveredTarget.health > 50 ? 'text-green-400' : 'text-red-400'}`}>
                {hoveredTarget.health}%
              </span>
            </p>
            <p className="flex justify-between">
              <span>Zırh:</span> <span className="text-blue-400 font-bold">{hoveredTarget.armor}%</span>
            </p>
            <p className="flex justify-between">
              <span>Silah:</span> <span className="text-yellow-400 font-semibold">{hoveredTarget.weapon}</span>
            </p>
            <p className="flex justify-between">
              <span>Mesafe:</span> <span className="text-neutral-200">{hoveredTarget.distance.toFixed(1)}m</span>
            </p>
            <p className="flex justify-between">
              <span>Görünürlük:</span> 
              <span className={hoveredTarget.spotted ? 'text-orange-400 font-bold' : 'text-neutral-500'}>
                {hoveredTarget.spotted ? 'GÖRÜNÜR' : 'PASİF'}
              </span>
            </p>
          </div>
        </div>
      )}

      {/* Target Crosshair Centered Reticle */}
      <div 
        className="absolute w-6 h-6 -translate-x-1/2 -translate-y-1/2 pointer-events-none transition-all duration-75 mix-blend-difference"
        style={{ left: `${crosshairPos.x}px`, top: `${crosshairPos.y}px` }}
      >
        <div className="absolute top-0 left-1/2 -translate-x-1/2 w-0.5 h-1.5 bg-cyan-400" />
        <div className="absolute bottom-0 left-1/2 -translate-x-1/2 w-0.5 h-1.5 bg-cyan-400" />
        <div className="absolute left-0 top-1/2 -translate-y-1/2 w-1.5 h-0.5 bg-cyan-400" />
        <div className="absolute right-0 top-1/2 -translate-y-1/2 w-1.5 h-0.5 bg-cyan-400" />
        <div className="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-1 h-1 bg-cyan-400 rounded-full" />
      </div>

      {/* Outer Corners Design to emphasize high-tech HUD aesthetic */}
      <div className="absolute top-3 left-3 w-4 h-4 border-t-2 border-l-2 border-neutral-800 pointer-events-none" />
      <div className="absolute top-3 right-3 w-4 h-4 border-t-2 border-r-2 border-neutral-800 pointer-events-none" />
      <div className="absolute bottom-3 left-3 w-4 h-4 border-b-2 border-l-2 border-neutral-800 pointer-events-none" />
      <div className="absolute bottom-3 right-3 w-4 h-4 border-b-2 border-r-2 border-neutral-800 pointer-events-none" />

      {/* Left Bottom Status overlay for cyber gothic telemetry */}
      <div className="absolute bottom-4 left-4 p-2 bg-black/70 backdrop-blur-sm border border-neutral-800 rounded font-mono text-[9px] text-neutral-400 space-y-0.5 pointer-events-none">
        <div className="flex items-center gap-1.5 text-neutral-200">
          <Eye size={10} className="text-purple-400 animate-pulse" />
          <span className="font-bold">TELEMETRİ PANELİ</span>
        </div>
        <p>MATRİS: 1920x1080 (SANAL)</p>
        <p>KOORDİNAT: X={crosshairPos.x.toFixed(1)}, Y={crosshairPos.y.toFixed(1)}</p>
        <p>TETİKTEKİ DÜŞMAN SAYISI: {targets.filter(t => t.team === 'Terrorist' && t.spotted).length}</p>
      </div>
    </div>
  );
}
