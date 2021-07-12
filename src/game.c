#include "game.h"
#include <math.h>

#define SPEED (200) // speed in pixels/second
#define MAX_NPC (200)
#define TILE_W (128)
#define TILE_H (64)

struct npc {
	SDL_Texture* texture;
	SDL_Rect rect;
	float x_pos;
	float y_pos;
	float x_vel;
	float y_vel;
};

int get_row(float mouse_x, float mouse_y) {
	return (int) floor(((mouse_y - mouse_x / 2) * -2 - TILE_H) / (float) TILE_W) + 10;
}

int get_column(float mouse_x, float mouse_y) {
	return (int) floor(((mouse_y + mouse_x / 2) * 2 + TILE_H) / (float) TILE_W) + 10;
}

int draw_bg(SDL_Window* win, SDL_Renderer* rend, struct mdata* map) {
	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;

	for (int x = 0; x < map->size; x++) {
		for (int y = 0; y < map->size; y++) {
			rect.x = x * TILE_W / 2 - y * TILE_W / 2 + map->x_off;
			rect.y = y * TILE_H / 2 + x * TILE_H / 2 + map->y_off;

			switch (map->tiles[x][y]) {
				case 0:
					SDL_RenderCopy(rend, map->textures[0], NULL, &rect);
					break;
				case 1:
					SDL_RenderCopy(rend, map->textures[0], NULL, &rect);
					break;
			}
		}
	}

	return 0;
}

int map_init(SDL_Window* win, SDL_Renderer* rend, struct mdata* map) {
	map->size  = (int) ceil((float) WINDOW_H / (float) TILE_H) * 2;
	if (!(map->tiles = malloc(map->size * sizeof(int *)))) {
		fprintf(stderr, "malloc() failled\n");
		return -1;
	}
	for (int i = 0; i < map->size; i++)
		if (!(map->tiles[i] = malloc(map->size * sizeof(int)))) {
			for (int j = 0; j < i; j++)
				free(map->tiles[j]);
			free(map->tiles);
			fprintf(stderr, "malloc() failled\n");
			return -1;
		}

	for (int x = 0; x < map->size; x++)
		for (int y = 0; y < map->size; y++)
			map->tiles[x][y] = 0;

	map->x_off = WINDOW_W / 2 - TILE_W / 2;
	map->y_off = -WINDOW_H / 2;
	return 0;
}

int texture_init(SDL_Renderer* rend, struct mdata* map) {
	if (load_texture(rend, &map->textures[0], "../resources/tiles/grassA.png")) {
		fprintf(stderr, "Failed to load texture 0\n");
		return -1;
	}
	if (load_texture(rend, &map->textures[1], "../resources/tiles/water.png")) {
		fprintf(stderr, "Failed to load texture 1\n");
		return -1;
	}
	return 0;
}

// processes events
int event(struct mdata* map) {
	int button, mouse_x, mouse_y;
	button = SDL_GetMouseState(&mouse_x, &mouse_y);
	/*
	if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
		map->tiles[get_row((float) mouse_x, (float) mouse_y) + map->size][get_column((float) mouse_x, (float) mouse_y) - get_row((float) mouse_x, (float) mouse_y)] = 1;
	else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
		map->tiles[get_row((float) mouse_x, (float) mouse_y) + map->size][get_column((float) mouse_x, (float) mouse_y) - get_row((float) mouse_x, (float) mouse_y)] = 0;
	*/

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				printf("mouse_x: %d, mouse_y: %d\n", mouse_x, mouse_y);
				printf("row: %d\n", get_row((float) mouse_x, (float) mouse_y));
				printf("column: %d\n", get_column((float) mouse_x, (float) mouse_y) - get_row((float) mouse_x, (float) mouse_y));
				break;
		}
	}

	return 0;
}

int animate(SDL_Window* win, SDL_Renderer* rend, struct mdata* map) {
	while (!event(map)) {
		// clear the window
		SDL_RenderClear(rend);

		draw_bg(win, rend, map);

		// display the window
		SDL_RenderPresent(rend);

		// wait 1/60th of a second
		SDL_Delay(1000/60);
	}

	return 0;
}

int main(void) {
	// attempt to initialise graphics and timer system
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* win;
	if (make_window(&win)) {
		closeSDL(NULL, NULL, NULL);
		return 1;
	}
	
	SDL_Renderer* rend;
	if (make_renderer(win, &rend)) {
		closeSDL(win, NULL, NULL);
		return 1;
	}

	struct mdata world_map;
	if (map_init(win, rend, &world_map)) {
		closeSDL(win, rend, NULL);
		return 1;
	}

	if (texture_init(rend, &world_map)) {
		closeSDL(win, rend, &world_map);
		return 1;
	}

	animate(win, rend, &world_map);

	closeSDL(win, rend, &world_map);
	return 0;
}
