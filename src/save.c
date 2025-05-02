#include "save.h"
#include <stdio.h>
#include <stdlib.h>

//TODO: implement exporting and importing YAML in the future

SaveFile* loadSavefile(char* filename) {
    FILE *save = fopen(filename, "rb");
    if(save == NULL) {
        printf("Failed to open savefile: %s", filename);
        return NULL;
    }

    SaveFile* saveFile = malloc(sizeof(SaveFile));

    size_t bytesRead = fread(saveFile, sizeof(SaveFile), 1, save);
    if(bytesRead != 1) {
        printf("Error reading from file: %s", filename);
        fclose(save);
        return NULL;
    }

    return saveFile;
}

int writeSavefile(char* filename, SaveFile* data) {
    FILE *save = fopen(filename, "wb");
    if(save == NULL) {
        printf("Failed to open savefile: %s", filename);
        return 1;
    }

    size_t bytesWritten = fwrite(data, sizeof(SaveFile), 1, save);
    if(bytesWritten != 1) {
        printf("Error writing to file: %s", filename);
        fclose(save);
        return 1;
    }

    return 0;
}

void closeSavefule(SaveFile* data) {
    free(data->components);
    free(data);
}
