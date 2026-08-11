#pragma once
#include "allocators.h"
#include "datatypes.h"
#include "stdbool.h"


#define MAX_ENTITIES 100 // to be changed in favor of changing the amount of active objects dynamically
#define MAX_POLYGON_VERTICES 8
extern const FVec2 GRAVITY;


typedef struct MainApp MainApp;
typedef struct UserRenderPanel UserRenderPanel;


typedef enum {
    BTYPE_STATIC,
    BTYPE_DYNAMIC,
} BodyType;


typedef enum {
    BSHAPE_CIRCLE,
    BSHAPE_POLYGON,
} BodyShape;


typedef struct {
    BodyType type;
    BodyShape shape;

    ui32 geometry_id;

    FVec2 screen_pos;
    FVec2 force;
    FVec2 velocity;

    f32 mass; // kilograms
    f32 inv_mass;

    // TBA: circular movement and characteristics. (torque, angular vel, rotation)
    // TBA: friction, restitution

    ui32 id;
} RigidBody;


typedef struct BodyGeometry {
    BodyShape shape;
    ui32 body_id;

    union {
        f32 radius;
        struct {
            FVec2 vertices[MAX_POLYGON_VERTICES];
            ui32 vertice_cnt;
        };
    } data;
} BodyGeometry;


typedef struct {
    MemoryPool *pool;

    // DEPRECATED
    RigidBody *active_objects[MAX_ENTITIES];
    ui32 active_amount;
    // DEPRECATED
    f32 dt;
} Engine;



void InitPhysics(MainApp *app);

void DestroyPhysics(Engine *engine);

void UpdatePhysics(Engine *engine);

bool Genesis(Engine *engine, const RigidBody *bodydef);

void Thanatos(Engine *engine, RigidBody *body);


void CreateSimplePolygon(FVec2 vertices[], ui8 edges, f32 radius);