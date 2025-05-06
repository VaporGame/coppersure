#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "editor.h"

// static void writeString(FILE* f, const char* str) {
//     uint32_t len = strnlen(str, UINT32_MAX);
// }

// int writeLevel(char *filename, Level* levelPtr, LevelFileHeader *header) {
//     char *path = NULL;
//     asprintf(&path, "%s%s", LEVEL_FOLDER_PATH, filename); //this wont work on windows

//     FILE *file = fopen(path, "wb");
//     if (file == NULL) {return 1;}

    // LevelFileHeader header;
    // if (fread(&header, sizeof(header), 1, file) != 1) {return 1;} //read the header
    // if (header.magic != MAGIC) {return 2;} //2-not a coppersure level



    // *levelPtr = malloc(sizeof(Level)); //allocate memory for the level struct
    // Level* level = *levelPtr;

    // level->name = readString(file); //read level name
    
    // uint32_t dialogCount;
    // if(fread(&dialogCount, sizeof(dialogCount), 1, file) != 1) {return 1;} //read the dialog count
    // level->dialogCount = dialogCount;
    // level->dialog = (char **)malloc(dialogCount * sizeof(char *));

    // for (uint32_t i = 0; i < dialogCount; i++) {
    //     level->dialog[i] = readString(file);
    // }

    // uint32_t pairCount;
    // if(fread(&pairCount, sizeof(pairCount), 1, file) != 1) {return 1;} //read the pair count
    // level->levelInputs = (int **)malloc(pairCount * sizeof(int *));
    // level->levelOutputs = (int **)malloc(pairCount * sizeof(int *));
    // level->pairCount = pairCount;

    // uint32_t inputAmount, outputAmount;
    // if(fread(&inputAmount, sizeof(inputAmount), 1, file) != 1) {return 1;}
    // if(fread(&outputAmount, sizeof(outputAmount), 1, file) != 1) {return 1;}

    // level->levelInputs = (uint8_t **) malloc(pairCount * inputAmount * sizeof(uint8_t));
    // level->levelOutputs = (uint8_t **) malloc(pairCount * outputAmount * sizeof(uint8_t));
    
    // for (uint32_t i = 0; i < pairCount; i++) {
    //     //loop that gets all of the inputs
    //     for(uint32_t j = 0; j < inputAmount; j++) {
    //         fread(&(level->levelInputs[i][j]), sizeof(uint8_t), 1, file); //i think this is correct
    //     }
    //     for(uint32_t j = 0; j < outputAmount; j++) {
    //         fread(&(level->levelOutputs[i][j]), sizeof(uint8_t), 1, file);
    //     }
    // }
    // //i dont know if storing it in pairs like this with a nested for is better than having inputs and outputs separate, with 2 nested for loops
    // fclose(file);
    // return 0;
// }