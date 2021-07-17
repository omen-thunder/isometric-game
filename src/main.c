#include "game.h"
#include <math.h>

void draw_tile(SDL_Renderer* rend, win_data* win_d, map_data* map_d, int x, int y) {
	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;

	rect.x = x * TILE_W / 2 - y * TILE_W / 2 + map_d->x_off;
	rect.y = y * TILE_H / 2 + x * TILE_H / 2 + map_d->y_off;

	if (rect.x > -TILE_W && rect.x < win_d->win_w && rect.y > -TILE_H && rect.y < win_d->win_h) {
		rect.x += map_d->x_off2;
		rect.y += map_d->y_off2;
		switch (map_d->tiles[x + map_d->x_cur][y + map_d->y_cur]) {
			case 0:
				SDL_RenderCopy(rend, map_d->textures[0], NULL, &rect);
				break;
			case 1:
				SDL_RenderCopy(rend, map_d->textures[1], NULL, &rect);
				break;
		}
	}
}

void draw_bg(SDL_Renderer* rend, win_data* win_d, map_data* map_d) {
	for (int x = 0; x < map_d->win_sz; x++) {
		for (int y = 0; y < map_d->win_sz; y++) {
			draw_tile(rend, win_d, map_d, x, y);
		}
	}
}

int texture_init(SDL_Renderer* rend, map_data* map_d) {
	if (load_texture(rend, &map_d->textures[0], "../resources/tiles/grassA.png")) {
		fprintf(stderr, "Failed to load texture 0\n");
		return -1;
	}
	if (load_texture(rend, &map_d->textures[1], "../resources/tiles/water.png")) {
		fprintf(stderr, "Failed to load texture 1\n");
		return -1;
	}
	return 0;
}

void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y) {
	if (mouse_x > win_d->win_w * 9 / 10 && mouse_y < win_d->win_h / 10) {
		if (--cam_d->frame <= 0) {
			move_u(map_d);
			cam_d->frame = cam_d->rate;
		}
	} else if (mouse_x < win_d->win_w / 10 && mouse_y < win_d->win_h / 10) {
		if (--cam_d->frame <= 0) {
			move_l(map_d);
			cam_d->frame = cam_d->rate;
		}
	} else if (mouse_x < win_d->win_w / 10 && mouse_y > win_d->win_h * 9 / 10) {
		if (--cam_d->frame <= 0) {
			move_d(win_d, map_d);
			cam_d->frame = cam_d->rate;
		}
	} else if (mouse_x > win_d->win_w * 9 / 10 && mouse_y > win_d->win_h * 9 / 10) {
		if (--cam_d->frame <= 0) {
			move_r(win_d, map_d);
			cam_d->frame = cam_d->rate;
		}
	} else if (mouse_y < win_d->win_h / 10) {
		if (--cam_d->frame <= 0) {
			move_u(map_d);
			move_l(map_d);
			cam_d->frame = cam_d->rate;
		}
	} else if (mouse_y > win_d->win_h * 9 / 10) {
		if (--cam_d->frame <= 0) {
			move_d(win_d, map_d);
			move_r(win_d, map_d);
			cam_d->frame = cam_d->rate;
		}
	} if (mouse_x < win_d->win_w / 10) {
		if (--cam_d->frame <= 0) {
			move_l(map_d);
			move_d(win_d, map_d);
			cam_d->frame = cam_d->rate;
		}
	} else if (mouse_x > win_d->win_w * 9 / 10) {
		if (--cam_d->frame <= 0) {
			move_r(win_d, map_d);
			move_u(map_d);
			cam_d->frame = cam_d->rate;
		}
	}
	return;
}

// processes events
int event(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	int button, mouse_x, mouse_y;
	button = SDL_GetMouseState(&mouse_x, &mouse_y);
	
	if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
		map_d->tiles[get_column(map_d, mouse_x, mouse_y)][get_row(map_d, mouse_x,  mouse_y)] = 1;
	else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
		map_d->tiles[get_column(map_d, mouse_x, mouse_y)][get_row(map_d, mouse_x,  mouse_y)] = 0;

	cam_pan(win_d, map_d, cam_d, mouse_x, mouse_y);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				// printf("row: %d column: %d x_cur: %d y_cur: %d\n", get_row(map, mouse_x, mouse_y), get_column(map, mouse_x, mouse_y), map->x_cur, map->y_cur);
				// printf("mouse_x: %d mouse_y: %d\n", mouse_x, mouse_y);
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						move_u(map_d);
						break;
					case SDLK_DOWN:
						move_d(win_d, map_d);
						break;
					case SDLK_LEFT:
						move_l(map_d);
						break;
					case SDLK_RIGHT:
						move_r(win_d, map_d);
						break;
					case SDLK_w:
						map_d->y_off2 += 5;
						break;
					case SDLK_s:
						map_d->y_off2 -= 5;
						break;
					case SDLK_a:
						map_d->x_off2 -= 5;
						break;
					case SDLK_d:
						map_d->x_off2 += 5;
						break;
				}
				break;
		}
	}

	return 0;
}

int animate(SDL_Window* win, SDL_Renderer* rend, win_data* win_d, map_data* map_d, cam_data* cam_d) {
	while (!event(win_d, map_d, cam_d)) {
		// clear the window
		SDL_RenderClear(rend);

		// render the background
		draw_bg(rend, win_d, map_d);

		// display the window
		SDL_RenderPresent(rend);

		// wait 1/60th of a second
		SDL_Delay(1000/60);
	}

	return 0;
}

int main(void) {
	win_data win_data = {1280, 720};
	cam_data cam_data = {0, 0, 15};
	map_data map_data;
	if (map_init(&win_data, &map_data)) {
		return 1;
	}

	// attempt to initialise graphics and timer system
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* win;
	if (make_window(&win, &win_data)) {
		closeSDL(NULL, NULL, NULL);
		return 1;
	}
	
	SDL_Renderer* rend;
	if (make_renderer(win, &rend)) {
		closeSDL(win, NULL, NULL);
		return 1;
	}

	if (texture_init(rend, &map_data)) {
		closeSDL(win, rend, &map_data);
		return 1;
	}

	animate(win, rend, &win_data, &map_data, &cam_data);

	closeSDL(win, rend, &map_data);
	return 0;
}
