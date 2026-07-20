#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <datastructs.h>



MainApp app = {0};

void InitMainLoop(MainApp *app);


int main(int argc, char *argv[]) {
    // ALLOCATE MEMORY FOR ENGINE AND RENDERER SOMEWHERE! app is supposed to store pointers, and where do they point? no idea! fix.
    InitRender(&app.render, 1);
    InitPhysics(&app.engine);
    app.alive = 1;

    InitMainLoop(&app);

    FreeRender(&app.render);
    
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
        UpdatePhysics(app->engine, app->userpanel);
        // SDL_RenderPresent(renderer); change to actual render logic
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
    static const EventHandlerOp HandlerTable[] = {
        { SDL_EVENT_KEY_DOWN, HandleKeyDown },
        { SDL_EVENT_KEY_UP, HandleKeyUp },
    };
    static const TABLE_SIZE = sizeof(HandlerTable) / sizeof(EventHandlerOp);

    for (ui32 i = 0; i < TABLE_SIZE; i++) {
        if (HandlerTable[i].event == app->render->event.type) {
            HandlerTable[i].func(app);
        }
    }

    return true;
}


bool HandleKeyDown(MainApp *app) {
    SDL_KeyboardEvent *key = &app->render->event.key;
    UserRenderPanel *panel = app->userpanel;

    switch (key->scancode) {
        // prepare to spawn an object on the next frame
        case SDL_SCANCODE_Q:
            panel->draw_on_next_frame = true;
            SDL_GetMouseState(&panel->draw_coord.x, &panel->draw_coord.y);
            break;
        // choosing a shape
        case SDL_SCANCODE_1: panel->body_shape = BSHAPE_RECTANGLE; break;
        // TBA: choosing a color
        default: break;
    }
    return true;
}


bool HandleKeyDown(MainApp *app) {
    return true;
}