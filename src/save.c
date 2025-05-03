#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

//TODO: implement exporting and importing YAML in the future

//this is all very bad
//TODO: fix this after implementing level format.

// static char *getSavefilePath() { //this is probably bad
//     #ifdef _WIN32
//         const char *username = getenv("USERNAME");
//     #else
//         const char *username = getenv("USER");
//     #endif 

//     char *path = NULL;

//     #ifdef _WIN32
//         asprintf(&path, "%s%s%s", "C:\\Users\\", username, "\\Documents\\coppersure\\");
//     #elif defined(__APPLE__)
//         asprintf(&path, "%s%s%s", "/Users/", username, "/Documents/coppersure/");
//     #else // Linux and other Unix-like systems
//         asprintf(&path, "%s%s%s", "/home/", username, "/Documents/coppersure/");
//     #endif

//     if(path == NULL) {
//         printf("Error getting Documents directory\n");
//         return NULL;
//     }

//     DIR* dir = opendir(path);
//     if(dir) {
//         closedir(dir);
//     } else if(ENOENT == errno) {
//         mkdir(path, 0700);
//     } else {
//         printf("What the fuck did you do");
//         return NULL;
//     }

//     return path;
// }

// static char* DocumentsPath = NULL;

// SaveFile* loadSavefile(char* filename) {
//     if(DocumentsPath == NULL) {
//         DocumentsPath = getSavefilePath();
//     }

//     char* path = NULL;
//     asprintf(&path, "%s%s", DocumentsPath, filename);

//     FILE *save = fopen(path, "rb");
//     if(save == NULL) {
//         printf("Failed to open savefile: %s when reading\n", path);
//         return NULL;
//     }

//     SaveFile* saveFile = malloc(sizeof(SaveFile));

//     size_t bytesRead = fread(saveFile, sizeof(SaveFile), 1, save);
//     fclose(save);
//     if(bytesRead != 1) {
//         printf("Error reading from file: %s\n", path);
//         return NULL;
//     }
//     return saveFile;
// }

// int writeSavefile(char* filename, SaveFile* data) {
//     if(DocumentsPath == NULL) {
//         DocumentsPath = getSavefilePath();
//     }

//     char* path = NULL;
//     asprintf(&path, "%s%s", DocumentsPath, filename);

//     FILE *save = fopen(path, "wb");
//     if(save == NULL) {
//         printf("Failed to open savefile: %s when writing\n", path);
//         return 1;
//     }

//     size_t bytesWritten = fwrite(data, sizeof(SaveFile), 1, save);
//     fclose(save);
//     if(bytesWritten != 1) {
//         printf("Error writing to file: %s\n", path);
//         return 1;
//     }
//     return 0;
// }

// void closeSavefile(SaveFile* data) {
//     free(data->components);
//     free(data);
// }
