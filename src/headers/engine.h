#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

// This stores the physics properties of an entity
typedef struct {
    float x;
    float y;
} Vector2;

// This stores the physics properties of an entity
typedef struct {
    Vector2 position; // Where the object is
    Vector2 velocity; // How fast it's moving
    Vector2 acceleration; // How much force is applied
    float mass; // How heavy it is
} PhysicsBody; 

// This represents a game entity (Player or Wall)
typedef struct {
    int id; // Unique identifier
    PhysicsBody physics; // Physics properties
    int width, height; // Size (for collision detection)
} Entity;

// Function declarations 
void apply_force(Entity *entity, Vector2 force);
void update_physics(Entity *entity, float dt);
int check_collision(Entity *a, Entity *b);

#endif

