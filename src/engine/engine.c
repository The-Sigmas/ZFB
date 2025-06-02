#include "../headers/engine.h"

ZFB_WorldPhysicsProperties wpp = {
    .DAMPING = 0.98f, .GRAVITY = 9.81f, .MAX_VELOCITY = 500.0f};

// Apply a force to an entity (F = ma)
void ZFB_ApplyForce(ZFB_Entity *entity, ZFB_Vector2 force) {
  if (entity->physics.mass == 0)
    return;
  entity->physics.acceleration.x += force.x / entity->physics.mass;
  entity->physics.acceleration.y += force.y / entity->physics.mass;
}

// Update physics (position, velocity)
void ZFB_UpdatePhysics(ZFB_Entity *entity, float dt) {
  // Apply gravity if entity is affected by gravity
  if (entity->physics.gravity) {
    entity->physics.acceleration.y += wpp.GRAVITY;
  }

  // Update velocity based on acceleration
  entity->physics.velocity.x += entity->physics.acceleration.x * dt;
  entity->physics.velocity.y += entity->physics.acceleration.y * dt;

  // Apply damping (simulates friction/air resistance)
  entity->physics.velocity.x *= wpp.DAMPING;
  entity->physics.velocity.y *= wpp.DAMPING;

  // Cap velocity to prevent extreme speeds
  if (entity->physics.velocity.x > wpp.MAX_VELOCITY)
    entity->physics.velocity.x = wpp.MAX_VELOCITY;
  if (entity->physics.velocity.x < -wpp.MAX_VELOCITY)
    entity->physics.velocity.x = -wpp.MAX_VELOCITY;
  if (entity->physics.velocity.y > wpp.MAX_VELOCITY)
    entity->physics.velocity.y = wpp.MAX_VELOCITY;
  if (entity->physics.velocity.y < -wpp.MAX_VELOCITY)
    entity->physics.velocity.y = -wpp.MAX_VELOCITY;

  // Update position based on velocity
  entity->physics.position.x += entity->physics.velocity.x * dt;
  entity->physics.position.y += entity->physics.velocity.y * dt;

  // Reset acceleration (forces are applied per frame)
  entity->physics.acceleration.x = 0;
  entity->physics.acceleration.y = 0;

  entity->physics.angularVelocity += entity->physics.angularAcceleration * dt;
  entity->physics.angularVelocity *= wpp.DAMPING;

  if (entity->physics.angularVelocity > wpp.MAX_VELOCITY)
    entity->physics.angularVelocity = wpp.MAX_VELOCITY;
  if (entity->physics.angularVelocity < -wpp.MAX_VELOCITY)
    entity->physics.angularVelocity = -wpp.MAX_VELOCITY;

  entity->physics.rotation += entity->physics.angularVelocity * dt;

  if (entity->physics.rotation >= (float)(M_PI * 2))
    entity->physics.rotation -= (float)(M_PI * 2);
  if (entity->physics.rotation < 0.0f)
    entity->physics.rotation += (float)(M_PI * 2);

  entity->physics.angularAcceleration = 0.0f;
}

// Check if two entities are colliding (simple box collision)
int ZFB_CheckCollision(ZFB_Entity a, ZFB_Entity b) {
  float a_left = a.physics.position.x;
  float a_right = a.physics.position.x + a.width;
  float a_top = a.physics.position.y;
  float a_bottom = a.physics.position.y + a.height;

  float b_left = b.physics.position.x;
  float b_right = b.physics.position.x + b.width;
  float b_top = b.physics.position.y;
  float b_bottom = b.physics.position.y + b.height;

  return (a_right > b_left && a_left < b_right && a_bottom > b_top &&
          a_top < b_bottom);
}

void ZFB_ApplyTorque(ZFB_Entity *entity, float torque) {
  if (entity->physics.mass == 0)
    return;
  entity->physics.angularAcceleration += torque / entity->physics.mass;
}

void ZFB_NewWPP(ZFB_WorldPhysicsProperties nwpp) {
  wpp.DAMPING = nwpp.DAMPING;
  wpp.GRAVITY = nwpp.GRAVITY;
  wpp.MAX_VELOCITY = nwpp.MAX_VELOCITY;
  return;
}

void ZFB_ApplyForceLocal(ZFB_Entity *entity, ZFB_Vector2 lf) {
  if (entity->physics.rotation == 0) {
    ZFB_ApplyForce(entity, lf);
    return;
  }

  float c = cosf(entity->physics.rotation);
  float s = sinf(entity->physics.rotation);

  ZFB_Vector2 wf = {.x = lf.x * c - lf.y * s, .y = lf.x * s + lf.y * c};
  ZFB_ApplyForce(entity, wf);
}

void ZFB_EntityCorners(const ZFB_Entity *e, ZFB_Vector2 out[4]) {
  float hx = e->width * 0.5f;
  float hy = e->height * 0.5f;

  float c = cosf(e->physics.rotation);
  float s = sinf(e->physics.rotation);

#define ROT2WORLD(ix, iy)                                                      \
  ((ZFB_Vector2){.x = e->physics.position.x + (ix) * c - (iy) * s,             \
                 .y = e->physics.position.y + (ix) * s + (iy) * c})

  out[0] = ROT2WORLD(-hx, -hy);
  out[1] = ROT2WORLD(hx, -hy);
  out[2] = ROT2WORLD(hx, hy);
  out[3] = ROT2WORLD(-hx, hy);
#undef ROT2WORLD
}
static inline float zfb_dot(ZFB_Vector2 u, ZFB_Vector2 v) {
  return u.x * v.x + u.y * v.y;
}

static inline void zfb_project4(const ZFB_Vector2 p[4], ZFB_Vector2 axis,
                                float *minOut, float *maxOut) {
  float d0 = zfb_dot(p[0], axis);
  float d1 = zfb_dot(p[1], axis);
  float d2 = zfb_dot(p[2], axis);
  float d3 = zfb_dot(p[3], axis);

  float min = d0, max = d0;
  if (d1 < min)
    min = d1;
  else if (d1 > max)
    max = d1;
  if (d2 < min)
    min = d2;
  else if (d2 > max)
    max = d2;
  if (d3 < min)
    min = d3;
  else if (d3 > max)
    max = d3;
  *minOut = min;
  *maxOut = max;
}

int ZFB_ObbOverlap(const ZFB_Entity *a, const ZFB_Entity *b) {
  ZFB_Vector2 pa[4], pb[4];

  ZFB_Vector2 axes[4] = {
      {pa[1].y - pa[0].y, -(pa[1].x - pa[0].x)},
      {pa[2].y - pa[1].y, -(pa[2].x - pa[1].x)},
      {pb[1].y - pb[0].y, -(pb[1].x - pb[0].x)},
      {pb[2].y - pb[1].y, -(pb[2].x - pb[1].x)},
  };
  ZFB_EntityCorners(a, pa);
  ZFB_EntityCorners(b, pb);
  for (int i = 0; i < 4; ++i) {
    float minA, maxA, minB, maxB;
    zfb_project4(pa, axes[i], &minA, &maxA);
    zfb_project4(pb, axes[i], &minB, &maxB);
    if (maxA < minB || maxB < minA)
      return 0;
  }
  return 1;
};
int ZFB_Collide(const ZFB_Entity *a, const ZFB_Entity *b) {
  if (fabsf(a->physics.rotation) < ZFB_ROT_EPS &&
      fabsf(b->physics.rotation) < ZFB_ROT_EPS) {
    return ZFB_CheckCollision(*a, *b);
  }
  return ZFB_ObbOverlap(a, b);
}
