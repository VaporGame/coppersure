#ifndef COMP_H
#define COMP_H
#include <stdbool.h>
#include <SDL3/SDL.h>

typedef struct {
    bool active;
    SDL_FRect dstRect;
} GUIComponent;

//not sure if level i/o should be its own thing, or a component with custom data
typedef enum { //subject to change
    DIODE = 0,
    GND = 1, //TEMPORARY REMOVE
    VCC = 2, //TEMPORARY REMOVE
} ComponentType;

typedef struct {
    int Id;
    float state; //this will be the "voltage" on the line. I dont think i wanna use a float for this in the future
} Connection;

typedef struct {
    ComponentType type;
    float position[2];
    float rotation;
    uint8_t state;

    //making there connections instead of ints breaks something somwhere i think :p
    Connection *inputs; //stores the IDs of components connected to inputs
    size_t inputCount; //might be useless
    Connection * outputs; //stores the IDs of components connected to outputs
    size_t outputCount; //might be useless

    void *data;
    size_t dataSize;
} GameComponent; 

GameComponent *createGameComponent(ComponentType type, float position[2], float rotation, void *data);

#endif