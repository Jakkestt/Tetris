#include "window.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define M 20
#define N 10
#define SIZE 40

int field[M][N] = {0};

int figures[7][4] = {
	1, 3, 5, 7, // CYAN
	2, 4, 5, 7, // GREEN
	3, 5, 4, 6, // RED
	3, 5, 4, 7, // PURPLE
	2, 3, 5, 7, // ORANGE
	3, 5, 7, 6, // BLUE
	2, 3, 4, 5, // YELLOW
};

typedef struct {
	int x, y;
} Point;

typedef struct {
	int x, y, width, height;
	bool mouseOver;
	char text[10];
} Button;

Point a[4] = {0}, b[4] = {0}, c[4] = {0}, d[4] = {0};

bool go = true;

bool check(Point[], size_t);

void drawUI(SDL_Renderer *, Button, TTF_Font *);

void draw(SDL_Renderer *, Point, int, int);

void drawText(SDL_Renderer *, TTF_Font *, int, int, const char *, int, int);

void InvCheck();

int main() {
	Window *window = Window_create(N * SIZE, M * SIZE, "Tetris");
	time_t t;

	srand((unsigned)time(&t));

	// Initialize new variables

	int n = 0;
	int dx = 0;
	bool rotate = 0;
	unsigned int timer;
	float delay = 0.5;
	int score = 0;
	int colorNum = 1;
	int switchBlock = -1;
	bool spawn = false;
	bool switched = false;
	Mix_Music *theme;
	Mix_Chunk *move;
	bool pause = false;
	int volume = 10;
	int frames = 0;
	double starttime = 0;
	bool first = true;
	float fps = 0.0f;
	char title[3];
	Button buttons[3];
	buttons[0] =
		(Button){10 * 40 / 2, 20 * 40 / 2 - 55, 260, 50, false, "Start"};
	buttons[1] = (Button){10 * 40 / 2, 20 * 40 / 2, 260, 50, false, "Quit"};
	buttons[2] =
		(Button){10 * 40 / 2, 20 * 40 / 2 - 55, 260, 50, false, "Continue"};
	int mouseX, mouseY;
	Uint32 mouseButtons;
	TTF_Font *font;
	bool begun = false;
	bool click = false;

	// Load Music and Sound Effects and allocate channels

	Mix_AllocateChannels(1);

	move = Mix_LoadWAV("resources/sfx/move.wav");
	if (!move)
		printf("Mix_LoadWAV Error: %s\n", Mix_GetError());

	theme = Mix_LoadMUS("resources/audio/main.wav");
	if (!theme)
		printf("Mix_LoadMUS Error: %s\n", Mix_GetError());

	font = TTF_OpenFont("resources/fonts/PixelEmulator.ttf", 22);

	Mix_Volume(-1, volume);
	Mix_VolumeMusic(volume);
	bool isRunning = true;
	SDL_Event event;
	while (isRunning) {
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_w:
				case SDLK_UP:
					rotate = true;
					go = true;
					break;
				case SDLK_a:
				case SDLK_LEFT:
					dx = -1;
					go = true;
					Mix_HaltChannel(0);
					Mix_PlayChannel(0, move, 0);
					break;
				case SDLK_d:
				case SDLK_RIGHT:
					dx = 1;
					go = true;
					Mix_HaltChannel(0);
					Mix_PlayChannel(0, move, 0);
					break;
				case SDLK_SPACE:
					for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
						a[i] = d[i];
					}
					break;
				case SDLK_RSHIFT:
				case SDLK_LSHIFT:
					if (!switched)
						spawn = true;
					break;
				case SDLK_ESCAPE:
					pause = !pause;
					if (pause)
						Mix_PauseMusic();
					else
						Mix_ResumeMusic();
					break;
				case SDLK_MINUS:
					Mix_VolumeMusic(--volume);
					break;
				case SDLK_EQUALS:
					Mix_VolumeMusic(++volume);
					break;
				default:
					break;
				}
			default:
				break;
			}
		}

		if (first) {
			frames = 0;
			starttime = SDL_GetTicks();
			first = false;
		}

		frames++;

		Uint32 time = SDL_GetTicks();
		if (time - starttime > 0.25 && frames > 10) {
			fps = (double)frames / (time - starttime);
			starttime = SDL_GetTicks();
			frames = 0;
		}

		sprintf(title, "%0.0f", fps * 1000);
		Window_setTitle(window, title);

		/* Keyboard: Dropping blocks faster feels better when we check its
		   status every frame rather than checking whether the key is held
		   down in the switch statement if that makes sense */

		const Uint8 *keystates = SDL_GetKeyboardState(NULL);

		if (keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_DOWN])
			delay = 0.05;

		mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

		if (pause || !begun) {
			for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
				if (mouseX > buttons[i].x - buttons[i].width / 2 &&
					mouseY > buttons[i].y - buttons[i].height / 2 &&
					mouseX < buttons[i].x + buttons[i].width / 2 &&
					mouseY < buttons[i].y + buttons[i].height / 2) {
					buttons[i].mouseOver = true;
					if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						click = true;
					}
				} else
					buttons[i].mouseOver = false;
			}
		}

		if (click) {
			click = false;
			if (buttons[0].mouseOver && !begun) {
				if (Mix_FadeInMusic(theme, -1, 2000) == -1)
					printf("Mix_FadeInMusic Error: %s\n", Mix_GetError());
				n = rand() % 7;
				for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
					c[i] = a[i];
				}
				begun = true;
			} else if (buttons[1].mouseOver) {
				isRunning = false;
			} else if (buttons[2].mouseOver) {
				pause = false;
			}
		}

		// Only update when game is not paused

		if (!pause && begun) {

			// Move
			for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
				b[i] = a[i];
				a[i].x += dx;
			}

			if (!check(a, sizeof(a) / sizeof(a[0])))
				for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
					a[i] = b[i];

			// Rotate
			if (rotate) {
				Point p = a[1];
				for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
					int x = a[i].y - p.y;
					int y = a[i].x - p.x;
					a[i].x = p.x - x;
					a[i].y = p.y + y;
				}

				if (!check(a, sizeof(a) / sizeof(a[0])))
					for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
						a[i] = b[i];
			}

			// Timer
			if (SDL_GetTicks() - timer > delay * 1000) {
				timer = SDL_GetTicks();
				for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
					b[i] = a[i];
					a[i].y += 1;
				}
				// printf("Timer: %i\n", timer);

				if (!check(a, sizeof(a) / sizeof(a[0]))) {
					for (int i = 0; i < 4; i++)
						field[b[i].y][b[i].x] = n + 1;
					n = rand() % 7;
					for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
						a[i].x = figures[n][i] % 2;
						a[i].y = figures[n][i] / 2;
						c[i] = a[i];
					}
					go = true;
					switched = false;
				}
			}

			// Spawn from stash
			if (spawn) {
				if (switchBlock == -1) {
					switchBlock = n;
					n = rand() % 7;
				} else {
					int temp;
					temp = n;
					n = switchBlock;
					switchBlock = temp;
				}
				for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
					c[i] = a[i];
				}
				go = true;
				spawn = false;
				switched = true;
			}

			// Check lines cleared
			int k = M - 1;
			int cleared = 0;
			for (int i = M - 1; i > 0; i--) {
				int count = 0;
				for (int j = 0; j < N; j++) {
					if (field[i][j]) {
						count++;
					}
					field[k][j] = field[i][j];
					if (count >= N) {
						cleared++;
					}
				}
				if (count < N) {
					k--;
				}
			}

			// Update score
			switch (cleared) {
			case 1:
				score += 100;
				break;
			case 2:
				score += 300;
				break;
			case 3:
				score += 500;
				break;
			case 4:
				score += 800;
				break;
			}

			// if (cleared)
			//	printf("Cleared %i line%s\n", cleared, cleared-1 ? "s" :
			//"");
		}

		// Reset variables
		dx = 0;
		rotate = 0;
		delay = 0.3;

		// printf("N: %i\n", n);
		// for (int i = 0; i < sizeof(field)/sizeof(field[0]); i++) {
		//	for (int j = 0; j <
		// sizeof(field[0])/sizeof(field[0][0]); j++) {
		// printf("%i", field[i][j]);
		//	}
		//	printf("\n");
		//}

		// Draw

		SDL_SetRenderDrawColor(Window_getRenderer(window), 43, 43, 48, 255);
		SDL_RenderClear(Window_getRenderer(window));

		if (begun) {
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N; j++) {
					if (field[i][j] == 0)
						continue;
					draw(Window_getRenderer(window), (Point){j, i}, field[i][j],
						 255);
				}
			}

			for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
				draw(Window_getRenderer(window), a[i], n + 1, 255);
			}

			InvCheck();
			for (int i = 0; i < sizeof(c) / sizeof(c[0]); i++) {
				draw(Window_getRenderer(window), d[i], 8, 100);
			}
		}

		if (pause || !begun) {
			for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
				if (i == 2 && !begun)
					continue;
				drawUI(Window_getRenderer(window), buttons[i], font);
			}
		}

		if (!begun) {
			drawText(Window_getRenderer(window), font, 10 * 40 / 2, 100,
					 "TETRIS", 30, 50);
		}

		SDL_RenderPresent(Window_getRenderer(window));
	}

	printf("Final Score: %i\n", score);

	TTF_CloseFont(font);
	Mix_FreeChunk(move);
	Mix_FreeMusic(theme);
	Window_destroy(window);

	return 0;
}

void draw(SDL_Renderer *renderer, Point box, int n, int alpha) {
	int r, g, b;
	switch (n) {
	case 1:
		r = 0;
		g = 255;
		b = 255;
		break;
	case 2:
		r = 0;
		g = 255;
		b = 0;
		break;
	case 3:
		r = 255;
		g = 0;
		b = 0;
		break;
	case 4:
		r = 128;
		g = 0;
		b = 128;
		break;
	case 5:
		r = 255;
		g = 127;
		b = 0;
		break;
	case 6:
		r = 0;
		g = 0;
		b = 255;
		break;
	case 7:
		r = 255;
		g = 255;
		b = 0;
		break;
	default:
		r = 255;
		g = 255;
		b = 255;
		break;
	}

	SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
	SDL_RenderFillRect(renderer,
					   &(SDL_Rect){box.x * SIZE, box.y * SIZE, SIZE, SIZE});
}

void drawUI(SDL_Renderer *renderer, Button button, TTF_Font *font) {
	int originX = button.x - button.width / 2;
	int originY = button.y - button.height / 2;
	SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
	SDL_RenderFillRect(
		renderer, &(SDL_Rect){originX, originY, button.width, button.height});
	if (button.mouseOver)
		SDL_SetRenderDrawColor(renderer, 235, 89, 108, 255);
	else
		SDL_SetRenderDrawColor(renderer, 227, 61, 83, 255);
	SDL_RenderFillRect(renderer,
					   &(SDL_Rect){originX + 6, originY + 6, button.width - 12,
								   button.height - 12});

	drawText(renderer, font, button.x, button.y, button.text, 15, 25);
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, int centerX, int centerY,
			  const char *title, int sizeX, int sizeY) {
	int i = 0;
	while (1)
		if (title[++i] == '\0')
			break;
	SDL_Surface *surface;
	SDL_Texture *texture;
	surface = TTF_RenderText_Solid(font, title, (SDL_Color){255, 255, 255});
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL,
				   &(SDL_Rect){centerX - i * sizeX, centerY - sizeY,
							   0 + i * sizeX * 2, sizeY * 2});
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

bool check(Point box[], size_t size) {
	for (int i = 0; i < size; i++) {
		if (box[i].x < 0 || box[i].x >= N || box[i].y >= M)
			return 0;
		else if (field[box[i].y][box[i].x])
			return 0;
	}

	return 1;
}

void InvCheck() {
	if (go) {
		for (int i = 0; i < 4; i++)
			c[i] = a[i];
	}
	while (go) {
		for (int i = 0; i < 4; i++) {
			d[i] = c[i];
			c[i].y += 1;
			if (!check(c, 4)) {
				go = false;
			}
		}
	}
}
