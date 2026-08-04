#pragma once
#include <SDL3/SDL.h>
#include "datatypes.h"


typedef struct MainApp MainApp;
typedef struct UserRenderPanel UserRenderPanel;


typedef struct {
    ui64 last_framestart;
    ui64 framestart;
} FrameData;


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Event event;
    FrameData frame;

    ui32 window_height;
    ui32 window_width;
} Render;


bool InitRender(MainApp *app, i32 vsync, int win_h, int win_w);

void UpdateDelta(Render *render, Engine *engine);

void RenderFrame(Render *render, Engine *engine);

void FreeRender(Render *render);