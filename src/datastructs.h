#pragma once
#include <stddef.h>
#include "datatypes.h"
#include "physics.h"
#include "SDL3/SDL.h"
#include "render.h"


typedef struct UserRenderPanel {
    BodyType body_type;
    BodyShape body_shape;
    SDL_Color color;
    SDL_FPoint draw_coord;
    bool draw_on_next_frame;
} UserRenderPanel;

typedef struct MainApp {
    Engine *engine;
    Render *render;

    UserRenderPanel userpanel;

    ui8 alive;
} MainApp;