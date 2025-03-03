#include <stdio.h>
#include <assert.h>
#include "./src/headers/engine.h"  

// Test: apply_force()
void test_apply_force() {
    ZFB_Entity entity = { .id = 1, .width = 10, .height = 10, .physics = {{0, 0}, {0, 0}, {0, 0}, 2} };
    ZFB_Vector2 force = {4, 2};

    apply_force(&entity, force);

    assert(entity.physics.acceleration.x == 2.0);  // 4 / 2 = 2
    assert(entity.physics.acceleration.y == 1.0);  // 2 / 2 = 1

    printf("test_apply_force bestanden\n");
}

// Test: update_physics()
void test_update_physics() {
    ZFB_Entity entity = { .id = 1, .width = 10, .height = 10, .physics = {{0, 0}, {1, 1}, {1, 1}, 1} };
    float dt = 1.0; // simulate 1 second

    update_physics(&entity, dt);

    assert(entity.physics.velocity.x == 2.0);  // v = v + a * dt → 1 + 1 * 1 = 2
    assert(entity.physics.velocity.y == 2.0);
    assert(entity.physics.position.x == 2.0);  // x = x + v * dt → 0 + 2 * 1 = 2
    assert(entity.physics.position.y == 2.0);

    assert(entity.physics.acceleration.x == 0.0);  // Acceleration reset
    assert(entity.physics.acceleration.y == 0.0);

    printf(" test_update_physics bestanden\n");
}

// Test: check_collision()
void test_check_collision() {
    ZFB_Entity a = { .id = 1, .width = 10, .height = 10, .physics = {{10, 10}, {0, 0}, {0, 0}, 1} };
    ZFB_Entity b = { .id = 2, .width = 10, .height = 10, .physics = {{15, 15}, {0, 0}, {0, 0}, 1} };

    assert(check_collision(&a, &b) == 1); // Collision

    b.physics.position.x = 30;
    b.physics.position.y = 30;
    
    assert(check_collision(&a, &b) == 0); // no collision

    printf(" test_check_collision bestanden\n");
}

// Main function
int main() {
    test_apply_force();
    test_update_physics();
    test_check_collision();

    printf("\n Alle Tests erfolgreich bestanden!\n");
    return 0;
}
