#include "game.h"

void zoom_in(Settings* settings_p, Data* data_p) {
	if (data_p->zoom < 4) {
		data_p->zoom++;
		data_p->win_sz = WIN_SZ;

		int new_x = data_p->mouse_adj_col - data_p->win_sz / 2;
		if (new_x >= GAP && new_x <= settings_p->map_sz - data_p->win_sz - GAP)
			data_p->cur_x = new_x;
		else if (new_x < GAP)
			data_p->cur_x = GAP;
		else if (new_x > settings_p->map_sz - data_p->win_sz - GAP)
			data_p->cur_x = settings_p->map_sz - data_p->win_sz - GAP;

		int new_y = data_p->mouse_adj_row - data_p->win_sz / 2;
		if (new_y >= GAP && new_y <= settings_p->map_sz - data_p->win_sz - GAP)
			data_p->cur_y = new_y;
		else if (new_y < GAP)
			data_p->cur_y = GAP;
		else if (new_y > settings_p->map_sz - data_p->win_sz - GAP)
			data_p->cur_y = settings_p->map_sz - data_p->win_sz - GAP;
	}
}

void zoom_out(Settings* settings_p, Data* data_p) {
	if (data_p->zoom > 0) {
		data_p->zoom--;
			int win_sz_old = data_p->win_sz;
			data_p->win_sz = WIN_SZ;

			int new_x = data_p->cur_x + (win_sz_old - data_p->win_sz) / 2;
			if (new_x >= GAP && new_x <= settings_p->map_sz - data_p->win_sz - GAP)
				data_p->cur_x = new_x;
			else if (new_x < GAP)
				data_p->cur_x = GAP;
			else if (new_x > settings_p->map_sz - data_p->win_sz - GAP)
				data_p->cur_x = settings_p->map_sz - data_p->win_sz - GAP;

			int new_y = data_p->cur_y + (win_sz_old - data_p->win_sz) / 2;
			if (new_y >= GAP && new_y <= settings_p->map_sz - data_p->win_sz - GAP)
				data_p->cur_y = new_y;
			else if (new_y < GAP)
				data_p->cur_y = GAP;
			else if (new_y > settings_p->map_sz - data_p->win_sz - GAP)
				data_p->cur_y = settings_p->map_sz - data_p->win_sz - GAP;
	}
}

int event(Settings* settings_p, Data* data_p) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 0;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_RIGHT:
						data_p->view = (data_p->view + 1) % 4;
						break;
					case SDLK_UP:
						zoom_in(settings_p, data_p);
						break;
					case SDLK_LEFT:
						data_p->view = (data_p->view + 3) % 4;
						break;
					case SDLK_DOWN:
						zoom_out(settings_p, data_p);
						break;
					case SDLK_b:
						data_p->mode = U_BASE;
						break;
					case SDLK_t:
						data_p->mode = U_TREE;
						break;
					case SDLK_w:
						data_p->mode = U_WATER;
						break;
					case SDLK_f:
						data_p->mode = U_WALL;
						break;
					case SDLK_p:
						data_p->mode = U_PLEB;
						break;
					case SDLK_SPACE:
						data_p->mode = U_DEFAULT;
						break;
				}
				break;
			case SDL_MOUSEWHEEL:
				if (event.wheel.y > 0) {
					zoom_in(settings_p, data_p);
				} else if (event.wheel.y < 0) {
					zoom_out(settings_p, data_p);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_X1)
					data_p->view = (data_p->view + 1) % 4;
				if (event.button.button == SDL_BUTTON_X2)
					data_p->view = (data_p->view + 3) % 4;
				printf("view %d\n", data_p->view);
				break;
		}
	}

	return 1;
}

// returns 1 if a tile is editable, 0 if it is not
int editable(Settings* settings_p, Maps* maps_p, int x, int y) {
	if (OUT_OF_BOUNDS(x, y))
		return 0;

	if (maps_p->tiles[x][y].type == GRASS && maps_p->objs[x][y].type == EMPTY)
		return 1;
	else
		return 0;
}

/*
// returns the index of a grass tile based on its surrounding tiles
int grass_index(map_data* map_d, int x, int y) {
	unsigned index = 0;

	// check the up-right and down-right tiles
	if (get_type(map_d->tiles, x, y - 1) == WATER && get_type(map_d->tiles, x + 1, y) == WATER)
		index |= 1;
	// check the up-right and up-left tiles
	if (get_type(map_d->tiles, x, y - 1) == WATER && get_type(map_d->tiles, x - 1, y) == WATER)
		index |= (1 << 1);
	// check the up-left and down-left tiles
	if (get_type(map_d->tiles, x - 1, y) == WATER && get_type(map_d->tiles, x, y + 1) == WATER)
		index |= (1 << 2);
	// check the down-left and down-right tiles
	if (get_type(map_d->tiles, x, y + 1) == WATER && get_type(map_d->tiles, x + 1, y) == WATER)
		index |= (1 << 3);

	return index;
}

// returns the index of a wall sprite based on its surrounding tiles
int wall_index(map_data* map_d, int x, int y) {
	unsigned index = 0;

	// check the up-right tile
	if (get_type(map_d->objs, x, y - 1) == WALL)
		index |= 1;
	// check the up-left tile
	if (get_type(map_d->objs, x - 1, y) == WALL)
		index |= (1 << 1);
	// check the down-left tile
	if (get_type(map_d->objs, x, y + 1) == WALL)
		index |= (1 << 2);
	// check the down-right tile
	if (get_type(map_d->objs, x + 1, y) == WALL)
		index |= (1 << 3);

	return index;
}
*/

void fix_indices(Sprite** map, int x, int y, int edit_type, int query_type) {
	unsigned index = 0;

	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++)
			if (map[i][j].type == edit_type) {
				index = 0;
				// check the right tile
				if (map[i + 1][j - 1].type == query_type)
					index |= 1;
				// check the up-right tile
				if (map[i][j - 1].type == query_type)
					index |= (1 << 1);
				// check the up tile
				if (map[i - 1][j - 1].type == query_type)
					index |= (1 << 2);
				// check the up-left tile
				if (map[i - 1][j].type == query_type)
					index |= (1 << 3);
				// check the left tile
				if (map[i - 1][j + 1].type == query_type)
					index |= (1 << 4);
				// check the down-left tile
				if (map[i][j + 1].type == query_type)
					index |= (1 << 5);
				// check the down tile
				if (map[i + 1][j + 1].type == query_type)
					index |= (1 << 6);
				// check the down-right tile
				if (map[i + 1][j].type == query_type)
					index |= (1 << 7);

				map[i][j].tex_index = index;
			}
}

void mouse(Settings* settings_p, Maps* maps_p, Data* data_p) {
	int x = data_p->mouse_adj_col;
	int y = data_p->mouse_adj_row;
	int button = data_p->mouse_button;

	switch (data_p->mode) {
		case U_DEFAULT:
			break;
		case U_WATER:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT) && editable(settings_p, maps_p, x, y)) {
				maps_p->tiles[x][y].type = WATER;
				maps_p->tiles[x][y].tab_id = L_WATER;
				fix_indices(maps_p->tiles, x, y, WATER, WATER);
				fix_indices(maps_p->tiles, x, y, GRASS, WATER);
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				if (maps_p->tiles[x][y].type == WATER) {
					maps_p->tiles[x][y].type = GRASS;
					maps_p->tiles[x][y].tab_id = L_GRASS;
					fix_indices(maps_p->tiles, x, y, WATER, WATER);
					fix_indices(maps_p->tiles, x, y, GRASS, WATER);
				}
			}
			break;
		case U_TREE:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT) && editable(settings_p, maps_p, x, y)) {
				maps_p->objs[x][y].type = TREE;
				switch (data_p->pres_t % 5) {
					case 0:
						maps_p->objs[x][y].tab_id = L_TREE_0;
						break;
					case 1:
						maps_p->objs[x][y].tab_id = L_TREE_1;
						break;
					case 2:
						maps_p->objs[x][y].tab_id = L_TREE_2;
						break;
					case 3:
						maps_p->objs[x][y].tab_id = L_TREE_3;
						break;
					case 4:
						maps_p->objs[x][y].tab_id = L_TREE_4;
						break;
					default:
						maps_p->objs[x][y].tab_id = L_TREE_0;
				}
				maps_p->objs[x][y].tex_index = T_TREE;
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				if (maps_p->objs[x][y].type == TREE) {
					maps_p->objs[x][y].type = EMPTY;
					maps_p->objs[x][y].tab_id = L_EMPTY;
					maps_p->objs[x][y].tex_index = T_EMPTY;
				}
			}
			break;
		case U_WALL:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT) && editable(settings_p, maps_p, x, y)) {
				maps_p->objs[x][y].type = WALL;
				maps_p->objs[x][y].tab_id = L_WALL;
				fix_indices(maps_p->objs, x, y, WALL, WALL);
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				if (maps_p->objs[x][y].type == WALL) {
					maps_p->objs[x][y].type = EMPTY;
					maps_p->objs[x][y].tab_id = L_EMPTY;
					maps_p->objs[x][y].tex_index = T_EMPTY;
					fix_indices(maps_p->objs, x, y, WALL, WALL);
				}
			}
			break;
		case U_BASE:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						if (!editable(settings_p, maps_p, x, y))
							return;

				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						maps_p->objs[i][j].type = OCCUPIED;

				maps_p->objs[x][y].type = BASE;
				maps_p->objs[x][y].tab_id = L_BASE;
				maps_p->objs[x][y].tex_index = T_BASE;
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT) && maps_p->objs[x][y].type == BASE) {
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						maps_p->objs[i][j].type = EMPTY;

				maps_p->objs[x][y].tab_id = L_EMPTY;
				maps_p->objs[x][y].tex_index = T_EMPTY;
			}
			break;
		case U_PLEB:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				printf("pleb click!\n");
			}
			break;
	}
}
