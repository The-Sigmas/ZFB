#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float x;
    float y;
} ZFB_Vector2;

typedef struct {
    ZFB_Vector2 position;
    ZFB_Vector2 velocity;
    ZFB_Vector2 acceleration;
    float mass;
    bool gravity;
} ZFB_PhysicsBody;

typedef struct {
    int id;
    ZFB_PhysicsBody physics;
    int width, height;
} ZFB_Entity;

typedef struct
{
    float DAMPING;
    float GRAVITY;
    float MAX_VELOCITY;
} ZFB_WorldPhysicsProperties;

extern ZFB_WorldPhysicsProperties wpp;

void ZFB_ApplyForce(ZFB_Entity *entity, ZFB_Vector2 force);
void ZFB_UpdatePhysics(ZFB_Entity *entity, float dt);
int ZFB_CheckCollision(ZFB_Entity a, ZFB_Entity b);
void ZFB_NewWPP(ZFB_WorldPhysicsProperties nwpp);

#endif
