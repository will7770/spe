#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "datastructs.h"
#include "render.h"
#include "physics.h"



void InitMainLoop(MainApp *app);
bool HandleEvent(MainApp *app);


int main(int argc, char *argv[]) {
    MainApp app = {0};
    InitRender(&app, 1, 640, 1280);
    InitPhysics(&app);
    app.alive = 1;

    InitMainLoop(&app);

    FreeRender(app.render);
    DestroyPhysics(app.engine);
    
    return 0;
}


void InitMainLoop(MainApp *app) {
    while (app->alive) {
        SDL_Event *ev = &app->render->event;

        while(SDL_PollEvent(ev)) {
            if (!HandleEvent(app)) {
                SDL_Log("Error during event handling: %s", SDL_GetError());
                break;
            }
        }
        UpdateDelta(app->render, app->engine);
        UpdatePhysics(app->engine, &app->userpanel);
        RenderFrame(app->render, app->engine);
    }

}


typedef bool (*EventHandlerFunc)(MainApp *app);

typedef struct {
    SDL_EventType event;
    EventHandlerFunc func;
} EventHandlerOp;

// if i extend this: 1. add a declaration below, 2. add it into the jump table
bool HandleKeyDown(MainApp *app);
bool HandleKeyUp(MainApp *app);

bool HandleEvent(MainApp *app) {
    if (app->render->event.type == SDL_EVENT_QUIT) {
        app->alive = false;
        return true;
    }

    static const EventHandlerOp HandlerTable[] = {
        { SDL_EVENT_KEY_DOWN, HandleKeyDown },
        { SDL_EVENT_KEY_UP, HandleKeyUp },
    };
    static const ui32 TABLE_SIZE = sizeof(HandlerTable) / sizeof(EventHandlerOp);

    for (ui32 i = 0; i < TABLE_SIZE; i++) {
        if (HandlerTable[i].event == app->render->event.type) {
            HandlerTable[i].func(app);
        }
    }

    return true;
}


bool HandleKeyDown(MainApp *app) {
    SDL_KeyboardEvent *key = &app->render->event.key;
    UserRenderPanel *panel = &app->userpanel;

    switch (key->scancode) {
        // prepare to spawn an object on the next frame
        case SDL_SCANCODE_Q:
            panel->draw_on_next_frame = true;
            SDL_GetMouseState(&panel->draw_coord.x, &panel->draw_coord.y);
            break;
        // choosing a shape
        case SDL_SCANCODE_1: panel->body_shape = BSHAPE_RECTANGLE; break;
        case SDL_SCANCODE_2: panel->body_shape = BSHAPE_CIRCLE; break;
        // TBA: choosing a color
        default: break;
    }
    return true;
}


bool HandleKeyUp(MainApp *app) {
    return true;
}