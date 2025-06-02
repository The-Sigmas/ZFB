#include "../headers/engine.h"

// Global physics settings for the world (damping, gravity, max velocity)
ZFB_WorldPhysicsProperties wpp = {
    .DAMPING = 0.98f, .GRAVITY = 9.81f, .MAX_VELOCITY = 500.0f};

// Apply a force to an entity (F = m * a)
void ZFB_ApplyForce(ZFB_Entity *entity, ZFB_Vector2 force) {
  // Do nothing if mass is zero (immovable object)
  if (entity->physics.mass == 0)
    return;

  // a = F / m → accumulate into acceleration
  entity->physics.acceleration.x += force.x / entity->physics.mass;
  entity->physics.acceleration.y += force.y / entity->physics.mass;
}

// Update an entity's physics properties over a time step dt
void ZFB_UpdatePhysics(ZFB_Entity *entity, float dt) {
  // Apply gravity to acceleration if enabled
  if (entity->physics.gravity) {
    entity->physics.acceleration.y += wpp.GRAVITY;
  }

  // Integrate acceleration into velocity
  entity->physics.velocity.x += entity->physics.acceleration.x * dt;
  entity->physics.velocity.y += entity->physics.acceleration.y * dt;

  // Apply damping (simulates friction or air resistance)
  entity->physics.velocity.x *= wpp.DAMPING;
  entity->physics.velocity.y *= wpp.DAMPING;

  // Clamp velocity to maximum limits in X
  if (entity->physics.velocity.x > wpp.MAX_VELOCITY)
    entity->physics.velocity.x = wpp.MAX_VELOCITY;
  if (entity->physics.velocity.x < -wpp.MAX_VELOCITY)
    entity->physics.velocity.x = -wpp.MAX_VELOCITY;
  // Clamp velocity to maximum limits in Y
  if (entity->physics.velocity.y > wpp.MAX_VELOCITY)
    entity->physics.velocity.y = wpp.MAX_VELOCITY;
  if (entity->physics.velocity.y < -wpp.MAX_VELOCITY)
    entity->physics.velocity.y = -wpp.MAX_VELOCITY;

  // Integrate velocity into position
  entity->physics.position.x += entity->physics.velocity.x * dt;
  entity->physics.position.y += entity->physics.velocity.y * dt;

  // Reset acceleration after applying all forces for this frame
  entity->physics.acceleration.x = 0;
  entity->physics.acceleration.y = 0;

  // Integrate angular acceleration into angular velocity
  entity->physics.angularVelocity += entity->physics.angularAcceleration * dt;
  // Apply damping to angular velocity
  entity->physics.angularVelocity *= wpp.DAMPING;

  // Clamp angular velocity to maximum limits
  if (entity->physics.angularVelocity > wpp.MAX_VELOCITY)
    entity->physics.angularVelocity = wpp.MAX_VELOCITY;
  if (entity->physics.angularVelocity < -wpp.MAX_VELOCITY)
    entity->physics.angularVelocity = -wpp.MAX_VELOCITY;

  // Integrate angular velocity into rotation (radians)
  entity->physics.rotation += entity->physics.angularVelocity * dt;

  // Keep rotation within [0, 2π)
  if (entity->physics.rotation >= (float)(M_PI * 2))
    entity->physics.rotation -= (float)(M_PI * 2);
  if (entity->physics.rotation < 0.0f)
    entity->physics.rotation += (float)(M_PI * 2);

  // Reset angular acceleration
  entity->physics.angularAcceleration = 0.0f;
}

// Simple axis-aligned bounding box (AABB) collision check between two entities
int ZFB_CheckCollision(ZFB_Entity a, ZFB_Entity b) {
  float a_left = a.physics.position.x;
  float a_right = a.physics.position.x + a.width;
  float a_top = a.physics.position.y;
  float a_bottom = a.physics.position.y + a.height;

  float b_left = b.physics.position.x;
  float b_right = b.physics.position.x + b.width;
  float b_top = b.physics.position.y;
  float b_bottom = b.physics.position.y + b.height;

  // Overlapping if intervals intersect on both axes
  return (a_right > b_left && a_left < b_right && a_bottom > b_top &&
          a_top < b_bottom);
}

// Apply a torque to an entity (affects angular acceleration)
void ZFB_ApplyTorque(ZFB_Entity *entity, float torque) {
  // Do nothing if mass is zero (immovable or infinite inertia)
  if (entity->physics.mass == 0)
    return;
  // α = τ / I ; treat mass as proxy for moment of inertia here
  entity->physics.angularAcceleration += torque / entity->physics.mass;
}

// Update global physics properties (damping, gravity, max velocity)
void ZFB_NewWPP(ZFB_WorldPhysicsProperties nwpp) {
  wpp.DAMPING = nwpp.DAMPING;
  wpp.GRAVITY = nwpp.GRAVITY;
  wpp.MAX_VELOCITY = nwpp.MAX_VELOCITY;
  return;
}

// Apply a force in the entity's local coordinate frame (rotated by its
// orientation)
void ZFB_ApplyForceLocal(ZFB_Entity *entity, ZFB_Vector2 lf) {
  // If no rotation, local = world directly
  if (entity->physics.rotation == 0) {
    ZFB_ApplyForce(entity, lf);
    return;
  }

  // Rotate local force vector by entity's rotation to get world force
  float c = cosf(entity->physics.rotation);
  float s = sinf(entity->physics.rotation);

  ZFB_Vector2 wf = {.x = lf.x * c - lf.y * s, .y = lf.x * s + lf.y * c};
  ZFB_ApplyForce(entity, wf);
}

// Calculate the four world-space corners of an oriented bounding box (entity)
void ZFB_EntityCorners(const ZFB_Entity *e, ZFB_Vector2 out[4]) {
  // Half extents
  float hx = e->width * 0.5f;
  float hy = e->height * 0.5f;

  // Precompute cos and sin of rotation
  float c = cosf(e->physics.rotation);
  float s = sinf(e->physics.rotation);

// Helper macro: rotate a local point (ix, iy) to world and offset by position
#define ROT2WORLD(ix, iy)                                                      \
  ((ZFB_Vector2){.x = e->physics.position.x + (ix) * c - (iy) * s,             \
                 .y = e->physics.position.y + (ix) * s + (iy) * c})

  // Compute the four corners (lower-left, lower-right, upper-right, upper-left)
  out[0] = ROT2WORLD(-hx, -hy);
  out[1] = ROT2WORLD(hx, -hy);
  out[2] = ROT2WORLD(hx, hy);
  out[3] = ROT2WORLD(-hx, hy);

#undef ROT2WORLD
}

// Compute dot product of two 2D vectors
static inline float zfb_dot(ZFB_Vector2 u, ZFB_Vector2 v) {
  return u.x * v.x + u.y * v.y;
}

// Project 4 points (corners of an OBB) onto an axis, returning min and max
// values
static inline void zfb_project4(const ZFB_Vector2 p[4], ZFB_Vector2 axis,
                                float *minOut, float *maxOut) {
  float d0 = zfb_dot(p[0], axis);
  float d1 = zfb_dot(p[1], axis);
  float d2 = zfb_dot(p[2], axis);
  float d3 = zfb_dot(p[3], axis);

  // Initialize min/max to the first projection
  float min = d0, max = d0;
  // Check each projection against current min/max
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

// Check overlap between two oriented bounding boxes (OBB) using the Separating
// Axis Theorem
int ZFB_ObbOverlap(const ZFB_Entity *a, const ZFB_Entity *b) {
  ZFB_Vector2 pa[4], pb[4];

  // Compute corners for both entities
  ZFB_EntityCorners(a, pa);
  ZFB_EntityCorners(b, pb);

  // Compute 4 candidate separating axes (normals to each box edge)
  ZFB_Vector2 axes[4] = {// Edge 0→1 of A, perpendicular
                         {pa[1].y - pa[0].y, -(pa[1].x - pa[0].x)},
                         // Edge 1→2 of A, perpendicular
                         {pa[2].y - pa[1].y, -(pa[2].x - pa[1].x)},
                         // Edge 0→1 of B, perpendicular
                         {pb[1].y - pb[0].y, -(pb[1].x - pb[0].x)},
                         // Edge 1→2 of B, perpendicular
                         {pb[2].y - pb[1].y, -(pb[2].x - pb[1].x)}};

  // For each axis, project both OBBs and check for gaps
  for (int i = 0; i < 4; ++i) {
    float minA, maxA, minB, maxB;
    zfb_project4(pa, axes[i], &minA, &maxA);
    zfb_project4(pb, axes[i], &minB, &maxB);
    // If projections don't overlap on this axis, no collision
    if (maxA < minB || maxB < minA)
      return 0;
  }
  // Overlaps on all axes → collision
  return 1;
}

// General collision check: if both entities have negligible rotation, use AABB;
// else, use OBB test
int ZFB_Collide(const ZFB_Entity *a, const ZFB_Entity *b) {
  if (fabsf(a->physics.rotation) < ZFB_ROT_EPS &&
      fabsf(b->physics.rotation) < ZFB_ROT_EPS) {
    // Both unrotated (or close to), use faster AABB check
    return ZFB_CheckCollision(*a, *b);
  }
  // At least one is rotated → use full OBB overlap test
  return ZFB_ObbOverlap(a, b);
}
