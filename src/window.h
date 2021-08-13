#ifndef WINDOW
#define WINDOW
#include <SDL2/SDL.h>

typedef struct Window Window;
struct Window *Window_create(int width, int height, char *title);
void Window_destroy(struct Window *window);
int Window_getWidth(struct Window *window);
int Window_getHeight(struct Window *window);
char *Window_getTitle(struct Window *window);
SDL_Window *Window_getWindow(struct Window *window);
SDL_Renderer *Window_getRenderer(struct Window *window);

#endif
