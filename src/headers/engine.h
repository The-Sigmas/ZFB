#ifndef ENGINE_H
#define ENGINE_H

#define ZFB_ROT_EPS  0.001f
#include <stdint.h>
#include <stdbool.h>

#define M_PI   3.14159265358979323846f
#define M_TAU  6.28318530717958647692f

static inline float zfb_sinf(float x){
        while (x >  M_PI) x -= M_TAU;
        while (x < -M_PI) x += M_TAU;
        float x2 = x * x;
        return x * (1.0f - x2 / 6.0f + x2 * x2 / 120.0f);
}
static inline float zfb_cosf(float x) { return zfb_sinf(x + M_PI * 0.5f); }
#define sinf(x)  zfb_sinf(x)
#define cosf(x)  zfb_cosf(x)

static inline float zfb_fabsf(float x) { return x < 0.0f ? -x : x; }
#define fabsf(x) zfb_fabsf(x)
 
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
    float rotation;
    float angularVelocity;
    float angularAcceleration;
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
void ZFB_ApplyTorque(ZFB_Entity *entity, float torque);
void ZFB_ApplyForceLocal(ZFB_Entity *entity, ZFB_Vector2 lf);

#endif
