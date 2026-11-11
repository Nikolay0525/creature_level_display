# Creature Level Display (Kyubu port)

> This is a **port to Kyubu** of the original mod [`ToufouMaster/CreatureLevelDisplay`](https://github.com/ToufouMaster/CreatureLevelDisplay).
> Original author: **ToufouMaster**. This port is not an official release by the author.

**What it does:** Shows the level of the creature you look at.

**Note:** Do not run it together with the `level_rework` mod: both draw the level.

**Original:** <https://github.com/ToufouMaster/CreatureLevelDisplay>

**Release of the original this was ported from:** <https://github.com/ToufouMaster/CreatureLevelDisplay/releases/tag/v1>

**Side:** client (`sides` in `mod.toml`).

## Installation

Requirements: **Cube World Alpha** (the 2013-07-20 `Cube.exe`, SHA-256 `84a7a132...30bf`, unpatched) and **Kyubu** (the mod loader, <https://github.com/kyubumods/kyubu>; this mod needs `kyubu = "^1.1"`, see `mod.toml`).

1. Install Kyubu with its own installer (`install\install.cmd`). The loader is not part of this mod.
2. Put **`creature_level_display.zip`** (do not unpack it) into the **`Mods\`** folder next to `Cube.exe`. If `creature_level_display.zip` is already there, replace it.
3. Start the game. `kyubu\logs\latest.log` should have a line saying `creature_level_display` loaded.
4. To turn the mod off or remove it, take `creature_level_display.zip` out of `Mods\` (or switch it off in the mod manager: **Mods** on the title screen).

The old Cube World Mod Launcher is **not needed** for this port and is not compatible with it: do not put the original mod's DLL next to it.


Mod id: `creature_level_display`. Source: `Mods\creature_level_display\src\main.cpp`.
