#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

// 2D Vector struct
typedef struct {
    float x;
    float y;
} ZFB_Vector2;

// Physics properties of an entity
typedef struct {
    ZFB_Vector2 position;      // Where the object is
    ZFB_Vector2 velocity;      // How fast it's moving
    ZFB_Vector2 acceleration;  // How much force is applied
    float mass;                // How heavy it is
    int affected_by_gravity;
} ZFB_PhysicsBody;

// Represents a game entity (e.g., Player, Wall, Object)
typedef struct {
    int id;                 // Unique identifier
    ZFB_PhysicsBody physics; // Physics properties
    int width, height;       // Size (for collision detection)
} ZFB_Entity;

#define DAMPING 0.98f
#define GRAVITY 9.81f
#define MAX_VELOCITY 500.0f

void apply_force(ZFB_Entity *entity, ZFB_Vector2 force);
void update_physics(ZFB_Entity *entity, float dt);
int check_collision(ZFB_Entity a, ZFB_Entity b);

#endif
