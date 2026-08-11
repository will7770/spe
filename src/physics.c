#include "allocators.h"
#include "datatypes.h"
#include "datastructs.h"
#include "physics.h"
#include <stdlib.h>
#include <math.h>


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


void PhysicsStep(RigidBody *body, f32 dt);
// replace panel with spawn queue
void UpdatePhysics(Engine *engine) {
    for (ui32 i = 0; i < engine->active_amount; i++) {
        RigidBody *body = engine->active_objects[i];
        PhysicsStep(body, engine->dt);
    }
}


void PhysicsStep(RigidBody *body, f32 dt) {
    // gravitational force (also, i could make an arena for cache locality. each time the function runs its gonna reference a known addr)
    FVec2 accel = fvec2_scalar(body->force, body->inv_mass);
    accel.y += GRAVITY.y;
    body->velocity = fvec2_add(body->velocity, fvec2_scalar(accel, dt)); // <- infinitely adds 
    body->force.x = 0.0f; body->force.y = 0.0f;

    // hazardous hardcode below
    if (body->screen_pos.y < (640.0f - 50.0f)) {
        body->screen_pos = fvec2_add(body->screen_pos, body->velocity);
     }
    
}


bool Genesis(Engine *engine, const RigidBody *bodydef) {
    if (engine->active_amount == MAX_ENTITIES) {
        return false;
    }

    RigidBody *new_body = pool_alloc(engine->pool);
    *new_body = *bodydef;

    engine->active_objects[engine->active_amount] = new_body;
    engine->active_amount++;

    return true;
}


void Thanatos(Engine *engine, RigidBody *body) {
    engine->active_objects[body->id] = engine->active_objects[engine->active_amount-1];
    engine->active_amount--;

    pool_free(engine->pool, (void*)body);
}


void CreateSimplePolygon(FVec2 vertices[], ui8 edges, f32 radius) {
    for (ui8 i = 0; i < edges; i++) {
        f32 radians = (2.0f * PI_F * i) / edges;
        vertices[i].x = (radius * cosf(radians));
        vertices[i].y = (radius * sinf(radians));
    }
}