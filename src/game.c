#include "game.h"
#include <math.h>

#define SPEED (200) // speed in pixels/second
#define MAX_NPC (200)
#define TILE_W (82)
#define TILE_H (41)
#define GRID_X (WINDOW_W / TILE_W)
#define GRID_Y (WINDOW_H / (TILE_H / 2) - 1)

struct npc {
	SDL_Texture* texture;
	SDL_Rect rect;
	float x_pos;
	float y_pos;
	float x_vel;
	float y_vel;
};

int get_row(float mouse_x, float mouse_y) {
	return (int) floor(((mouse_y - mouse_x / 2) * -2 + TILE_W / 2) / TILE_W);
}

int get_column(float mouse_x, float mouse_y) {
	return (int) floor(((mouse_y + mouse_x / 2) * 2 - TILE_W / 2) / TILE_W);
}

int draw_bg(SDL_Window* win, SDL_Renderer* rend, int map[GRID_X][GRID_Y]) {
	SDL_Texture* tile_tex;
	SDL_Texture* tile_tex2;
	SDL_Rect tile_rect;
	tile_rect.w = TILE_W;
	tile_rect.h = TILE_W;
	if (load_texture(rend, &tile_tex, "../resources/tiles/grass1.gif")) {
		closeSDL(win, rend);
		return -1;
	}
	if (load_texture(rend, &tile_tex2, "../resources/tiles/water1.gif")) {
		closeSDL(win, rend);
		return -1;
	}

	for (int x = 0; x < GRID_X; x++) {
		for (int y = 0; y < GRID_Y; y++) {
			tile_rect.x = x * TILE_W + y * TILE_W / 2;

			tile_rect.y = y * (TILE_H / 2) - TILE_H;

			if (!map[x][y])
				SDL_RenderCopy(rend, tile_tex, NULL, &tile_rect);
			else
				SDL_RenderCopy(rend, tile_tex2, NULL, &tile_rect);
		}
	}

	SDL_DestroyTexture(tile_tex);
	SDL_DestroyTexture(tile_tex2);
	return 0;
}

int animate(SDL_Window* win, SDL_Renderer* rend, int map[GRID_X][GRID_Y]) {
	while (1) {
		int button, mouse_x, mouse_y;
		button = SDL_GetMouseState(&mouse_x, &mouse_y);
		if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
			map[get_row((float) mouse_x, (float) mouse_y)][get_column((float) mouse_x, (float) mouse_y) - get_row((float) mouse_x, (float) mouse_y)] = 1;
		else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
			map[get_row((float) mouse_x, (float) mouse_y)][get_column((float) mouse_x, (float) mouse_y) - get_row((float) mouse_x, (float) mouse_y)] = 0;


		// process events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					return 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					printf("mouse_x: %d, mouse_y: %d\n", mouse_x, mouse_y);
					printf("row: %d\n", get_row((float) mouse_x, (float) mouse_y));
					printf("column: %d\n", get_column((float) mouse_x, (float) mouse_y) - get_row((float) mouse_x, (float) mouse_y));
					break;
			}
		}

		// clear the window
		SDL_RenderClear(rend);

		draw_bg(win, rend, map);

		// display the window
		SDL_RenderPresent(rend);

		// wait 1/60th of a second
		SDL_Delay(1000/60);
	}
}

int main(void) {
	// attempt to initialise graphics and timer system
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* win;
	if (make_window(&win)) {
		closeSDL(NULL, NULL);
		return 1;
	}
	
	SDL_Renderer* rend;
	if (make_renderer(win, &rend)) {
		closeSDL(win, NULL);
		return 1;
	}

	int map[GRID_X][GRID_Y] = {0};

	animate(win, rend, map);

	// clean up resources before exiting
	closeSDL(win, rend);
	return 0;
}
