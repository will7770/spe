#pragma once
#include <stddef.h>
#include "datatypes.h"
#include "physics.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL.h"
#include "render.h"


typedef struct {
    BodyType body_type;
    BodyShape body_shape;
    SDL_Color color;
    SDL_FPoint draw_coord;
    bool draw_on_next_frame;
} UserRenderPanel;

typedef struct {
    Engine *engine;
    Render *render;

    UserRenderPanel *userpanel;

    ui8 alive;
} MainApp;


typedef struct {
    ui32 x;
    ui32 y;
} Vec2;

typedef struct {
    f32 x;
    f32 y;
} FVec2;