#include "allocators.h"
#include "datatypes.h"
#include "datastructs.h"

#define MAX_ENTITIES 100 // to be changed in favor of changing the amount of active objects dynamically


typedef enum {
    STATIC,
    DYNAMIC,
} BodyType;


typedef enum {
    RECTANGLE,
    // TBA: circle, polygon
} BodyShape;


typedef struct {
    BodyType type;
    BodyShape shape;

    FVec2 screen_pos;
    FVec2 force;
    FVec2 velocity;

    f32 mass;
    f32 inv_mass;

    // TBA: circular movement and characteristics. (torque, angular vel, rotation)
    // TBA: friction, restitution
} RigidBody;


typedef struct {
    MemoryPool *pool;
    RigidBody *active_objects[MAX_ENTITIES];
    ui32 active_amount;
    f32 dt;
} Engine;


void InitPhysics(Engine *engine) {
    engine->pool = pool_init(sizeof(RigidBody), MAX_ENTITIES);
    engine->active_amount = 0;
}


void DestroyPhysics(Engine *engine) {
    pool_destroy(engine->pool);
    
}


void UpdatePhysics(Engine *engine, UserRenderPanel *panel) {
    if (panel->draw_on_next_frame) {
        RigidBody new_body = {
            .shape = panel->body_shape,
            .type = panel->body_type,
            .screen_pos = { .x = panel->draw_coord.x, .y = panel->draw_coord.y },
            // TBA: fields below also need dynamic initializing
            .force = {0},
            .mass = 100.0f,
            .inv_mass = -100.0f,
            .velocity = {0},
        };
        Genesis(engine, &new_body); // do i really need to return a body inside of genesis?
    }

    for (ui32 i = 0; i < engine->active_amount; i++) {
        RigidBody *body = engine->active_objects[i];
        PhysicsStep(body, engine->dt);
    }
}


void PhysicsStep(RigidBody *body, f32 dt) {

}


RigidBody *Genesis(Engine *engine, const RigidBody *initial) {
    RigidBody *new_body = pool_alloc(engine->pool);
    *new_body = *initial;

    engine->active_objects[engine->active_amount] = new_body;
    engine->active_amount++;

    return new_body;
}


void Thanatos() {

}