#include <SDL3/SDL.h>
#include "datatypes.h"
#include "datastructs.h"
#include "physics.h"
#include "render.h"
#include <stdlib.h>
#include <stdio.h>



bool InitRender(MainApp *app, i32 vsync, int win_h, int win_w) {
    app->render = (Render*)malloc(sizeof(Render));
    Render *render = app->render;
    render->frame.framestart = SDL_GetTicksNS();

    if (!SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        SDL_Log("Subsystem init error: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("SPE", win_w, win_h, 0, &render->window, &render->renderer)) {
        SDL_Log("Window or renderer init error: %s", SDL_GetError());
        return false;
    }

    if (!SDL_SetRenderVSync(render->renderer, vsync)) {
        SDL_Log("VSync couldnt be enabled: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderLogicalPresentation(render->renderer, win_w, win_h, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return true;
}


void UpdateDelta(Render *render, Engine *engine) {
    FrameData *frame = &render->frame;

    frame->last_framestart = render->frame.framestart;
    frame->framestart = SDL_GetTicksNS();

    ui64 dt_ns = frame->framestart - frame->last_framestart;
    f32 dt = (f32)((double)dt_ns / 1000000000.0);

    if (dt > 0.1f) { dt = 0.1f; }
    engine->dt = dt;
}


void RenderShape(SDL_Renderer *renderer, RigidBody *body, ui8 vertice_amount, FVec2 raw_vertex_pos[]);

void RenderFrame(Render *render, Engine *engine) {
    SDL_Renderer *renderer = render->renderer;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    // idea: loop through phys. bodies and call their rendering callbacks
    for (ui32 i = 0; i < engine->active_amount; i++) {
        RigidBody *body = engine->active_objects[i];
        RenderShape(renderer, body, body->shape_data.vertice_cnt, body->shape_data.vertices);
    }
    if (engine->active_amount > 0) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugTextFormat(renderer, 0.0f, 0.0f, "Body1 velocity: { %f;%f }", engine->active_objects[0]->velocity.x, engine->active_objects[0]->velocity.y);
    }

    SDL_RenderPresent(renderer);
}


void RenderShape(SDL_Renderer *renderer, RigidBody *body, ui8 vertice_amount, FVec2 raw_vertex_pos[]) {
    if (body->shape == BSHAPE_CIRCLE) { vertice_amount = 65; }

    static SDL_Vertex vertices[65]; // make vertices and indices static arrs. dont accept radius and precision

    SDL_FColor color = { 0.0f, 0.0f, 0.0f, SDL_ALPHA_OPAQUE_FLOAT };

    if (body->shape == BSHAPE_CIRCLE) {
        vertices[0].position.x = body->screen_pos.x;
        vertices[0].position.y = body->screen_pos.y;
        vertices[0].color = color;

        for (ui8 i = 1; i < vertice_amount; i++) {
            f32 radians = 2.0f * PI_F * (float)i / (float)(vertice_amount-1);
            vertices[i].position.x = vertices[0].position.x + (body->shape_data.radius * PIXELS_PER_METER) * SDL_cosf(radians);
            vertices[i].position.y = vertices[0].position.y + (body->shape_data.radius * PIXELS_PER_METER) * SDL_sinf(radians);
            vertices[i].color = color;
        }
    }
    else if (body->shape == BSHAPE_POLYGON) {
        for (ui8 i = 0; i < vertice_amount; i++) {
            vertices[i].color = color;
            vertices[i].position.x = (raw_vertex_pos[i].x * PIXELS_PER_METER) + body->screen_pos.x;
            vertices[i].position.y = (raw_vertex_pos[i].y * PIXELS_PER_METER) + body->screen_pos.y;
        }
    }

    ui8 triangle_amount = vertice_amount-2;
    ui8 index_amount =  triangle_amount*3;
    i32 indices[index_amount];
    for (ui8 i = 0; i < triangle_amount; i++) {
        ui8 step = i*3;

        indices[step] = 0;
        indices[step+1] = i+1;
        indices[step+2] = i+2;
    }
    if (body->shape == BSHAPE_CIRCLE) { indices[index_amount-1] = 1; } // lock the last triangle

    SDL_RenderGeometry(renderer, NULL, vertices, vertice_amount, indices, index_amount);
}


void FreeRender(Render *render) {
    SDL_DestroyWindow(render->window);
    SDL_DestroyRenderer(render->renderer);
    SDL_Quit();
    free(render);
}