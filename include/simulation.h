#ifndef SIMULATION_H
#define SIMULATION_H
#include <stdint.h>
#include "component.h"

typedef enum GameState {
    STOPPED = 0,
    RUNNING = 1,
    PAUSED = 2
} GameState;

typedef struct Game { //dont know the best way to name this
    GameState gameState;
    uint64_t tick; //tick count

    GameComponent *components;
    size_t componentCount;
} Game;

Game *initSimulation(); //maybe will be needed
void stopSimulation();
void tickSimulation(Game *game);
void freeSimulation(Game *game);

#endif