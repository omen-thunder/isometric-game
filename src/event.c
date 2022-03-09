#include "game.h"

void zoom_in(Settings* settings_p, Data* data_p) {
	if (data_p->cam_zoom < 4) {
		data_p->cam_zoom++;
		data_p->win_sz = WIN_SZ;

		int new_x = data_p->mouse_adj_col - data_p->win_sz / 2;
		if (new_x >= GAP && new_x <= settings_p->map_sz - data_p->win_sz - GAP)
			data_p->map_cur_x = new_x;
		else if (new_x < GAP)
			data_p->map_cur_x = GAP;
		else if (new_x > settings_p->map_sz - data_p->win_sz - GAP)
			data_p->map_cur_x = settings_p->map_sz - data_p->win_sz - GAP;

		int new_y = data_p->mouse_adj_row - data_p->win_sz / 2;
		if (new_y >= GAP && new_y <= settings_p->map_sz - data_p->win_sz - GAP)
			data_p->map_cur_y = new_y;
		else if (new_y < GAP)
			data_p->map_cur_y = GAP;
		else if (new_y > settings_p->map_sz - data_p->win_sz - GAP)
			data_p->map_cur_y = settings_p->map_sz - data_p->win_sz - GAP;
	}
}

void zoom_out(Settings* settings_p, Data* data_p) {
	if (data_p->cam_zoom > 0) {
		data_p->cam_zoom--;
		int win_sz_old = data_p->win_sz;
		data_p->win_sz = WIN_SZ;

		int new_x = data_p->map_cur_x + (win_sz_old - data_p->win_sz) / 2;
		if (new_x >= GAP && new_x <= settings_p->map_sz - data_p->win_sz - GAP)
			data_p->map_cur_x = new_x;
		else if (new_x < GAP)
			data_p->map_cur_x = GAP;
		else if (new_x > settings_p->map_sz - data_p->win_sz - GAP)
			data_p->map_cur_x = settings_p->map_sz - data_p->win_sz - GAP;

		int new_y = data_p->map_cur_y + (win_sz_old - data_p->win_sz) / 2;
		if (new_y >= GAP && new_y <= settings_p->map_sz - data_p->win_sz - GAP)
			data_p->map_cur_y = new_y;
		else if (new_y < GAP)
			data_p->map_cur_y = GAP;
		else if (new_y > settings_p->map_sz - data_p->win_sz - GAP)
			data_p->map_cur_y = settings_p->map_sz - data_p->win_sz - GAP;
		}
}

void rot_clockwise(Data* data_p) {
	data_p->cam_view = (data_p->cam_view + 3) % 4;
	int temp = data_p->cam_iso_x;
	data_p->cam_iso_x = data_p->cam_iso_y;
	data_p->cam_iso_y = -temp;
}

void rot_anticlockwise(Data* data_p) {
	data_p->cam_view = (data_p->cam_view + 1) % 4;
	int temp = data_p->cam_iso_x;
	data_p->cam_iso_x = -data_p->cam_iso_y;
	data_p->cam_iso_y = temp;
}

int event(Settings* settings_p, Data* data_p) {
	SDL_Event event;
	const Uint8* key_state = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 0;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_RIGHT:
						rot_anticlockwise(data_p);
						break;
					case SDLK_UP:
						zoom_in(settings_p, data_p);
						break;
					case SDLK_LEFT:
						rot_clockwise(data_p);
						break;
					case SDLK_DOWN:
						zoom_out(settings_p, data_p);
						break;
					case SDLK_b:
						data_p->menu_mode = U_BASE;
						break;
					case SDLK_t:
						data_p->menu_mode = U_TREE;
						break;
					case SDLK_w:
						data_p->menu_mode = U_WATER;
						break;
					case SDLK_f:
						data_p->menu_mode = U_WALL;
						break;
					case SDLK_p:
						//data_p->menu_mode = U_PLEB;
						push_npc(&data_p->npc_head, data_p->mouse_adj_col, data_p->mouse_adj_row);
						break;
					case SDLK_SPACE:
						data_p->menu_mode = U_DEFAULT;
						break;
				}
				break;
			case SDL_MOUSEWHEEL:
				if (key_state[SDL_SCANCODE_LCTRL]) {
					if (event.wheel.y > 0 && data_p->menu_selec_sz < data_p->win_sz / 4) {
						data_p->menu_selec_sz++;
					} else if (event.wheel.y < 0 && data_p->menu_selec_sz > 0) {
						data_p->menu_selec_sz--;
					}
				} else if (event.wheel.y > 0) {
					zoom_in(settings_p, data_p);
				} else if (event.wheel.y < 0) {
					zoom_out(settings_p, data_p);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_X1)
					rot_anticlockwise(data_p);
				else if (event.button.button == SDL_BUTTON_X2)
					rot_clockwise(data_p);
				else if (event.button.button == SDL_BUTTON_MIDDLE)
					data_p->cam_drag = 1;

				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_MIDDLE)
					data_p->cam_drag = 0;

				break;
		}
	}

	return 1;
}

// returns 1 if a tile is editable, 0 if it is not
int editable(Settings* settings_p, Data* data_p, int x, int y) {
	if (OUT_OF_BOUNDS(x, y))
		return 0;

	if (data_p->map_tiles[x][y].type == GRASS && data_p->map_objs[x][y].type == EMPTY)
		return 1;
	else
		return 0;
}

void fix_indices(Sprite** map, int x, int y, int size, int edit_type, int query_type) {
	unsigned index = 0;

	for (int i = x - size; i <= x + size; i++)
		for (int j = y - size; j <= y + size; j++)
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

void mouse(Settings* settings_p, Data* data_p) {
	int x = data_p->mouse_adj_col;
	int y = data_p->mouse_adj_row;
	int button = data_p->mouse_button;
	int size = data_p->menu_selec_sz;

	switch (data_p->menu_mode) {
		case U_DEFAULT:
			break;
		case U_WATER:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				for (int i = x - size; i <= x + size; i++)
					for (int j = y - size; j <= y + size; j++)
						if (editable(settings_p, data_p, i, j)) {
							data_p->map_tiles[i][j].type = WATER;
							data_p->map_tiles[i][j].tab_id = L_WATER;
							data_p->map_tiles[i][j].rand_x = 0;
							data_p->map_tiles[i][j].rand_y = 0;
						}

				fix_indices(data_p->map_tiles, x, y, size + 1, WATER, WATER);
				fix_indices(data_p->map_tiles, x, y, size + 1, GRASS, WATER);
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				for (int i = x - size; i <= x + size; i++)
					for (int j = y - size; j <= y + size; j++)
						if (data_p->map_tiles[i][j].type == WATER) {
							data_p->map_tiles[i][j].type = GRASS;
							data_p->map_tiles[i][j].tab_id = L_GRASS;
							data_p->map_tiles[i][j].rand_x = 0;
							data_p->map_tiles[i][j].rand_y = 0;
						}

				fix_indices(data_p->map_tiles, x, y, size + 1, WATER, WATER);
				fix_indices(data_p->map_tiles, x, y, size + 1, GRASS, WATER);
			}
			break;
		case U_TREE:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				for (int i = x - size; i <= x + size; i++)
					for (int j = y - size; j <= y + size; j++)
						if (editable(settings_p, data_p, i, j)) {
							data_p->map_objs[i][j].type = TREE;
							data_p->map_objs[i][j].tab_id = L_TREE;
							data_p->map_objs[i][j].tex_index = T_TREE;
							data_p->map_objs[i][j].rand_x = RANDOM_X(i, j);
							data_p->map_objs[i][j].rand_y = RANDOM_Y(i, j);
						}
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				for (int i = x - size; i <= x + size; i++)
					for (int j = y - size; j <= y + size; j++)
						if (data_p->map_objs[i][j].type == TREE) {
							data_p->map_objs[i][j].type = EMPTY;
							data_p->map_objs[i][j].tab_id = L_EMPTY;
							data_p->map_objs[i][j].tex_index = T_EMPTY;
							data_p->map_objs[i][j].rand_x = 0;
							data_p->map_objs[i][j].rand_y = 0;
						}
			}
			break;
		case U_WALL:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				for (int i = x - size; i <= x + size; i++)
					for (int j = y - size; j <= y + size; j++)
						if (editable(settings_p, data_p, i, j)) {
							data_p->map_objs[i][j].type = WALL;
							data_p->map_objs[i][j].tab_id = L_WALL;
							data_p->map_objs[i][j].rand_x = 0;
							data_p->map_objs[i][j].rand_y = 0;
						}

				fix_indices(data_p->map_objs, x, y, size + 1, WALL, WALL);
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				for (int i = x - size; i <= x + size; i++)
					for (int j = y - size; j <= y + size; j++)
						if (data_p->map_objs[i][j].type == WALL) {
							data_p->map_objs[i][j].type = EMPTY;
							data_p->map_objs[i][j].tab_id = L_EMPTY;
							data_p->map_objs[i][j].tex_index = T_EMPTY;
							data_p->map_objs[i][j].rand_x = 0;
							data_p->map_objs[i][j].rand_y = 0;
						}

				fix_indices(data_p->map_objs, x, y, size + 1, WALL, WALL);
			}
			break;
		case U_BASE:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						if (!editable(settings_p, data_p, x, y))
							return;

				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						data_p->map_objs[i][j].type = OCCUPIED;

				data_p->map_objs[x][y].type = BASE;
				data_p->map_objs[x][y].tab_id = L_BASE;
				data_p->map_objs[x][y].tex_index = T_BASE;
				data_p->map_objs[x][y].rand_x = 0;
				data_p->map_objs[x][y].rand_y = 0;
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT) && data_p->map_objs[x][y].type == BASE) {
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						data_p->map_objs[i][j].type = EMPTY;

				data_p->map_objs[x][y].tab_id = L_EMPTY;
				data_p->map_objs[x][y].tex_index = T_EMPTY;
				data_p->map_objs[x][y].rand_x = 0;
				data_p->map_objs[x][y].rand_y = 0;
			}
			break;
		case U_PLEB:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				printf("pleb click!\n");
			}
			break;
	}
}
