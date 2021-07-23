#include "game.h"

// return the number of orthogonally and diagonally
// adjacent tiles that are water
int water_count(map_data* map_d, int x, int y) {
	int water = 0;
	// check right tile
	if (map_d->tiles[x + 1][y - 1] == 1)
		water++;
	// check up-right tile
	if (map_d->tiles[x][y - 1] == 1)
		water++;
	// check up tile
	if (map_d->tiles[x - 1][y - 1] == 1)
		water++;
	// check up-left tile
	if (map_d->tiles[x - 1][y] == 1)
		water++;
	// check left tile
	if (map_d->tiles[x - 1][y + 1] == 1)
		water++;
	// check down-left tile
	if (map_d->tiles[x][y + 1] == 1)
		water++;
	// check down tile
	if (map_d->tiles[x + 1][y + 1] == 1)
		water++;
	// check down-right tile
	if (map_d->tiles[x + 1][y] == 1)
		water++;

	return water;
}

size_t water_sel_1(map_data* map_d, int x, int y) {
	// check up-right tile
	if (map_d->tiles[x][y - 1] == 1)
		return 1;
	// check up-left tile
	if (map_d->tiles[x - 1][y] == 1)
		return 2;
	// check down-left tile
	if (map_d->tiles[x][y + 1] == 1)
		return 3;
	// check down-right tile
	if (map_d->tiles[x + 1][y] == 1)
		return 4;

	return 0;
}
size_t water_sel_2(map_data* map_d, int x, int y) {
	// check up-right tile
	if (map_d->tiles[x][y - 1] == 1) {
		// check up-left tile
		if (map_d->tiles[x - 1][y] == 1)
			return 5;
		// check down-left tile
		else if (map_d->tiles[x][y + 1] == 1)
			return 6;
		// check down-right tile
		else if (map_d->tiles[x + 1][y] == 1)
			return 7;
	}
	// check up-left tile
	if (map_d->tiles[x - 1][y] == 1) {
		// check down-left tile
		if (map_d->tiles[x][y + 1] == 1)
			return 8;
		// check down-right tile
		else if (map_d->tiles[x + 1][y] == 1)
			return 9;
	}
	// check down-left and down-right tiles
	if (map_d->tiles[x][y + 1] == 1
			&& map_d->tiles[x + 1][y] == 1)
		return 10;

	return water_sel_1(map_d, x, y);
}
size_t water_sel_3(map_data* map_d, int x, int y) {
	// check right, up-right and down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1 
			&& map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 11;
	// check up-right, up and up-left tiles
	if (map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x - 1][y - 1] == 1 
			&& map_d->tiles[x - 1][y] == 1)
		return 12;
	// check up-right, up-left and down-left tiles
	if (map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x - 1][y] == 1 
			&& map_d->tiles[x][y + 1] == 1)
		return 13;
	// check up-right, up-left and down-right tiles
	if (map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x - 1][y] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 14;
	// chech up-right, down-left and down-right tiles
	if (map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x][y + 1] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 15;
	// check up-left, left and down-left tiles
	if (map_d->tiles[x - 1][y] == 1 
			&& map_d->tiles[x - 1][y + 1] == 1 
			&& map_d->tiles[x][y + 1] == 1)
		return 16;
	// check up-left, down-left, and down-right tiles
	if (map_d->tiles[x - 1][y] == 1 
			&& map_d->tiles[x][y + 1] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 17;
	// check down-left, down and down-right tiles
	if (map_d->tiles[x][y + 1] == 1 
			&& map_d->tiles[x + 1][y + 1] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 18;

	return water_sel_2(map_d, x, y);
}
size_t water_sel_4(map_data* map_d, int x, int y) {
	// check right, up-right, up-left, down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1
			&& map_d->tiles[x][y - 1]
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x + 1][y])
		return 19;
	// check right, up-right, down-left, down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1
			&& map_d->tiles[x][y - 1]
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y])
		return 20;
	// check up-right, up, up-left, down-left tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y - 1]
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x][y + 1])
		return 21;
	// check up-right, up, up-left, down-right tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y - 1]
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x + 1][y])
		return 22;
	// check up-right, up-left, left and down-left tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x - 1][y + 1]
			&& map_d->tiles[x][y + 1])
		return 23;
	// check up-right, up-left, down-left and down-right tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y])
		return 24;
	// check up-right, down-left, down and down-right tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y + 1]
			&& map_d->tiles[x + 1][y])
		return 25;
	// check up-left, left, down-left and down-right tiles
	if (map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x - 1][y + 1]
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y])
		return 26;
	// check up-left, down-left, down and down-right tiles
	if (map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y + 1]
			&& map_d->tiles[x + 1][y])
		return 27;
	
	return water_sel_3(map_d, x, y);
}
size_t water_sel_5(map_data* map_d, int x, int y) {
	// check up-left, left, down-left, down and down-right tiles
	if (map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x - 1][y + 1] == 1
			&& map_d->tiles[x][y + 1] == 1
			&& map_d->tiles[x + 1][y + 1] == 1
			&& map_d->tiles[x + 1][y] == 1)
		return 28;
	// check right, up and left tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0)
		return 29;
	// check right, up and down tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 30;
	// check right, left and down tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 31;
	// check up-right, up, up-left, left and down-left tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y - 1] == 1
			&& map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x - 1][y + 1] == 1
			&& map_d->tiles[x][y + 1] == 1)
		return 32;
	// check right, up-right down-left, down and down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1
			&& map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x][y + 1] == 1
			&& map_d->tiles[x + 1][y + 1] == 1
			&& map_d->tiles[x + 1][y] == 1)
		return 33;
	// check up, left and down tiles
	if (map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 34;
	// check right, up-right, up, up-left and down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1
			&& map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y - 1] == 1
			&& map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x + 1][y] == 1)
		return 35;

	return water_sel_4(map_d, x, y);
}
size_t water_sel_6(map_data* map_d, int x, int y) {
	// check right and up tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y - 1] == 0)
		return 36;
	// check right and left tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0)
		return 37;
	// check right and down tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 38;
	// check up and left tiles
	if (map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0)
		return 39;
	// check up and down tiles
	if (map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 40;
	// check left and down tiles
	if (map_d->tiles[x - 1][y + 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 41;

	return water_sel_5(map_d, x, y);
}
size_t water_sel_7(map_data* map_d, int x, int y) {
	// check right tile
	if (map_d->tiles[x + 1][y - 1] == 0)
		return 42;
	// check up tile
	if (map_d->tiles[x - 1][y - 1] == 0)
		return 43;
	// check left tile
	if (map_d->tiles[x - 1][y + 1] == 0)
		return 44;
	// check down tile
	if (map_d->tiles[x + 1][y + 1] == 0)
		return 45;

	return water_sel_6(map_d, x, y);
}

size_t water_index(map_data* map_d, int x, int y) {
	switch (water_count(map_d, x, y)) {
		case 0:
			return 0;
		case 1:
			return water_sel_1(map_d, x, y);
		case 2:
			return water_sel_2(map_d, x, y);
		case 3:
			return water_sel_3(map_d, x, y);
		case 4:
			return water_sel_4(map_d, x, y);
		case 5:
			return water_sel_5(map_d, x, y);
		case 6:
			return water_sel_6(map_d, x, y);
		case 7:
			return water_sel_7(map_d, x, y);
		case 8:
			return 46;
	}

	return 46;
}
