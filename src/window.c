#include "window.h"

typedef struct Window {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int width;
	int height;
	char title[20];
} Window;

void Window_init(Window *window, int width, int height, char *title) {
	window->window = NULL;
	window->renderer = NULL;
	window->width = width;
	window->height = height;
	strcpy(window->title, title);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
		printf("SDL_Init Error: %s\n", SDL_GetError());

	window->window = SDL_CreateWindow(
		title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window->width,
		window->height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (window->window == NULL)
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());

	window->renderer = SDL_CreateRenderer(window->window, -1,
										  SDL_RENDERER_ACCELERATED |
											  SDL_RENDERER_PRESENTVSYNC);

	if (window->renderer == NULL)
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());

	if (SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND) != 0)
		printf("SDL_SetRenderDrawBlendMode Error: %s\n", SDL_GetError());

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		printf("Mix_OpenAudio Error: %s", SDL_GetError());
}

Window *Window_create(int width, int height, char title[20]) {
	Window *window = (Window *)malloc(sizeof(Window));
	Window_init(window, width, height, title);
	return window;
}

void Window_reset(Window *window) {}

void Window_destroy(Window *window) {
	if (window) {
		Mix_CloseAudio();
		SDL_DestroyRenderer(window->renderer);
		SDL_DestroyWindow(window->window);
		SDL_Quit();
		Window_reset(window);
		free(window);
	}
}

int Window_getWidth(Window *window) { return window->width; }

int Window_getHeight(Window *window) { return window->height; }

char *Window_getTitle(Window *window) { return window->title; }

SDL_Window *Window_getWindow(Window *window) { return window->window; }

SDL_Renderer *Window_getRenderer(Window *window) { return window->renderer; }
