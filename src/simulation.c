#include "simulation.h"
#include <stdio.h>

Game *initSimulation(); //maybe will be needed
void stopSimulation();
void tickSimulation(Game *game) {
    for(size_t i = 0; i < game->componentCount; i++) {
        GameComponent component = game->components[i];
        switch(component.type) {
            case DIODE:
                    //pass input to output.
                    for(size_t j = 0; j < component.outputCount; j++) {
                        game->components[component.outputs[j].Id].inputs[component.outputs[j].Id].state = component.inputs[0].state;
                        //addressing the input by the Id is pretty ineficient, but i think i can get away with it
                    }
                break;

            case GND: //TEMP

                break;

            case VCC: //TEMP

                break;

            default:
                printf("Unknown component ID: %d", component.type);
                break;
        }
    }
}
void freeSimulation(Game *game);