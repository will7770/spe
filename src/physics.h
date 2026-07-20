#include "allocators.h"
#include "datatypes.h"
#include "datastructs.h"

#define MAX_ENTITIES 100


typedef enum {
    BTYPE_STATIC,
    BTYPE_DYNAMIC,
} BodyType;

typedef enum {
    BSHAPE_RECTANGLE,
} BodyShape;

typedef struct {
    BodyType type;
    BodyShape shape;

    FVec2 screen_pos;
    FVec2 force;
    FVec2 velocity;

    f32 mass;
    f32 inv_mass;

} RigidBody;

typedef struct {
    MemoryPool *pool;
    RigidBody *active_objects[MAX_ENTITIES];
    ui32 active_amount;
    f32 dt;
} Engine;


void InitPhysics(Engine *engine);

void DestroyPhysics(Engine *engine);

void UpdatePhysics(Engine *engine, UserRenderPanel *panel);

void PhysicsStep(RigidBody *body, f32 dt);

RigidBody *Genesis(Engine *engine, const RigidBody *initial);

void Thanatos();