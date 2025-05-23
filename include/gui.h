#ifndef GUI_H
#define GUI_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "component.h"

typedef enum {
    TITLEBAR,
    DROPDOWN,
    ICON
} ButtonStyle;

typedef struct {
    GUIComponent _;
    char* text;
    SDL_FRect clipRect;
    enum {IDLE, HOVERED, PRESSED} state;
    ButtonStyle style;
    void (*callback)(int* data);
    int *callback_data;
    int dropdownIdx;
    bool toggle;
} Button;

typedef struct {
    GUIComponent _;
    int *buttons;
    size_t buttonsCount;
} Dropdown;

typedef union {
    Button* button;
    Dropdown* dropdown;
} GUIElements;

typedef struct {
    int type;
    GUIElements element;
} GUIElement;

typedef struct {
    size_t used, len;
    GUIElement** array;
} GUIComponentArray;

void initGUI(SDL_Renderer* renderer, GUIComponentArray* componentArray);
void cleanupGUI();

void drawGUI();

// SDL_Texture* bmpToTexture(char* path);

int drawText(const char* string, SDL_Color color, int x, int y, int size);
int drawTextBold(const char* string, SDL_Color color, int x, int y, int size);
// int drawTextShaded(const char* string, SDL_Color color, SDL_Color bgColor, int x, int y, int size);
// int drawTextWrapped(char* string, SDL_Color color, int x, int y, int size, int wrapLength);
SDL_FRect getTextSize(const char* string, int size);

// int createIconButton(SDL_FRect rect, SDL_FRect clipRect, void (*callback)(int* data), int* data);
int createIconButton(int x, int y, SDL_FRect clipRect, void (*callback)(int* data), int* data);
int createTextButton(char* text, SDL_FRect rect, void (*callback)(int* data), int* data, ButtonStyle style, bool toggle);
void setButtonDropdown(Button* button, int idx);
void setButtonCallback(Button* buton, int* data);
int createDropDown(const int* buttons, size_t size, int x, int y);
// bool drawButton(Button* button);

// void componentArrayInit(size_t initialSize);
void componentArrayFree();
bool componentArrayAppend(GUIElement* element);

SDL_Rect getFullscreenResolution(SDL_Window* window);

#endif