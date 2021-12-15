#include "game.h"

// returns the index of a wall sprite based on its surrounding tiles
int wall_index(map_data* map_d, int x, int y) {
	unsigned index = 0;

	// check the up-right tile
	if (get_obj_type(map_d, x, y - 1) == WALL)
		index |= 1;
	// check the up-left tile
	if (get_obj_type(map_d, x - 1, y) == WALL)
		index |= (1 << 1);
	// check the down-left tile
	if (get_obj_type(map_d, x, y + 1) == WALL)
		index |= (1 << 2);
	// check the down-right tile
	if (get_obj_type(map_d, x + 1, y) == WALL)
		index |= (1 << 3);

	return index;
}
