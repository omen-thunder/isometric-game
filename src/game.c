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

// returns the distance between two points
float dist(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// returns the row the given point is in
int get_row(float mouse_x, float mouse_y) {
	// derived from the cartesian equation y = -x / 2 + b
	// to find the column, solve for the x-intercept and divide
	// by the tile width
	float x_off = WINDOW_W / 2 - TILE_W / 2;
	float y_off = -WINDOW_W / 4;
	float y_inter = mouse_y + mouse_x / 2 - y_off;
	return floor((x_off / -2 + y_inter) / (float) TILE_H - 0.5);
}

// returns the column the given point is in
int get_column(float mouse_x, float mouse_y) {
	// derived from the cartesian equation y = x / 2 + b
	// to find the row, solve for the x-intercept and divide
	// by the tile width
	float x_off = WINDOW_W / 2 - TILE_W / 2;
	float y_off = -WINDOW_W / 4;
	float y_inter = mouse_y - mouse_x / 2 - y_off;
	return floor((x_off / 2 + y_inter) / (float) TILE_H + 0.5);
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
					SDL_RenderCopy(rend, map->textures[1], NULL, &rect);
					break;
			}
		}
	}

	return 0;
}

int map_init(SDL_Window* win, SDL_Renderer* rend, struct mdata* map) {
	// calculate the size of each side of the background rhombus by
	// finding its side length based on the window height and width
	// and dividing it by the side length of one tile rhombus
	map->size  = (int) ceil(dist((float) -WINDOW_H,
			(float) WINDOW_H / 2,
			(float) WINDOW_W / 2,
			(float) -WINDOW_W / 4)
			/ dist(0, (float) TILE_W / 2, (float) TILE_H / 2, 0));

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

	// offset for the background rhombus derived from the intercepts
	// between the sides of the window rectangle and the sides of the
	// background rhombus
	
	map->x_off = WINDOW_W / 2 - TILE_W / 2;
	map->y_off = -WINDOW_W / 4;

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
	
	if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
		map->tiles[get_row(mouse_x, mouse_y)][get_column(mouse_x,  mouse_y)] = 1;
	else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
		map->tiles[get_row(mouse_x, mouse_y)][get_column(mouse_x,  mouse_y)] = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				printf("mouse_x: %d, mouse_y: %d\n", mouse_x, mouse_y);
				printf("row: %d\n", get_row(mouse_x, mouse_y));
				printf("column: %d\n", get_column(mouse_x, mouse_y));
				break;
		}
	}

	return 0;
}

int animate(SDL_Window* win, SDL_Renderer* rend, struct mdata* map) {
	while (!event(map)) {
		// clear the window
		SDL_RenderClear(rend);

		// render the background
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
