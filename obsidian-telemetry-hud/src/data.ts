import { Target } from './types';

// Standard Bones
export const BONES = {
  HEAD: 6,
  NECK: 5,
  CHEST: 4,
  STOMACH: 3,
  PELVIS: 0,
  LEFT_SHOULDER: 13,
  RIGHT_SHOULDER: 9,
  LEFT_ELBOW: 14,
  RIGHT_ELBOW: 10,
  LEFT_HAND: 15,
  RIGHT_HAND: 11,
  LEFT_KNEE: 23,
  RIGHT_KNEE: 19,
  LEFT_FOOT: 24,
  RIGHT_FOOT: 20,
};

// Generates skeleton bones relative to the target's center bounding box (x, y)
export function generateSkeletonBones(x: number, y: number, height: number): { [key: number]: { x: number; y: number } } {
  const w = height * 0.45;
  return {
    [BONES.HEAD]: { x: x, y: y - height * 0.42 },
    [BONES.NECK]: { x: x, y: y - height * 0.32 },
    [BONES.CHEST]: { x: x, y: y - height * 0.20 },
    [BONES.STOMACH]: { x: x, y: y - height * 0.05 },
    [BONES.PELVIS]: { x: x, y: y + height * 0.08 },
    
    [BONES.LEFT_SHOULDER]: { x: x - w * 0.35, y: y - height * 0.25 },
    [BONES.LEFT_ELBOW]: { x: x - w * 0.48, y: y - height * 0.12 },
    [BONES.LEFT_HAND]: { x: x - w * 0.52, y: y },

    [BONES.RIGHT_SHOULDER]: { x: x + w * 0.35, y: y - height * 0.25 },
    [BONES.RIGHT_ELBOW]: { x: x + w * 0.48, y: y - height * 0.12 },
    [BONES.RIGHT_HAND]: { x: x + w * 0.52, y: y },

    [BONES.LEFT_KNEE]: { x: x - w * 0.22, y: y + height * 0.26 },
    [BONES.LEFT_FOOT]: { x: x - w * 0.25, y: y + height * 0.46 },

    [BONES.RIGHT_KNEE]: { x: x + w * 0.22, y: y + height * 0.26 },
    [BONES.RIGHT_FOOT]: { x: x + w * 0.25, y: y + height * 0.46 },
  };
}

export const INITIAL_TARGETS: Target[] = [
  {
    id: 1,
    name: "Viper_0x",
    x: 35,
    y: 40,
    vx: 0.15,
    vy: -0.05,
    health: 84,
    armor: 100,
    team: 'Terrorist',
    spotted: true,
    weapon: "AK-47",
    distance: 24.3,
    bones: generateSkeletonBones(35, 40, 180)
  },
  {
    id: 2,
    name: "Wraith_CS",
    x: 65,
    y: 50,
    vx: -0.1,
    vy: 0.12,
    health: 42,
    armor: 50,
    team: 'Terrorist',
    spotted: false,
    weapon: "Glock-18",
    distance: 45.1,
    bones: generateSkeletonBones(65, 50, 150)
  },
  {
    id: 3,
    name: "Spectre",
    x: 50,
    y: 35,
    vx: 0.0,
    vy: 0.05,
    health: 100,
    armor: 100,
    team: 'Counter-Terrorist',
    spotted: true,
    weapon: "M4A1-S",
    distance: 18.7,
    bones: generateSkeletonBones(50, 35, 210)
  },
  {
    id: 4,
    name: "Necromancer",
    x: 20,
    y: 60,
    vx: 0.2,
    vy: -0.1,
    health: 12,
    armor: 0,
    team: 'Terrorist',
    spotted: true,
    weapon: "Desert Eagle",
    distance: 12.4,
    bones: generateSkeletonBones(20, 60, 220)
  },
  {
    id: 5,
    name: "Shadow_Walk",
    x: 80,
    y: 30,
    vx: -0.08,
    vy: -0.05,
    health: 95,
    armor: 90,
    team: 'Counter-Terrorist',
    spotted: false,
    weapon: "USP-S",
    distance: 36.2,
    bones: generateSkeletonBones(80, 30, 160)
  }
];

export const WEAPON_INFO: { [key: string]: { damage: number; fireRate: number; recoil: number; magSize: number } } = {
  "AK-47": { damage: 36, fireRate: 600, recoil: 8, magSize: 30 },
  "M4A1-S": { damage: 38, fireRate: 666, recoil: 5, magSize: 20 },
  "AWP": { damage: 115, fireRate: 41, recoil: 15, magSize: 10 },
  "Desert Eagle": { damage: 53, fireRate: 267, recoil: 12, magSize: 7 },
  "Glock-18": { damage: 28, fireRate: 400, recoil: 3, magSize: 20 },
  "USP-S": { damage: 35, fireRate: 352, recoil: 2, magSize: 12 },
};

export const GOTHIC_SOUNDS = {
  click: "data:audio/wav;base64,UklGRiQAAABXQVZFZm10IBAAAAABAAEARKwAAIhYAQACABAAZGF0YQAAAAA=", // Tiny silent placeholder
};
