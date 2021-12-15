#include "game.h"

// returns the index of a water tile based on its surrounding tiles
int water_index(map_data* map_d, int x, int y) {
	unsigned index = 0;

	// check the right tile OR the up-right and down-right tiles
	if (get_tile_type(map_d, x + 1, y - 1))
		index |= 1;
	// check the up-right tile
	if (get_tile_type(map_d, x, y - 1) == WATER)
		index |= (1 << 1);
	// check the up tile OR the up-right and up-left tiles
	if (get_tile_type(map_d, x - 1, y - 1))
		index |= (1 << 2);
	// check the up-left tile
	if (get_tile_type(map_d, x - 1, y) == WATER)
		index |= (1 << 3);
	// check the left tile OR the up-left and down-left tiles
	if (get_tile_type(map_d, x - 1, y + 1))
		index |= (1 << 4);
	// check the down-left tile
	if (get_tile_type(map_d, x, y + 1) == WATER)
		index |= (1 << 5);
	// check the down tile OR the down-left and down-right tiles
	if (get_tile_type(map_d, x + 1, y + 1))
		index |= (1 << 6);
	// check the down-right tile
	if (get_tile_type(map_d, x + 1, y) == WATER)
		index |= (1 << 7);

	return index;
}
