#ifndef WINDOW
#define WINDOW
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

typedef struct Window Window;
Window *Window_create(int width, int height, const char *title);
void Window_destroy(Window *window);
int Window_getWidth(Window *window);
int Window_getHeight(Window *window);
char *Window_getTitle(Window *window);
SDL_Window *Window_getWindow(Window *window);
SDL_Renderer *Window_getRenderer(Window *window);
void Window_setTitle(Window *window, const char *title);

#endif
