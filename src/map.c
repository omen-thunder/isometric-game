#include "game.h"

#define Y_INTER(x, y, off) ((y) - ((x) / 2) - (off))

// returns the distance between two points
float dist(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int map_init(win_data* win_d, map_data* map_d) {
	// calculate the size of each side of the background rhombus by
	// finding its side length based on the window height and width
	// and dividing it by the side length of one tile rhombus
	map_d->win_sz  = (int) ceil(dist(win_d->win_h * -1.0,
			win_d->win_h / 2.0,
			win_d->win_w / 2.0,
			win_d->win_w / -4.0)
			/ dist(0, TILE_W / 2.0, TILE_H / 2.0, 0));

	map_d->map_sz = map_d->win_sz * 4;

	// allocate memory for the tile map
	if (!(map_d->tiles = malloc(map_d->map_sz * sizeof(int *)))) {
		fprintf(stderr, "malloc() failed\n");
		return -1;
	}
	for (int i = 0; i < map_d->map_sz; i++)
		if (!(map_d->tiles[i] = malloc(map_d->map_sz * sizeof(int)))) {
			for (int j = 0; j < i; j++)
				free(map_d->tiles[j]);
			free(map_d->tiles);
			fprintf(stderr, "malloc() failed\n");
			return -1;
		}

	// initialise the tile map
	for (int x = 0; x < map_d->map_sz; x++)
		for (int y = 0; y < map_d->map_sz; y++)
			map_d->tiles[x][y] = 0;

	// allocate memory for the object map
	if (!(map_d->objs = malloc(map_d->map_sz * sizeof(int *)))) {
		for (int i = 0; i < map_d->map_sz; i ++)
			free(map_d->tiles[i]);
		free(map_d->tiles);
		fprintf(stderr, "malloc() failed\n");
		return -1;
	}
	for (int i = 0; i < map_d->map_sz; i++)
		if (!(map_d->objs[i] = malloc(map_d->map_sz * sizeof(int)))) {
			for (int j = 0; j < i; j++)
				free(map_d->objs[j]);
			free(map_d->objs); for (int i = 0; i < map_d->map_sz; i ++)
				free(map_d->tiles[i]);
			free(map_d->tiles);
			fprintf(stderr, "malloc() failed\n");
			return -1;
		}

	// initialise the object map
	for (int x = 0; x < map_d->map_sz; x++)
		for (int y = 0; y < map_d->map_sz; y++) {
			if (x <= BOARDER || y <= BOARDER || x >= map_d->map_sz - BOARDER || y >= map_d->map_sz - BOARDER)
				map_d->objs[x][y] = 1;
			else
				map_d->objs[x][y] = 0;
		}

	// offset for the background rhombus derived from the intercepts
	// between the sides of the window rectangle and the sides of the
	// background rhombus
	map_d->x_off = win_d->win_w / 2 - TILE_W / 2;
	map_d->y_off = -win_d->win_w / 4;

	map_d->x_cur = 10;
	map_d->y_cur = 10;
	map_d->x_off2 = 0;
	map_d->y_off2 = 0;
	return 0;
}

// returns the row the given point is in
int get_row(map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y) {
	/* Derived from the cartesian equation y = x / 2 + b.
	To find the row, solve for the y-intercept of the above line
	using the mouse's x and y position and subtract the y-axis
	offset of the background. Using this y-intercept and the x-axis
	offset of the background, solve for the y value in the original
	equation for the line. Divide this y value by the tile height
	to get the row index */
	return floor(Y_INTER((float) (-(map_d->x_off + cam_d->iso_x + cam_d->iso_y)),
			Y_INTER(mouse_x, mouse_y, map_d->y_off + cam_d->iso_x / 2 - cam_d->iso_y / 2),
			TILE_H / -2.0) / (float) TILE_H) + map_d->y_cur;
}

// returns the column the given point is in
int get_column(map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y) {
	/* Derived from the cartesian equation y = -x / 2 + b.
	To find the column, solve for the y-intercept of the above line
	using the mouse's x and y position and subtract the y-axis
	offset of the background. Using this y-intercept and the x-axis
	offset of the background, solve for the y value in the original
	equation for the line. Divide this y value by the tile height
	to get the column index */
	return floor(Y_INTER((float) (map_d->x_off + cam_d->iso_x + cam_d->iso_y),
			Y_INTER(-mouse_x, mouse_y, map_d->y_off + cam_d->iso_x / 2 - cam_d->iso_y / 2),
			TILE_H / 2.0) / (float) TILE_H) + map_d->x_cur;
}

// move the map cursor up and right
void move_ur(map_data* map_d) {
	if (map_d->y_cur > 4)
		map_d->y_cur--;
}

// move the map cursor up and left
void move_ul(map_data* map_d) {
	if (map_d->x_cur > 4)
		map_d->x_cur--;
}

// move the map cursor down and left
void move_dl(map_data* map_d) {
	if (map_d->y_cur < map_d->map_sz - map_d->win_sz - 4)
		map_d->y_cur++;
}

// move the map cursor down and right
void move_dr(map_data* map_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4)
		map_d->x_cur++;
}
