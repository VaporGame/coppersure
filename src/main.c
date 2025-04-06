#define SDL_MAIN_USE_CALLBACKS 1
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "gui.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360
#define FRAMERATE 30

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static float SCALE = 1.0f;

static ComponentArray compArray;

SDL_HitTestResult hitTestCallback(SDL_Window *window, const SDL_Point *area, void*data) {
    if (area->x > 619 && area->x < 635 && area->y > 5 && area->y < 19) {
        return SDL_HITTEST_NORMAL;
    } else if (area->y <= 21) {
        return SDL_HITTEST_DRAGGABLE;
    }
    return SDL_HITTEST_NORMAL;
}

void titleBarButton(int* args) {
    //SDL_Log("%d, %d", args[0], args[1]);

    int dropdownIdx = compArray.array[args[0]]->element.button->dropdownIdx;
    if(dropdownIdx > 0) {
        bool state = (args[1] == 1);
        compArray.array[dropdownIdx]->element.dropdown->_.active = state;

        for(size_t i = 0; i < compArray.array[dropdownIdx]->element.dropdown->buttonsCount; i++) {
            compArray.array[compArray.array[dropdownIdx]->element.dropdown->buttons[i]]->element.button->_.active = state;
        }
    }
}

void iconButton(int* args) {
    SDL_Log("%d, %d", args[0], args[1]);

}

static bool fullscreen = false;
static float prevScale;

void dropdownButton(int* args) {
    if(args[0] == 11) { //fullscreen button
        fullscreen = !fullscreen;
        SDL_SetWindowFullscreen(window, fullscreen);
        if(fullscreen) {
            SDL_Rect dispBounds = getFullscreenResolution(window);
            prevScale = SCALE;
            SCALE = dispBounds.w / (float)WINDOW_WIDTH + 0.001f; //slightly fixes artifacts on non integer resolutions
        } else {
            SCALE = prevScale;
            SDL_SetWindowSize(window, WINDOW_WIDTH*SCALE, WINDOW_HEIGHT*SCALE);
        }
        SDL_SetRenderScale(renderer, SCALE, SCALE);
    }
}

void createGUIButtons() {
    //add title bar buttons
    createTextButton("File",(SDL_FRect) {2,21,32,20}, &titleBarButton, (int[]){compArray.used,0}, TITLEBAR, true);
    createTextButton("Edit",(SDL_FRect) {36,21,29,20}, &titleBarButton, (int[]){compArray.used,0}, TITLEBAR, true);
    createTextButton("View",(SDL_FRect) {71,21,39,20}, &titleBarButton, (int[]){compArray.used,0}, TITLEBAR, true);
    createTextButton("Help",(SDL_FRect) {114,21,34,20}, &titleBarButton, (int[]){compArray.used,0}, TITLEBAR, true);

    //icon buttons
    //something is wrong here but it looks okay
    createIconButton((SDL_FRect) {14,46,0,0}, (SDL_FRect) {0,0,16,16}, &iconButton, (int[]){compArray.used,0});
    createIconButton((SDL_FRect) {31,46,0,0}, (SDL_FRect) {16,0,16,16}, &iconButton, (int[]){compArray.used,0});
    createIconButton((SDL_FRect) {49,46,0,0}, (SDL_FRect) {32,0,16,16}, &iconButton, (int[]){compArray.used,0});
    createIconButton((SDL_FRect) {80,46,0,0}, (SDL_FRect) {48,0,16,16}, &iconButton, (int[]){compArray.used,0});
    createIconButton((SDL_FRect) {97,46,0,0}, (SDL_FRect) {64,0,16,16}, &iconButton, (int[]){compArray.used,0});
    createIconButton((SDL_FRect) {116,46,0,0}, (SDL_FRect) {80,0,16,16}, &iconButton, (int[]){compArray.used,0});
    createIconButton((SDL_FRect) {146,46,0,0}, (SDL_FRect) {94,0,16,16}, &iconButton, (int[]){compArray.used,0});

    //dropdowns added last because im too lazy to make them layer correctly
    //dropdown buttons
    createTextButton("Fullscreen",(SDL_FRect) {5,44,32,20}, &dropdownButton, (int[]){compArray.used,0}, DROPDOWN, true);
    createTextButton("Resolution",(SDL_FRect) {5,61,32,20}, &dropdownButton, (int[]){compArray.used,0}, DROPDOWN, false);
    setButtonDropdown(compArray.array[0]->element.button, compArray.used);
    createDropDown((int[]){11, 12}, 2, 5, 43);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("coppersure", "1.0", "com.vaporgame.coppersure");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    SDL_WindowFlags windowFlags = SDL_WINDOW_BORDERLESS;
    for(int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        if(SDL_strcmp("vulkan", SDL_GetRenderDriver(i)) == 0) {
            windowFlags = windowFlags | SDL_WINDOW_VULKAN;
        }
    }

    if (!SDL_CreateWindowAndRenderer("coppersure", WINDOW_WIDTH*SCALE, WINDOW_HEIGHT*SCALE, windowFlags, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetWindowHitTest(window, hitTestCallback, NULL);

    if (!SDL_SetWindowResizable(window, false)) {
        SDL_Log("error setting resize %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }  
    
    // if (!SDL_SetWindowBordered(window, false)) {
    //     SDL_Log("error setting bordered %s", SDL_GetError());
    //     return SDL_APP_FAILURE;
    // }  

    if (!TTF_Init()) {
        SDL_Log("TTF_Init Error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    SDL_SetRenderScale(renderer, SCALE, SCALE);
    initGUI(renderer, &compArray);

    createGUIButtons();

    return SDL_APP_CONTINUE;
}

bool getButtonCollision(SDL_FRect rect, SDL_MouseMotionEvent motion) {
    float butX = rect.x * SCALE, butY = rect.y * SCALE, butW = rect.w * SCALE, butH = rect.h * SCALE;
    return (motion.x > butX && motion.x < butX+butW && motion.y > butY && motion.y < butY+butH);
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;

    } else if (event->type == SDL_EVENT_MOUSE_MOTION) {
        for(size_t i = 0; i < compArray.used; i++) {
            if (compArray.array[i]->type != 0) {continue;}
            Button* button = compArray.array[i]->element.button;
            if (!button->_.active || button->state == PRESSED) {continue;}

            if (getButtonCollision(button->_.dstRect, event->motion)) {
                button->state = HOVERED;
            } else {
                button->state = IDLE;
            }
        }
        
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        for(size_t i = 0; i < compArray.used; i++) {
            if (compArray.array[i]->type != 0) {continue;}
            Button* button = compArray.array[i]->element.button;
            if (!button->_.active) {continue;}

            if(event->motion.x > 619 && event->motion.x < 635 && event->motion.y > 5 && event->motion.y < 19) {
                SDL_Event quit;
                SDL_zero(quit);
                quit.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quit);
            }

            if (getButtonCollision(button->_.dstRect, event->motion)) {
                int callback[2] = {button->callback_data[0], 0};
                
                if (button->state == HOVERED) {
                    button->state = PRESSED;
                    callback[1] = 1;
                } else {
                    button->state = HOVERED;
                }

                setButtonCallback(button, callback);
                button->callback(button->callback_data);
            }
        }
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        for(size_t i = 0; i < compArray.used; i++) {
            if (compArray.array[i]->type != 0) {continue;}
            Button* button = compArray.array[i]->element.button;
            if (!button->_.active || button->toggle) {continue;}
            
            if(button->state == PRESSED) {
                
                button->state = HOVERED;
                int callback[2] = {button->callback_data[0], 0};
                setButtonCallback(button, callback);
                button->callback(button->callback_data);
            }
        }
    }
    return SDL_APP_CONTINUE;
}

static Uint64 prev = 0;
SDL_AppResult SDL_AppIterate(void *appstate) {   
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawGUI();
    SDL_RenderPresent(renderer);

    //cap the framerate
    Uint64 now = SDL_GetTicksNS();
    if (SDL_NS_PER_SECOND / FRAMERATE >= now - prev) {
        SDL_DelayNS((SDL_NS_PER_SECOND / FRAMERATE) - (now - prev));
    }
    prev = now;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {   
    cleanupGUI();
    componentArrayFree();
}
