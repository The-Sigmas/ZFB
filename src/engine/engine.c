#include "../headers/engine.h"

// Apply a force to an entity (F = ma)
void apply_force(ZFB_Entity *entity, ZFB_Vector2 force) {
    if (entity->physics.mass == 0) return;
    entity->physics.acceleration.x += force.x / entity->physics.mass;
    entity->physics.acceleration.y += force.y / entity->physics.mass;
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

// Check if two entities are colliding (simple box collision)
int check_collision(ZFB_Entity *a, ZFB_Entity *b) {
    float a_left = a->physics.position.x;
    float a_right = a->physics.position.x + a->width;
    float a_top = a->physics.position.y;
    float a_bottom = a->physics.position.y + a->height;
    
    float b_left = b->physics.position.x;
    float b_right = b->physics.position.x + b->width;
    float b_top = b->physics.position.y;
    float b_bottom = b->physics.position.y + b->height;
    
     return (a_right > b_left && a_left < b_right &&
            a_bottom > b_top && a_top < b_bottom);
}
