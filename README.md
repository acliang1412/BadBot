# BadBot

A third-person drone combat game built in Unreal Engine 5 using Blueprint. The player pilots a drone and battles waves of enemy bots, culminating in a boss encounter. Enemies fire homing blaster beams and must be destroyed before they overwhelm the player.

---

## Table of Contents

- [Gameplay Overview](#gameplay-overview)
- [Core Systems](#core-systems)
- [Blueprint Architecture](#blueprint-architecture)
- [Key Assets](#key-assets)
- [Project Structure](#project-structure)
- [Setup](#setup)
- [Known Issues & Design Notes](#known-issues--design-notes)

---

## Gameplay Overview

- The player controls `BP_BotPawn`, a drone with dual-rifle blaster weapons and free-floating movement.
- Enemy drones (`BP_Bot`) spawn via `BP_BotSpawner` when the player enters a trigger volume.
- Enemies track the player, rotate to face them, and fire `BP_EnemyBlasterBeam` projectiles at a set interval.
- A boss variant (`BP_BotBoss`) inherits from `BP_Bot` with elevated stats and behaviour.
- Collectible `BP_SmallSphere` pickups orbit in the level; grabbing them increments the player's score via `BP_BadBotGameMode`.
- Both the player and enemies implement `BPI_Damageable`, giving every actor a standard `GetHurt(Damage)` entry point.

---

## Core Systems

### Damage System
All damageable actors implement the `BPI_Damageable` interface. Any actor that wants to deal damage calls `GetHurt` on its target through the interface. This decouples projectiles from specific actor types.

### Blaster Beam Projectiles
Projectiles use a parent/child class hierarchy:

| Class | Role |
|---|---|
| `BP_BlasterBeam` | Base class. Owns `BeamMesh` (StaticMesh) + `ProjectileMovement`. On hit calls `HandleOnHit`, which damages the hit actor and destroys itself. |
| `BP_PlayerBlasterBeam` | Child of `BP_BlasterBeam`. Fired by the player pawn. |
| `BP_EnemyBlasterBeam` | Child of `BP_BlasterBeam`. Fired by enemy bots. Overrides `HandleOnHit` to skip damage when the hit actor is the bot that owns the beam (prevents self-collision on spawn). |

### Enemy AI
`BP_Bot` uses a tick-driven state machine rather than a Behavior Tree:
- `StoreTargetPawn` — caches a reference to the player pawn.
- `FindTargetLocation` — returns the rotation needed to face the target.
- `InterpRotation(DeltaTime, InterpSpeed)` — smoothly rotates the drone toward the target each tick.
- `FireBlasterBeam` (custom event) — triggered by a timer; calls `SpawnBlasterBeam` for the left and right rifle sockets.

### Spawning
`BP_BotSpawner` holds an array of `TargetPoint` references and a `BotClass` variable (defaults to `BP_Bot`, swappable to `BP_BotBoss` in the editor). On player overlap it spawns bots at each target point once, tracked by a `HasSpawnBots` guard.

### Score
`BP_BadBotGameMode` owns a single `PlayerScore` integer. Any actor can retrieve the GameMode and call `IncrementScore` to add to it (triggered by sphere overlap and enemy destruction).

### Small Sphere Pickups
`BP_SmallSphere` orbits using per-tick `UpdateLocation` and `UpdateRotation` functions driven by `Speed`, `Delta Rotation`, and `RotationScaleFactor` variables. On player overlap it plays a `WinkOut` timeline (scale-to-zero), increments score, then destroys itself.

---

## Blueprint Architecture

```flow
[BP_BotSpawner] overlap -> [BP_Bot spawns]
[BP_Bot] tick -> [InterpRotation]
[BP_Bot] timer -> [SpawnBlasterBeam]
[SpawnBlasterBeam] spawn -> [BP_EnemyBlasterBeam]
[BP_EnemyBlasterBeam] hit -> [HandleOnHit override]
[HandleOnHit override] owner check -> [Skip if self]
[HandleOnHit override] not owner -> [Parent HandleOnHit]
[Parent HandleOnHit] damage -> [BPI_Damageable GetHurt]
[BPI_Damageable GetHurt] lethal -> [BlowUp]
[BlowUp] -> [IncrementScore]
