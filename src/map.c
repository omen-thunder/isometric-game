#include "game.h"

// returns the distance between two points
float dist(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int map_init(SDL_Window* win, SDL_Renderer* rend, struct mdata* map) {
	// calculate the size of each side of the background rhombus by
	// finding its side length based on the window height and width
	// and dividing it by the side length of one tile rhombus
	map->win_sz  = (int) ceil(dist((float) -WINDOW_H,
			(float) WINDOW_H / 2,
			(float) WINDOW_W / 2,
			(float) -WINDOW_W / 4)
			/ dist(0, (float) TILE_W / 2, (float) TILE_H / 2, 0));

	map->map_sz = 50;

	if (!(map->tiles = malloc(map->map_sz * sizeof(int *)))) {
		fprintf(stderr, "malloc() failled\n");
		return -1;
	}

	for (int i = 0; i < map->map_sz; i++)
		if (!(map->tiles[i] = malloc(map->map_sz * sizeof(int)))) {
			for (int j = 0; j < i; j++)
				free(map->tiles[j]);
			free(map->tiles);
			fprintf(stderr, "malloc() failled\n");
			return -1;
		}

	for (int x = 0; x < map->map_sz; x++)
		for (int y = 0; y < map->map_sz; y++)
			map->tiles[x][y] = 0;

	// offset for the background rhombus derived from the intercepts
	// between the sides of the window rectangle and the sides of the
	// background rhombus
	map->x_off = WINDOW_W / 2 - TILE_W / 2;
	map->y_off = -WINDOW_W / 4;

	map->x_cur = map->map_sz / 2;
	map->y_cur = map->map_sz / 2;
	map->x_off2 = 0;
	map->y_off2 = 0;
	return 0;
}

// returns the row the given point is in
int get_row(struct mdata* map, float mouse_x, float mouse_y) {
	/* Derived from the cartesian equation y = x / 2 + b.
	To find the row, solve for the y-intercept of the above line
	using the mouse's x and y position and subtract the y-axis
	offset of the background. Using this y-intercept and the x-axis
	offset of the background, solve for the y value in the original
	equation for the line. Divide this y value by the tile height
	to get the row index */
	float y_inter = mouse_y - mouse_x / 2 - map->y_off;
	return floor(((float) map->x_off / 2 + y_inter) / (float) TILE_H + 0.5) + map->y_cur;
}

// returns the column the given point is in
int get_column(struct mdata* map, float mouse_x, float mouse_y) {
	/* Derived from the cartesian equation y = -x / 2 + b.
	To find the column, solve for the y-intercept of the above line
	using the mouse's x and y position and subtract the y-axis
	offset of the background. Using this y-intercept and the x-axis
	offset of the background, solve for the y value in the original
	equation for the line. Divide this y value by the tile height
	to get the column index */
	float y_inter = mouse_y + mouse_x / 2 - map->y_off;
	return floor(((float) map->x_off / -2 + y_inter) / (float) TILE_H - 0.5) + map->x_cur;
}

void cam_l(struct mdata* map) {
	if (map->x_cur > 0)
		map->x_cur--;
	return;
}

void cam_u(struct mdata* map) {
	if (map->y_cur > 0)
		map->y_cur--;
	return;
}

void cam_r(struct mdata* map) {
	if (map->x_cur < map->map_sz - map->win_sz)
		map->x_cur++;
	return;
}

void cam_d(struct mdata* map) {
	if (map->y_cur < map->map_sz - map->win_sz)
		map->y_cur++;
	return;
}
