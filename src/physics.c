#include "allocators.h"
#include "datatypes.h"
#include "datastructs.h"
#include "physics.h"
#include <stdlib.h>


const FVec2 GRAVITY = { 0.0f, 19.62f };

void InitPhysics(MainApp *app) {
    app->engine = (Engine*)malloc(sizeof(Engine));
    app->engine->pool = pool_init(sizeof(RigidBody), MAX_ENTITIES);
    app->engine->active_amount = 0;
}


void DestroyPhysics(Engine *engine) {
    pool_destroy(engine->pool);
    free(engine);
}


void UpdatePhysics(Engine *engine, UserRenderPanel *panel) {
    if (panel->draw_on_next_frame) {
        if (engine->active_amount < MAX_ENTITIES) {
            RigidBody new_body = {
            .shape = panel->body_shape,
            .type = panel->body_type,
            .screen_pos = { .x = panel->draw_coord.x, .y = panel->draw_coord.y },
            // TBA: fields below also need dynamic initializing
            .force = { 0.0f, 0.0f },
            .mass = 100.0f,
            .inv_mass = {1.0f / 1.0f, 1.0f / 1.0f},
            .velocity = { 0.0f, 0.5f },
            .id = engine->active_amount + 1,
            };
            Genesis(engine, &new_body);
        }
        panel->draw_on_next_frame = false;
    }

    for (ui32 i = 0; i < engine->active_amount; i++) {
        RigidBody *body = engine->active_objects[i];
        PhysicsStep(body, engine->dt);
    }
}


void PhysicsStep(RigidBody *body, f32 dt) {
    // gravitational force (also, i could make an arena for cache locality. each time the function runs its gonna reference a known addr)
    FVec2 accel = fvec2_mult(body->force, body->inv_mass);
    accel.y += GRAVITY.y;
    body->velocity = fvec2_add(body->velocity, fvec2_scalar(accel, dt)); // <- infinitely adds 
    body->force.x = 0.0f; body->force.y = 0.0f;

    // hazardous hardcode below
    if (body->screen_pos.y < (640.0f - 50.0f)) { body->screen_pos = fvec2_add(body->screen_pos, body->velocity); }
    
}


void Genesis(Engine *engine, const RigidBody *initial) {
    RigidBody *new_body = pool_alloc(engine->pool);
    *new_body = *initial;

    engine->active_objects[engine->active_amount] = new_body;
    engine->active_amount++;
}


void Thanatos(Engine *engine, RigidBody *body) {
    engine->active_objects[body->id] = engine->active_objects[engine->active_amount-1];
    engine->active_amount--;

    pool_free(engine->pool, (void*)body);
}