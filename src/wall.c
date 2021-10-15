#include "game.h"

// return the number of diagonally adjacent wall objects
int wall_count(map_data* map_d, int x, int y) {
	int walls = 0;
	// check up-right tile
	if (map_d->objs[x][y - 1] == WALL)
		walls++;
	// check up-left tile
	if (map_d->objs[x - 1][y] == WALL)
		walls++;
	// check down-left tile
	if (map_d->objs[x][y + 1] == WALL)
		walls++;
	// check down-right tile
	if (map_d->objs[x + 1][y] == WALL)
		walls++;

	return walls;
}

// returns the index of a wall sprite with 1 surrounding wall object
int wall_sel_1(map_data* map_d, int x, int y) {
	// check up-right tile
	if (map_d->objs[x][y - 1] == WALL)
		return 1;
	// check up-left tile
	if (map_d->objs[x - 1][y] == WALL)
		return 2;
	// check down-left tile
	if (map_d->objs[x][y + 1] == WALL)
		return 3;
	// check down-right tile
	if (map_d->objs[x + 1][y] == WALL)
		return 4;
	
	return 0;
}

// returns the index of a wall sprite with 2 surrounding wall objects
int wall_sel_2(map_data* map_d, int x, int y) {
	// check up-right tile
	if (map_d->objs[x][y - 1] == WALL) {
		// check down-right tile
		if (map_d->objs[x + 1][y] == WALL)
			return 5;
		// check up-left tile
		if (map_d->objs[x - 1][y] == WALL)
			return 6;
		// check down-left tile
		if (map_d->objs[x][y + 1] == WALL)
			return 9;
	}
	// check down-right tile
	if (map_d->objs[x + 1][y] != WALL)
		return 7;
	// check up-left tile
	if (map_d->objs[x - 1][y] != WALL)
		return 8;
	// check down-left tile
	if (map_d->objs[x][y + 1] != WALL)
		return 10;
	
	return 0;
}

// returns the index of a wall sprite with 3 surrounding wall objects
int wall_sel_3(map_data* map_d, int x, int y) {
	// check down-right tile
	if (map_d->objs[x + 1][y] != WALL)
		return 11;
	// check down-left tile
	if (map_d->objs[x][y + 1] != WALL)
		return 12;
	// check up-left tile
	if (map_d->objs[x - 1][y] != WALL)
		return 13;
	// check up-right tile
	if (map_d->objs[x][y - 1] != WALL)
		return 14;
	
	return 0;
}

// returns the index of a wall sprite based on its surrounding tiles
int wall_index(map_data* map_d, int x, int y) {
	switch (wall_count(map_d, x, y)) {
		case 0:
			return 0;
		case 1:
			return wall_sel_1(map_d, x, y);
		case 2:
			return wall_sel_2(map_d, x, y);
		case 3:
			return wall_sel_3(map_d, x, y);
		case 4:
			return 15;
		default:
			return 0;
	}
}
