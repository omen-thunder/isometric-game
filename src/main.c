#include "game.h"
#include <math.h>

// converts a cartesian vector to an isometric vector
// and returns the x-axis component
float iso_x(float x, float y) {
	return -x + y;
}

// converts a cartesian vector to an isometric vector
// and returns the y-axis component
float iso_y(float x, float y) {
	return (x + y) / -2;
}

// draws a single tile
void draw_tile(SDL_Renderer* rend, win_data* win_d, map_data* map_d, cam_data* cam_d, int x, int y) {
	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;

	rect.x = x * TILE_W / 2 - y * TILE_W / 2 + map_d->x_off + cam_d->frame * (int) iso_x(cam_d->rate * cam_d->x_dir, cam_d->rate * cam_d->y_dir);
	rect.y = y * TILE_H / 2 + x * TILE_H / 2 + map_d->y_off + cam_d->frame * (int) iso_y(cam_d->rate * cam_d->x_dir, cam_d->rate * cam_d->y_dir);

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

// draws the background
void draw_bg(SDL_Renderer* rend, win_data* win_d, map_data* map_d, cam_data* cam_d) {
	for (int x = 0; x < map_d->win_sz; x++)
		for (int y = 0; y < map_d->win_sz; y++)
			draw_tile(rend, win_d, map_d, cam_d, x, y);

	// if the camera is moving on the x-axis, draw an extra column
	if (cam_d->x_dir == 1) {
		for (int y = 0; y < map_d->win_sz; y++)
			draw_tile(rend, win_d, map_d, cam_d, map_d->win_sz, y);
	} else if (cam_d->x_dir == -1) {
		for (int y = 0; y < map_d->win_sz; y++)
			draw_tile(rend, win_d, map_d, cam_d, -1, y);
	}

	// if the camera is moving on the y-axis, draw an extra row 
	if (cam_d->y_dir == -1) {
		for (int x = 0; x < map_d->win_sz; x++)
			draw_tile(rend, win_d, map_d, cam_d, x, -1);
	} else if (cam_d->y_dir == 1) {
		for (int x = 0; x < map_d->win_sz; x++)
			draw_tile(rend, win_d, map_d, cam_d, x, map_d->win_sz);
	}
}

// initialise the textures
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

// returns the edge-pan direction based on the mouse position
int pan_dir(win_data* win_d, int mouse_x, int mouse_y) {
	if ((mouse_x > win_d->win_w * 9 / 10 && mouse_y < win_d->win_h / 5) 
			|| (mouse_x > win_d->win_w * 8 / 10 && mouse_y < win_d->win_h / 10))
		return 2;
	else if ((mouse_x < win_d->win_w / 10 && mouse_y < win_d->win_h / 5) 
			|| (mouse_x < win_d->win_w / 5 && mouse_y < win_d->win_h / 10))
		return 4;
	else if ((mouse_x < win_d->win_w / 10 && mouse_y > win_d->win_h * 8 / 10) 
			|| (mouse_x < win_d->win_w / 5 && mouse_y > win_d->win_h * 9 / 10))
		return 6;
	else if ((mouse_x > win_d->win_w * 9 / 10 && mouse_y > win_d->win_h * 8 / 10) 
			|| (mouse_x > win_d->win_w * 8 / 10 && mouse_y > win_d->win_h * 9 / 10))
		return 8;
	else if (mouse_x > win_d->win_w * 9 / 10)
		return 1;
	else if (mouse_y < win_d->win_h / 10)
		return 3;
	else if (mouse_x < win_d->win_w / 10)
		return 5;
	else if (mouse_y > win_d->win_h * 9 / 10)
		return 7;
	return 0;
}

// edge-pans the camera
void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y) {
	if (cam_d->frame >= TILE_H / cam_d->rate) {
		map_d->x_cur += cam_d->x_dir;
		map_d->y_cur += cam_d->y_dir;
		if ((cam_d->x_dir == 1 && cam_d->y_dir == -1) || (cam_d->x_dir == -1 && cam_d->y_dir == 1))
			cam_d->rate *= 2;
		cam_d->x_dir = 0;
		cam_d->y_dir = 0;
		cam_d->frame = 0;
	}

	if (cam_d->frame > 0) {
		cam_d->frame++;
		return;
	}

	switch (pan_dir(win_d, mouse_x, mouse_y)) {
		case 0:
			cam_d->x_dir = 0;
			cam_d->y_dir = 0;
			break;
		case 1:
			move_r(map_d, cam_d);
			break;
		case 2:
			move_ur(map_d, cam_d);
			break;
		case 3:
			move_u(map_d, cam_d);
			break;
		case 4:
			move_ul(map_d, cam_d);
			break;
		case 5:
			move_l(map_d, cam_d);
			break;
		case 6:
			move_dl(map_d, cam_d);
			break;
		case 7:
			move_d(map_d, cam_d);
			break;
		case 8:
			move_dr(map_d, cam_d);
			break;
	}
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
					case SDLK_UP:
						break;
					case SDLK_DOWN:
						break;
					case SDLK_LEFT:
						break;
					case SDLK_RIGHT:
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
		draw_bg(rend, win_d, map_d, cam_d);

		// display the window
		SDL_RenderPresent(rend);

		// wait 1/60th of a second
		SDL_Delay(1000/60);
	}

	return 0;
}

int main(void) {
	win_data win_data = {.win_w = 1280, .win_h = 720};
	cam_data cam_data = {
		.frame = 0,
		.rate = 8,	// should be a power of 2
		.x_dir = 0,
		.y_dir = 0
	};
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
