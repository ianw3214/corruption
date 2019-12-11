# Corruption

*Corruption* is a 2D top down open world RPG made for the UTGDDC Game Making Deathmatch.

***

## CORE

- [x] Optimize tilemap rendering (it is super slow right now)
- [x] Console to show info (use ImGui wrapper)
- [x] Basic profiling tools
- [x] Clean up profiling tools code (Proper-ish initialization)
- [ ] Visual profiling tools
- [x] Test other entities in the map
- [x] Sprite render order
- [x] Basic Collision system
- [x] Generalize collision system
- [ ] Better text rendering
- [ ] Support rendering multiple fonts
- [ ] Particle system
- [ ] Don't render sprite if not on screen
- [ ] More robust animation system
- [x] Collision debug draw

## SERIALIZATION

- [x] Serialization of entities
- [x] Implement collision component serialization
- [ ] Implement health component serialization
- [ ] Figure out how to implement serialization for interactables
- [ ] Export map (save to file)

## EDITOR

- [x] Map tile editing
- [ ] Display entity info in editor

## GAMEPLAY

- [x] Full player movement animation
- [x] NPCs (interactable component)
- [ ] Dialogue system
- [ ] Sprite render at offset from position
- [ ] Camera System Based on where player is looking
- [ ] Only allow one movement direction at a time (diagonal movement looks weird)
- [x] Health component
- [ ] Basic AI (AI component)
- [x] Player attack (Shoot projectile)
- [ ] Remove projectiles after a while (can be just based off time)
- [ ] Shoot projectiles based on CENTER of sprite, not edge
- [ ] Game mode service to make the game globally accessibly
- [ ] Collision masks (Ignore certain collisions like projectile-projectile collisions)

***

## ART

- [x] Player animation outlines
- [ ] Player concept
- [ ] Player sprites
- [ ] World concept
- [ ] Basic tilemap

***

## OTHER IMPORTANT SHIT

- [ ] Main story design

***

## BUGS

- [ ] Fix camera jitter on player collisions
- [ ] ImGui mouse capture
- [ ] mouse y is 'inverted'?
- [ ] Empty interactable component is added to manager because of serialization registration
