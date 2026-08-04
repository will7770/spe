#pragma once
#include "allocators.h"
#include "datatypes.h"


#define MAX_ENTITIES 100 // to be changed in favor of changing the amount of active objects dynamically
extern const FVec2 GRAVITY;


typedef struct MainApp MainApp;
typedef struct UserRenderPanel UserRenderPanel;


typedef enum {
    BTYPE_STATIC,
    BTYPE_DYNAMIC,
} BodyType;


typedef enum {
    BSHAPE_RECTANGLE,
    BSHAPE_CIRCLE,
    // TBA: polygon
} BodyShape;


typedef struct {
    BodyType type;
    BodyShape shape;

    FVec2 screen_pos;
    FVec2 force;
    FVec2 velocity;

    f32 mass;
    FVec2 inv_mass;

    // TBA: circular movement and characteristics. (torque, angular vel, rotation)
    // TBA: friction, restitution

    ui32 id;
} RigidBody;


typedef struct {
    MemoryPool *pool;
    RigidBody *active_objects[MAX_ENTITIES];
    ui32 active_amount;
    f32 dt;
} Engine;



void InitPhysics(MainApp *app);

void DestroyPhysics(Engine *engine);

void UpdatePhysics(Engine *engine, UserRenderPanel *panel);

void PhysicsStep(RigidBody *body, f32 dt);

void Genesis(Engine *engine, const RigidBody *initial);

void Thanatos(Engine *engine, RigidBody *body);