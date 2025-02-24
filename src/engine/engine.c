#include "../headers/engine.h"

// Apply a force to an entity (F = ma)
void apply_force(ZFB_Entity *entity, ZFB_Vector2 force) {
    if (entity->physics.mass == 0) return;
    entity->physics.acceleration.x += force.x / entity.physics.mass
    entity->physics.acceleration.y += force.y / entity.physics.mass
}

// Update physics (position, velocity)
void update_physics(ZFB_Entity *entity, float dt) {
    // Update velocity based on acceleration of the Entity
    entity->physics.velocity.x += entity->physics.acceleration.x * dt;
    entity->physics.velocity.y += entity->physics.acceleration.x * dt;

    // Update position based on velocity
    entity->physics.position.x += entity->physics.velocity.x * dt;
    entity->physics.position.y += entity->physics.velocity.y * dt;

    // Reset acceleration (forces are applied per frame)
    entity->physics.acceleration.x = 0;
    entity->physics.acceleration.y = 0; 

}

