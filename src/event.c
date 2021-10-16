#include "game.h"

// processes keyboard events
int keyboard(map_data* map_d, menu_data* menu_d) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 0;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_RIGHT:
						move_ur(map_d);
						move_dr(map_d);
						break;
					case SDLK_UP:
						move_ur(map_d);
						move_ul(map_d);
						break;
					case SDLK_LEFT:
						move_ul(map_d);
						move_dl(map_d);
						break;
					case SDLK_DOWN:
						move_dl(map_d);
						move_dr(map_d);
						break;
					case SDLK_b:
						menu_d->mode = U_BASE;
						break;
					case SDLK_t:
						menu_d->mode = U_TREE;
						break;
					case SDLK_w:
						menu_d->mode = U_WATER;
						break;
					case SDLK_f:
						menu_d->mode = U_WALL;
						break;
					case SDLK_SPACE:
						menu_d->mode = U_DEFAULT;
						break;
				}
				break;
		}
	}

	return 1;
}

// processes mouse events
void mouse(win_data* win_d, map_data* map_d, cam_data* cam_d, menu_data* menu_d) {
	// get the current state of the mouse
	int button;
	button = SDL_GetMouseState(&win_d->mouse_x, &win_d->mouse_y);
	
	// find which tile the mouse is on
	int x = get_column(map_d, cam_d, win_d->mouse_x, win_d->mouse_y) + map_d->cur_x;
	int y = get_row(map_d, cam_d, win_d->mouse_x, win_d->mouse_y) + map_d->cur_y;

	// check if the selected tile is in the boarder
	switch (menu_d->mode) {
		case U_DEFAULT:
			break;
		case U_WATER:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT) && editable(map_d, menu_d, x, y)) {
				set_tile_type(map_d, x, y, WATER);
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						if (get_tile_type(map_d, i, j) == WATER)
							set_tile_tex(map_d, i, j, water_index(map_d, i, j));
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				set_tile_type(map_d, x, y, GRASS);
				set_tile_tex(map_d, x, y, T_GRASS);
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						if (get_tile_type(map_d, i, j) == WATER)
							set_tile_tex(map_d, i, j, water_index(map_d, i, j));
			}

			break;
		case U_TREE:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT) && editable(map_d, menu_d, x, y)) {
				set_obj_type(map_d, x, y, TREE);
				set_obj_tex(map_d, x, y, T_TREE);
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				set_obj_type(map_d, x, y, EMPTY);
				set_obj_tex(map_d, x, y, T_EMPTY);
			}

			break;
		case U_WALL:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT) && editable(map_d, menu_d, x, y)) {
				set_obj_type(map_d, x, y, WALL);
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						if (get_obj_type(map_d, i, j) == WALL)
							set_obj_tex(map_d, i, j, wall_index(map_d, i, j));
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				set_obj_type(map_d, x, y, EMPTY);
				set_obj_tex(map_d, x, y, T_EMPTY);
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						if (get_obj_type(map_d, i, j) == WALL)
							set_obj_tex(map_d, i, j, wall_index(map_d, i, j));
			}

			break;
		/*
		case U_BASE:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)
				&& editable(map_d, menu_d, x, y)
				&& editable(map_d, menu_d, x + 1, y)
				&& editable(map_d, menu_d, x + 1, y - 1)
				&& editable(map_d, menu_d, x, y - 1)
				&& editable(map_d, menu_d, x - 1, y - 1)
				&& editable(map_d, menu_d, x - 1, y)
				&& editable(map_d, menu_d, x - 1, y + 1)
				&& editable(map_d, menu_d, x, y + 1)
				&& editable(map_d, menu_d, x + 1, y + 1)
			) {
				map_d->objs[x][y] = BASE;
				map_d->objs[x + 1][y] = OCCUPIED;
				map_d->objs[x + 1][y - 1] = OCCUPIED;
				map_d->objs[x][y - 1] = OCCUPIED;
				map_d->objs[x - 1][y - 1] = OCCUPIED;
				map_d->objs[x - 1][y] = OCCUPIED;
				map_d->objs[x - 1][y + 1] = OCCUPIED;
				map_d->objs[x][y + 1] = OCCUPIED;
				map_d->objs[x + 1][y + 1] = OCCUPIED;
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT) && map_d->objs[x][y] == BASE) {
				map_d->objs[x][y] = EMPTY;
				map_d->objs[x + 1][y] = EMPTY;
				map_d->objs[x + 1][y - 1] = EMPTY;
				map_d->objs[x][y - 1] = EMPTY;
				map_d->objs[x - 1][y - 1] = EMPTY;
				map_d->objs[x - 1][y] = EMPTY;
				map_d->objs[x - 1][y + 1] = EMPTY;
				map_d->objs[x][y + 1] = EMPTY;
				map_d->objs[x + 1][y + 1] = EMPTY;
			}

			break;
		*/
	}
}

// processes events
int event(win_data* win_d, map_data* map_d, cam_data* cam_d, menu_data* menu_d) {

	// keyboard events
	if (!keyboard(map_d, menu_d))
		return 0;

	// mouse events
	mouse(win_d, map_d, cam_d, menu_d);

	// pans the camera
	cam_pan(win_d, map_d, cam_d);

	return 1;
}
