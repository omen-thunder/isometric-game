#include "game.h"

// zooms the camera in
void zoom_in(win_data* win_d, map_data* map_d, int x, int y) {
	if (map_d->zoom < 4) {
		map_d->zoom++;
		map_d->win_sz = calc_win_sz(win_d->win_h, win_d->win_w, TILE_W, TILE_H);

		int new_x = x - map_d->win_sz / 2;
		if (new_x >= GAP && new_x <= map_d->map_sz - map_d->win_sz - GAP)
			map_d->cur_x = new_x;
		else if (new_x < GAP)
			map_d->cur_x = GAP;
		else if (new_x > map_d->map_sz - map_d->win_sz - GAP)
			map_d->cur_x = map_d->map_sz - map_d->win_sz - GAP;

		int new_y = y - map_d->win_sz / 2;
		if (new_y >= GAP && new_y <= map_d->map_sz - map_d->win_sz - GAP)
			map_d->cur_y = new_y;
		else if (new_y < GAP)
			map_d->cur_y = GAP;
		else if (new_y > map_d->map_sz - map_d->win_sz - GAP)
			map_d->cur_y = map_d->map_sz - map_d->win_sz - GAP;
	}
}

// zooms the camera out
void zoom_out(win_data* win_d, map_data* map_d) {
	if (map_d->zoom > 0) {
		map_d->zoom--;
			int win_sz_old = map_d->win_sz;
			map_d->win_sz = calc_win_sz(win_d->win_h, win_d->win_w, TILE_W, TILE_H);

			int new_x = map_d->cur_x + (win_sz_old - map_d->win_sz) / 2;
			if (new_x >= GAP && new_x <= map_d->map_sz - map_d->win_sz - GAP)
				map_d->cur_x = new_x;
			else if (new_x < GAP)
				map_d->cur_x = GAP;
			else if (new_x > map_d->map_sz - map_d->win_sz - GAP)
				map_d->cur_x = map_d->map_sz - map_d->win_sz - GAP;

			int new_y = map_d->cur_y + (win_sz_old - map_d->win_sz) / 2;
			if (new_y >= GAP && new_y <= map_d->map_sz - map_d->win_sz - GAP)
				map_d->cur_y = new_y;
			else if (new_y < GAP)
				map_d->cur_y = GAP;
			else if (new_y > map_d->map_sz - map_d->win_sz - GAP)
				map_d->cur_y = map_d->map_sz - map_d->win_sz - GAP;
	}
}

// process a key-press
void key_press(win_data* win_d, map_data* map_d, menu_data* menu_d, int x, int y, SDL_Event* event) {
	switch(event->key.keysym.sym) {
		case SDLK_RIGHT:
			break;
		case SDLK_UP:
			zoom_in(win_d, map_d, x, y);
			break;
		case SDLK_LEFT:
			break;
		case SDLK_DOWN:
			zoom_out(win_d, map_d);
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
}

// process a mouse-wheel event
void mouse_wheel(win_data* win_d, map_data* map_d, menu_data* menu_d, int x, int y, SDL_Event* event) {
	if (event->wheel.y > 0) {
		zoom_in(win_d, map_d, x, y);
	} else if (event->wheel.y < 0) {
		zoom_out(win_d, map_d);
	}
}

// processes keyboard events
int keyboard(win_data* win_d, map_data* map_d, menu_data* menu_d, int x, int y) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 0;
			case SDL_KEYDOWN:
				key_press(win_d, map_d, menu_d, x, y, &event);
				break;
			case SDL_MOUSEWHEEL:
				mouse_wheel(win_d, map_d, menu_d, x, y, &event);
				break;
		}
	}

	return 1;
}

// processes mouse events
void mouse(win_data* win_d, map_data* map_d, cam_data* cam_d, menu_data* menu_d, int x, int y) {
	// get the current state of the mouse
	int button;
	button = SDL_GetMouseState(&win_d->mouse_x, &win_d->mouse_y);

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
		case U_BASE:
			if (button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						if (!editable(map_d, menu_d, i, j))
							return;

				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						set_obj_type(map_d, i, j, OCCUPIED);
				
				set_obj_type(map_d, x, y, BASE);
				set_obj_tex(map_d, x, y, T_BASE);
			} else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT) && get_obj_type(map_d, x, y) == BASE) {
				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++) {
						set_obj_type(map_d, i, j, EMPTY);
					}
				
				set_obj_tex(map_d, x, y, T_EMPTY);
			}

			break;
	}
}

// processes events
int event(win_data* win_d, map_data* map_d, cam_data* cam_d, menu_data* menu_d) {
	// find which tile the mouse is on
	int x = get_column(map_d, cam_d, win_d->mouse_x, win_d->mouse_y) + map_d->cur_x;
	int y = get_row(map_d, cam_d, win_d->mouse_x, win_d->mouse_y) + map_d->cur_y;

	// keyboard events
	if (!keyboard(win_d, map_d, menu_d, x, y))
		return 0;

	// mouse events
	mouse(win_d, map_d, cam_d, menu_d, x, y);

	// pans the camera
	cam_pan(win_d, map_d, cam_d);

	return 1;
}
