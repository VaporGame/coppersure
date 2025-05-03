#include "level.h"
#include <stdio.h>
#include <stdlib.h>

//the level folder path will be ../ right now because in the final game it should be in the same directory as the executable
#define LEVEL_FOLDER_PATH "../levels/"

static char* readString(FILE* f) {
    uint32_t len;
    fread(&len, sizeof(uint32_t), 1, f);
    char* str = malloc(len + 1);
    fread(str, len, 1, f);
    str[len] = '\0';
    return str;
}

int loadLevel(char *filename, Level** levelPtr) {
    char *path = NULL;
    asprintf(&path, "%s%s", LEVEL_FOLDER_PATH, filename); //this wont work on windows

    FILE *file = fopen(path, "rb");
    if (file == NULL) {return 1;}

    LevelFileHeader header;
    if (fread(&header, sizeof(header), 1, file) != 1) {return 1;}
    if (header.magic != MAGIC) {return 2;} //2-not a coppersure level

    *levelPtr = malloc(sizeof(Level));
    Level* level = *levelPtr;

    level->name = readString(file);
    
    uint32_t dialogCount;
    if(fread(&dialogCount, sizeof(dialogCount), 1, file) != 1) {return 1;}
    //TODO: finish this
}