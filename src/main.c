#include "window.h"
#include <stdbool.h>

#define M 20
#define N 10

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

typedef struct Point {
	int x, y;
} Point;

Point a[4] = {0}, b[4] = {0}, c[4] = {0}, d[4] = {0};

bool go = true;

bool check(Point[], size_t);

void draw(SDL_Renderer *, Point, int, int);

void InvCheck();

int main() {
	Window *window = Window_create(N * 20, M * 20, "Tetris");

	srand(SDL_GetTicks());
	int n = rand() % 7;
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
		c[i] = a[i];
	}

	int dx = 0;
	bool rotate = 0;
	unsigned int timer;
	float delay = 0.5;
	int score = 0;
	int colorNum = 1;

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
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_w:
				case SDLK_UP:
					rotate = true;
					go = true;
					break;
				case SDLK_a:
				case SDLK_LEFT:
					dx = -1;
					go = true;
					break;
				case SDLK_d:
				case SDLK_RIGHT:
					dx = 1;
					go = true;
					break;
				case SDLK_SPACE:
					for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
						a[i] = d[i];
					}
				default:
					break;
				}
			default:
				break;
			}
		}

		// Keyboard

		const Uint8 *keystates = SDL_GetKeyboardState(NULL);

		if (keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_DOWN])
			delay = 0.05;

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
					go = true;
				}
			}
		}

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

		SDL_SetRenderDrawColor(Window_getRenderer(window), 0, 0, 0, 255);
		SDL_RenderClear(Window_getRenderer(window));

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

		SDL_RenderPresent(Window_getRenderer(window));
	}

	printf("Final Score: %i\n", score);

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
	SDL_RenderFillRect(renderer, &(SDL_Rect){box.x * 20, box.y * 20, 20, 20});
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
