#ifndef COMP_H
#define COMP_H
#include <SDL3/SDL.h>

typedef struct {
    bool active;
    SDL_FRect dstRect;
} Component;

typedef struct {

} l1Component;

typedef union {
    l1Component* layer1Component;
} GameElements;

typedef struct {
    int type;
    GameElements element;
} GameElement;

typedef struct {
    size_t used, len;
    GameElement** array;
} GameComponentArray;

int createLayerOneComponent(const int type, SDL_FRect dstRect, float rotation);

#endif