#ifndef LEVEL_H
#define LEVEL_H
#include <string.h>
#include <stdint.h>

//level format
//magic, name, version,
//dialog and dialog count
//array of inputs and their corresponding correct outputs.

#define MAGIC 0x43535245 //CSRE

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint64_t dataSize;
} LevelFileHeader;

typedef struct {
    char *name;

    char** dialog;
    size_t dialogCount;

    int **levelInputs;
    size_t InputAmount; //how many inputs does the design have
    int **levelOutputs;
    size_t OutputAmount; //how many outputs does the design have
    
    size_t pairCount; //how many input-output pairs (test cases) does the level have
    //the levelInputs and levelOutput arrays are pairCount long
    //each of the indices have the same amount of inputs/outputs
    //int levelInputs[pairCount][InputAmount];
    //int levelOutputs[pairCount][OutputAmount];
} Level;

//TODO: implement loading levels and functions for:
//getting next dialogue
//getting next i/o pair
//

int loadLevel(char *filename, Level** levelPtr);

#endif