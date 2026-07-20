#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "datatypes.h"


typedef struct {
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


bool InitRender(Render *render, i32 vsync);

void FreeRender(Render *render);