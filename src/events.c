#include <SDL3/SDL.h>
#include "datastructs.h"
#include "physics.h"



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


void CreateBodyFromInput(Engine *engine, UserRenderPanel *panel);

bool HandleKeyDown(MainApp *app) {
    SDL_KeyboardEvent *key = &app->render->event.key;
    UserRenderPanel *panel = &app->userpanel;

    if (key->scancode == SDL_SCANCODE_Q) {
        SDL_GetMouseState(&panel->draw_coord.x, &panel->draw_coord.y);
        panel->body_type = BTYPE_DYNAMIC;
        CreateBodyFromInput(app->engine, panel);
    }
    else if (SDL_SCANCODE_1 <= key->scancode && key->scancode <= SDL_SCANCODE_8) {
        panel->vertices_amount = MAX_POLYGON_VERTICES - (SDL_SCANCODE_8 - key->scancode);
    }
    // TBA: choosing colors
    return true;
}


bool HandleKeyUp(MainApp *app) {
    return true;
}


void CreateBodyFromInput(Engine *engine, UserRenderPanel *panel) {
    if (engine->active_amount < MAX_ENTITIES) {
        BodyDef new_body;
        new_body.screen_pos = (FVec2){ .x = panel->draw_coord.x, .y = panel->draw_coord.y };

        if (panel->vertices_amount == 1) { 
            new_body.shape = BSHAPE_CIRCLE;
            new_body.data.radius = 50.0f;
        }
        else {
            new_body.shape = BSHAPE_POLYGON;
            new_body.data.vertice_cnt = panel->vertices_amount;
            CreateSimplePolygon(new_body.data.vertices, panel->vertices_amount, 50.0f);
        }

        new_body.type = panel->body_type;
        // TBA: fields below also need dynamic initializing
        new_body.force = (FVec2){ 0.0f, 0.0f };
        new_body.mass = 100.0f;
        new_body.velocity = (FVec2){ 0.0f, 0.5f };

        Genesis(engine, &new_body);
    }

}