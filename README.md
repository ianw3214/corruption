# Corruption

*Corruption* is a 2D top down open world RPG made for the UTGDDC Game Making Deathmatch.

***

## TODO

- [x] Optimize tilemap rendering (it is super slow right now)
- [x] Console to show info (use ImGui wrapper)
- [x] Basic profiling tools
- [x] Clean up profiling tools code (Proper-ish initialization)
- [ ] Visual profiling tools
- [x] Test other entities in the map
- [x] Sprite render order
- [x] Serialization of entities
- [ ] Implement serialization for all other components
- [ ] Figure out how to implement serialization for interactables
- [x] Full player movement animation
- [x] Map tile editing
- [ ] Export map (save to file)
- [x] NPCs (interactable component)
- [ ] Dialogue system
- [x] Basic Collision system
- [ ] Generalize collision system
- [ ] Fix camera jitter on player collisions
- [x] Collision debug draw
- [ ] Sprite render at offset from position
- [ ] Camera System Based on where player is looking
- [ ] More robust animation system
- [ ] Only allow one movement direction at a time (diagonal movement looks weird)
- [x] Health component
- [ ] Basic AI (AI component)
- [ ] Particle system
- [ ] Better text rendering
- [ ] Support rendering multiple fonts
- [x] Player attack (Shoot projectile)
- [ ] Remove projectiles after a while (can be just based off time)
- [ ] Shoot projectiles based on CENTER of sprite, not edge
- [ ] Don't render sprite if not on screen
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

- [ ] ImGui mouse capture
- [ ] mouse y is 'inverted'?
- [ ] Empty interactable component is added to manager because of serialization registration
