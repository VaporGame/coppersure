#include "gui.h"
#include "hashmap.h"
#include <string.h>

Cache* textCache = NULL;

static char* assetPath = "../src/assets/";
static SDL_Renderer* renderer = NULL;
static SDL_Texture* windowTexture = NULL;
static SDL_Texture* spriteSheet = NULL;
static TTF_Font* w95f = NULL;

#define COMPONENT_ARRAY_INITIAL_SIZE 13
static ComponentArray* compArray;

static SDL_Color black = {0,0,0,255};
// static SDL_Color darkGray = {128,128,128,255};
// static SDL_Color gray = {192,192,192,255};
static SDL_Color white = {255,255,255,255};

static void componentArrayInit() {
    compArray->array = SDL_malloc(COMPONENT_ARRAY_INITIAL_SIZE * sizeof(GUIElement));
    compArray->used = 0;
    compArray->len = COMPONENT_ARRAY_INITIAL_SIZE;
}

static SDL_Texture* bmpToTexture(char* name) {
    char *bmp_path = NULL;

    SDL_asprintf(&bmp_path, "%s%s", assetPath, name);

    SDL_Surface* surface = SDL_LoadBMP(bmp_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return NULL;
    }

    SDL_free(bmp_path);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return NULL;
    }

    SDL_DestroySurface(surface);
    return texture;
}

void initGUI(SDL_Renderer* render, ComponentArray* componentArray) {
    textCache = createCache();
    compArray = componentArray;

    componentArrayInit();

    SDL_asprintf(&assetPath, "%s%s", SDL_GetBasePath(), assetPath);
    renderer = render;

    //load w95 font
    char* font_path = NULL;
    SDL_asprintf(&font_path, "%sW95FA.ttf", assetPath);
    w95f = TTF_OpenFont(font_path, 100);

    SDL_free(font_path);

    if (w95f == NULL) {
        SDL_Log("TTF_OpenFont() Error: %s", SDL_GetError());
    }

    //load textures
    windowTexture = bmpToTexture("window.bmp");
    SDL_SetTextureScaleMode(windowTexture, SDL_SCALEMODE_NEAREST);
    spriteSheet = bmpToTexture("spritesheet.bmp");
    SDL_SetTextureScaleMode(spriteSheet, SDL_SCALEMODE_NEAREST);
}

void cleanupGUI() {
    freeTextureCache(textCache);
    TTF_CloseFont(w95f);
    SDL_DestroyTexture(windowTexture);
    SDL_DestroyTexture(spriteSheet);
}

static bool drawButton(Button* button, int max) {
    if (!button->_.active) return false;

    SDL_FRect dstRect = button->_.dstRect;
    SDL_FRect bg;
    if(button->style != ICON) {
        bg = (SDL_FRect){dstRect.x + 2, dstRect.y + 1, getTextSize(button->text, 12).w + 13, 18};
    } else {
        bg = (SDL_FRect){dstRect.x, dstRect.y, 16, 16};
    }
    // Common border points
    SDL_FPoint borderPoints[12] = {
        {dstRect.x + 1, dstRect.y + 19},
        {dstRect.x + 1, dstRect.y},
        {dstRect.x + bg.w, dstRect.y},
        {dstRect.x + bg.w + 1, dstRect.y},
        {dstRect.x + bg.w + 1, dstRect.y + 19},
        {dstRect.x + 2, dstRect.y + 19},
        {dstRect.x - 1, dstRect.y + 16}, 
        {dstRect.x - 1, dstRect.y - 1},
        {dstRect.x + 15, dstRect.y - 1},
        {dstRect.x + 16, dstRect.y - 1},
        {dstRect.x + 16, dstRect.y + 16},
        {dstRect.x, dstRect.y + 16}
    };

    SDL_FRect rect = dstRect;
    int color1 = 128, color2 = 255;
    if (button->state == PRESSED && button->style != DROPDOWN) { //this should be fine since im not planning for any more button styles
        rect.x++;
        rect.y++;
        color1 = 255;
        color2 = 128;
    }

    switch (button->style) {
        case TITLEBAR: {
            if (button->state == IDLE) {
                bg = (SDL_FRect){dstRect.x + 1, dstRect.y, getTextSize(button->text, 12).w + 15, 20};
            } else {
                SDL_SetRenderDrawColor(renderer, color1, color1, color1, 255);
                SDL_RenderLines(renderer, borderPoints, 3);
                SDL_SetRenderDrawColor(renderer, color2, color2, color2, 255);
                SDL_RenderLines(renderer, borderPoints + 3, 3);
            }

            SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
            bg.w--;
            SDL_RenderFillRect(renderer, &bg);
            
            drawText(button->text, black, rect.x + 9, rect.y + 5, 12);
            break;
        }

        case ICON: {
            SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
            SDL_RenderFillRect(renderer, &bg);

            SDL_RenderTexture(renderer, spriteSheet, &(button->clipRect), &rect);

            if (button->state != IDLE) {
                SDL_SetRenderDrawColor(renderer, color1, color1, color1, 255);
                SDL_RenderLines(renderer, borderPoints + 6, 3);
                SDL_SetRenderDrawColor(renderer, color2, color2, color2, 255);
                SDL_RenderLines(renderer, borderPoints + 9, 3);
            }
            break;
        }

        case DROPDOWN: {
            SDL_Color color = white;
            if (button->state == IDLE) {
                color = black;
            } else {
                bg = (SDL_FRect){dstRect.x + 1, dstRect.y, max, 17};
                SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
                SDL_RenderFillRect(renderer, &bg);
            }

            drawText(button->text, color, dstRect.x + 22, dstRect.y + 3, 12);
            break;
        }
    }
    return true;
}

static bool drawDropdown(Dropdown* dropdown) {
    if(dropdown->_.active == false) {return false;}

    SDL_FRect dstRect = dropdown->_.dstRect;

    SDL_FPoint line1[3] = {
        {dstRect.x-2, dstRect.y-2 + dstRect.h - 2},
        {dstRect.x-2, dstRect.y-2},
        {dstRect.x-2 + dstRect.w - 1, dstRect.y-2}
    };
    SDL_FPoint line2[3] = {
        {dstRect.x-2, dstRect.y-2 + dstRect.h - 1},
        {dstRect.x-2 + dstRect.w- 1, dstRect.y-2 + dstRect.h - 1},
        {dstRect.x-2 + dstRect.w- 1, dstRect.y-2}
    };
    SDL_FRect bg = {dstRect.x, dstRect.y, dstRect.w - 4, dstRect.h - 4};



    SDL_SetRenderDrawColor(renderer,192,192,192,255);
    SDL_RenderLines(renderer, line1, 3);
    SDL_RenderFillRect(renderer, &bg);
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderLines(renderer, line2, 3);

    SDL_FPoint line3[3] = {
        {dstRect.x-1, dstRect.y-1 + dstRect.h - 4},
        {dstRect.x-1, dstRect.y-1},
        {dstRect.x-1 + dstRect.w - 3, dstRect.y-1}
    };
    SDL_FPoint line4[3] = {
        {dstRect.x-1, dstRect.y-1 + dstRect.h -3},
        {dstRect.x-1 + dstRect.w- 3, dstRect.y-1 + dstRect.h - 3},
        {dstRect.x-1 + dstRect.w- 3, dstRect.y}
    };

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderLines(renderer, line3, 3);
    SDL_SetRenderDrawColor(renderer,128,128,128,255);
    SDL_RenderLines(renderer, line4, 3);

    for(size_t j = 0; j < dropdown->buttonsCount; j++) {
        drawButton(compArray->array[dropdown->buttons[j]]->element.button, dstRect.w - 6);
    }
    return true;
}

void drawGUI() {
    //draw "window"
    SDL_RenderTexture(renderer, windowTexture, NULL, NULL);

    drawTextBold("Coppersure", white, 4, 6, 12);

    for(size_t i = 0; i < compArray->used; i++) {
        switch(compArray->array[i]->type) {
            case 0:
                drawButton(compArray->array[i]->element.button, 0);
                break;
            case 1: 
                drawDropdown(compArray->array[i]->element.dropdown);
                break;
        }
    }
}

static TextureCacheEntry* createText(const char* string, SDL_Color color, int size) {
    char *key = NULL;
    SDL_asprintf(&key, "%s%c", string, (char)color.r);
    TextureCacheEntry* cachedTexture = cacheGet(textCache, key);

    if (cachedTexture != NULL) {
        SDL_free(key);
        return cachedTexture;
    } 

    TTF_SetFontSize(w95f, size);
    SDL_Surface* surface = TTF_RenderText_Solid(w95f, string, 0, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(surface);

    float w = 0, h = 0;
    SDL_GetTextureSize(texture, &w, &h);
    TextureCacheEntry* test = SDL_malloc(sizeof(TextureCacheEntry));
    test->w = w;
    test->h = h;
    test->texture = texture;
    cacheSet(textCache, key, test);
    SDL_free(key);
    return test;
}

int drawText(const char* string, SDL_Color color, int x, int y, int size) {
    TextureCacheEntry* entry = createText(string, color, size);

    SDL_FRect rect = {x, y, entry->w, entry->h};
    SDL_RenderTexture(renderer, entry->texture, NULL, &rect);
    return entry->w;
}

int drawTextBold(const char* string, SDL_Color color, int x, int y, int size) {
    TextureCacheEntry* entry = createText(string, color, size);

    SDL_FRect rect = {x, y, entry->w, entry->h};
    SDL_RenderTexture(renderer, entry->texture, NULL, &rect);
    rect.x++;
    SDL_RenderTexture(renderer, entry->texture, NULL, &rect);
    return entry->w;
}

SDL_FRect getTextSize(const char* string, int size) {
    TextureCacheEntry* entry = createText(string, black, size);

    return (SDL_FRect) {0,0,entry->w,entry->h};
}

GUIElement* createIconButton(SDL_FRect rect, SDL_FRect clipRect, void (*callback)(int* data), int* data) {
    Button* button = SDL_malloc(sizeof(Button));

    button->_.dstRect = rect;
    button->_.dstRect.w = button->_.dstRect.h = 16;
    button->clipRect=clipRect;
    button->callback = callback;
    button->state = IDLE;
    button->style = ICON;
    button->_.active = true;
    size_t dataSize = sizeof(data);
    button->callback_data = SDL_malloc(dataSize);
    if(button->callback_data == NULL) {
        SDL_Log("Error allocating memory for button callback data");
        return NULL;
    }
    memcpy(button->callback_data, data, dataSize);

    GUIElement* elem = SDL_malloc(sizeof(GUIElement));
    elem->type = 0;
    elem->element.button = button;
    componentArrayAppend(elem);
    return elem;
}

GUIElement* createTextButton(char* text, SDL_FRect rect, void (*callback)(int* data), int* data, ButtonStyle style) {
    Button* button = SDL_malloc(sizeof(Button));

    button->text = text;
    if(style == DROPDOWN) {
        button->_.dstRect = (SDL_FRect) {rect.x, rect.y, getTextSize(text, 12).w + 38, 17};
    } else {
        button->_.dstRect = (SDL_FRect) {rect.x, rect.y, getTextSize(text, 12).w + 15, 20};
    }
    button->callback = callback;
    button->state = IDLE;
    button->_.active = true;
    button->style = style;
    size_t dataSize = sizeof(data);
    button->callback_data = SDL_malloc(dataSize);
    if(button->callback_data == NULL) {
        SDL_Log("Error allocating memory for button callback data");
        return NULL;
    }
    memcpy(button->callback_data, data, dataSize);
    button->dropdownIdx = -1;

    GUIElement* elem = SDL_malloc(sizeof(GUIElement));
    elem->type = 0;
    elem->element.button = button;
    componentArrayAppend(elem);
    return elem;
}

void setButtonDropdown(Button* button, int idx) {
    button->dropdownIdx = idx;
}

void setButtonCallback(Button* button, int* data) {
    memcpy(button->callback_data, data, sizeof(int) * 2);
}

GUIElement* createDropDown(const int *buttons, size_t size, int x, int y) {
    Dropdown* dropdown = SDL_malloc(sizeof(Dropdown));
    dropdown->_.active = false;
    dropdown->_.dstRect.x = x;
    dropdown->_.dstRect.y = y;
    dropdown->buttons = SDL_malloc(sizeof(*buttons) * size);
    memcpy(dropdown->buttons, buttons, sizeof(*buttons) * size);
    dropdown->buttonsCount = size;

    int max = 0;
    for(size_t i = 0; i < size; i++) {
        Button* button = compArray->array[buttons[i]]->element.button;
        button->_.active = false;
        if(button->_.dstRect.w > max) {
            max = button->_.dstRect.w;
        }
    }
    dropdown->_.dstRect.w = max + 6;
    dropdown->_.dstRect.h = size*17 + 6;

    GUIElement* elem = SDL_malloc(sizeof(GUIElement));
    elem->type = 1;
    elem->element.dropdown = dropdown;
    componentArrayAppend(elem);
    return elem;
}

void componentArrayFree() {
    for(size_t i = 0; i < compArray->used; i++) {
        if(compArray->array[i]->type == 0) {
            SDL_free(compArray->array[i]->element.button->callback_data);
            SDL_free(compArray->array[i]->element.button);
        } else if (compArray->array[i]->type == 1) {
            SDL_free(compArray->array[i]->element.dropdown->buttons);
            SDL_free(compArray->array[i]->element.dropdown);
        }
        SDL_free(compArray->array[i]);
    }
    SDL_free(compArray->array);
    // compArray->array = NULL;
    // compArray->len = compArray->used = 0;
}

bool componentArrayAppend(GUIElement* element) {
    if (compArray->used >= compArray->len / 0.7) {
        compArray->len *= 1.5;
        compArray->array = SDL_realloc(compArray->array, compArray->len * sizeof(GUIElement));
        if (compArray->array == NULL) {
            componentArrayFree();
            SDL_Log("Error reallocating component array");
            return false;
        }
    }   

    compArray->array[compArray->used++] = element;
    return true;
}

SDL_Rect getFullscreenResolution(SDL_Window* window) {
    SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
    SDL_Rect bounds;
    SDL_GetDisplayBounds(displayID, &bounds);
    return bounds;
}