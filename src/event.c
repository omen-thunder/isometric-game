#include "game.h"

// processes events
int event(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	// get the current state of the mouse
	int button;
	button = SDL_GetMouseState(&win_d->mouse_x, &win_d->mouse_y);
	
	// edit the tiles on the map based on the mouse state
	int col = get_column(map_d, cam_d, win_d->mouse_x, win_d->mouse_y) + map_d->cur_x;
	int row = get_row(map_d, cam_d, win_d->mouse_x, win_d->mouse_y) + map_d->cur_y;

	if (col > BOARDER && row > BOARDER && col < map_d->map_sz - BOARDER && row < map_d->map_sz - BOARDER) {
		if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
			map_d->tiles[col][row] = 1;
		else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
			map_d->tiles[col][row] = 0;
	}

	cam_pan(win_d, map_d, cam_d);

	// main event handling
	// not really spag code, but not pretty
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
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
						map_d->objs[col][row] = 1;
						break;
					case SDLK_t:
						map_d->objs[col][row] = 0;
						break;
					case SDLK_SPACE:
						map_d->objs[col][row] = -1;
						break;
				}
				break;
		}
	}

	return 0;
}
