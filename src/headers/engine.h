#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

// This stores the physics properties of an entity
typedef struct {
    float x;
    float y;
} ZFB_Vector2;

// This stores the physics properties of an entity
typedef struct {
    ZFB_Vector2 position; // Where the object is
    ZFB_Vector2 velocity; // How fast it's moving
    ZFB_Vector2 acceleration; // How much force is applied
    float mass; // How heavy it is
} ZFB_PhysicsBody; 

// This represents a game entity (Player or Wall)
typedef struct {
    int id; // Unique identifier
    ZFB_PhysicsBody physics; // Physics properties
    int width, height; // Size (for collision detection)
} ZFB_Entity;

// Function declarations 
void apply_force(ZFB_Entity *entity, ZFB_Vector2 force);
void update_physics(ZFB_Entity *entity, float dt);
int check_collision(ZFB_Entity a, ZFB_Entity b);

#endif

