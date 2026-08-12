#include "allocators.h"
#include "datatypes.h"
#include "datastructs.h"
#include "physics.h"
#include <stdlib.h>
#include <math.h>


const FVec2 GRAVITY = { 0.0f, 19.62f };


void InitPhysics(MainApp *app) {
    app->engine = (Engine*)malloc(sizeof(Engine));
    app->engine->rbodies = (RigidBody*)darray_init(sizeof(RigidBody), MAX_ENTITIES);
    app->engine->gbodies = (BodyGeometry*)darray_init(sizeof(BodyGeometry), MAX_ENTITIES);
    app->engine->active_amount = 0;
}


void DestroyPhysics(Engine *engine) {
    darray_free(engine->rbodies);
    darray_free(engine->gbodies);
    free(engine);
}


void PhysicsStep(RigidBody *body, f32 dt);
// replace panel with spawn queue
void UpdatePhysics(Engine *engine) {
    for (ui32 i = 0; i < engine->active_amount; i++) {
        RigidBody *body = &engine->rbodies[i];
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


bool Genesis(Engine *engine, const BodyDef *bodydef) {
    if (engine->active_amount == MAX_ENTITIES) {
        return false;
    }

    RigidBody *new_rbody = darray_alloc(engine->rbodies, engine->active_amount, sizeof(RigidBody));
    new_rbody->force = bodydef->force;
    new_rbody->mass = bodydef->mass;
    new_rbody->inv_mass = 1.0f / new_rbody->mass;
    new_rbody->screen_pos = bodydef->screen_pos;
    new_rbody->velocity = bodydef->velocity;
    new_rbody->type = bodydef->type;

    BodyGeometry *new_gbody = darray_alloc(engine->gbodies, engine->active_amount, sizeof(BodyGeometry));
    new_gbody->shape = bodydef->shape;
    if (new_gbody->shape == BSHAPE_POLYGON) {
        new_gbody->data.vertice_cnt = bodydef->data.vertice_cnt;
        memcpy(new_gbody->data.vertices, bodydef->data.vertices, sizeof(FVec2)*bodydef->data.vertice_cnt);
    }
    else {
        new_gbody->data.radius = bodydef->data.radius;
    }

    engine->active_amount++;

    return true;
}


void Thanatos(Engine *engine, ui32 body_id) {
    darray_dealloc(engine->rbodies, engine->active_amount, sizeof(RigidBody), body_id);
    engine->active_amount--;
}


void CreateSimplePolygon(FVec2 vertices[], ui8 edges, f32 radius) {
    for (ui8 i = 0; i < edges; i++) {
        f32 radians = (2.0f * PI_F * i) / edges;
        vertices[i].x = (radius * cosf(radians));
        vertices[i].y = (radius * sinf(radians));
    }
}