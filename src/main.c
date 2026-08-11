#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include "datastructs.h"
#include "render.h"
#include "physics.h"
#include "events.h"




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
        UpdatePhysics(app->engine);
        RenderFrame(app->render, app->engine);
    }

}


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