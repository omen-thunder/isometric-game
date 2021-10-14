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
	int col = get_column(map_d, cam_d, win_d->mouse_x, win_d->mouse_y) + map_d->cur_x;
	int row = get_row(map_d, cam_d, win_d->mouse_x, win_d->mouse_y) + map_d->cur_y;

	// check if the selected tile is in the boarder
	if (!out_of_bounds(map_d, col, row))
		switch (menu_d->mode) {
			case U_DEFAULT:
				break;
			case U_WATER:
				if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
					map_d->tiles[col][row] = WATER;
				else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
					map_d->tiles[col][row] = GRASS;
				break;
			case U_TREE:
				if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
					map_d->objs[col][row] = TREE;
				else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
					map_d->objs[col][row] = EMPTY;
				break;
			case U_BASE:
				if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
					map_d->objs[col][row] = BASE;
				else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
					map_d->objs[col][row] = EMPTY;
				break;
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
