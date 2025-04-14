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

typedef struct
{
    float DAMPING;
    float GRAVITY;
    float MAX_VELOCITY;
} ZFB_WorldPhysicsProperties;

extern ZFB_WorldPhysicsProperties wpp =
{
    .DAMPING = 0.98f,
    .GRAVITY = 9.81f,
    .MAX_VELOCITY = 500.0f
}

void ZFB_ApplyForce(ZFB_Entity *entity, ZFB_Vector2 force);
void ZFB_UpdatePhysics(ZFB_Entity *entity, float dt);
int ZFB_CheckCollision(ZFB_Entity a, ZFB_Entity b);
void ZFB_NewWPP(ZFB_WorldPhysicsProperties nwpp);

#endif
