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



bool InitRender(Render *render, i32 vsync) {
    if (!SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        SDL_Log("Subsystem init error: %s", SDL_GetError());
        return -1;
    }

    if (!SDL_CreateWindowAndRenderer("SPE", render->window_width, render->window_height, 0, render->window, render->renderer)) {
        SDL_Log("Window or renderer init error: %s", SDL_GetError());
        return -1;
    }

    if (!SDL_SetRenderVSync(render->renderer, vsync)) {
        SDL_Log("VSync couldnt be enabled: %s", SDL_GetError());
        return -1;
    }

    SDL_SetRenderLogicalPresentation(render->renderer, render->window_width, render->window_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    render->frame.framestart = SDL_GetTicks();

    return 0;
}


void FreeRender(Render *render) {
    SDL_DestroyWindow(render->window);
    SDL_DestroyRenderer(render->renderer);
    SDL_Quit();
}