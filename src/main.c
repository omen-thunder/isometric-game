#include "game.h"

// creates an SDL window
int make_window(SDL_Window** win, win_data* win_d) {
	*win = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, win_d->win_w, win_d->win_h, 0);  
	if (!*win) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

// creates a SDL renderer
int make_renderer(SDL_Window* win, SDL_Renderer** rend) {
	*rend = SDL_CreateRenderer(win, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!*rend) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

int main(void) {

	win_data win_data = {
		.win_w = 1280,
		.win_h = 720,
		.fps = 60,
		.old_t = 0,
		.pres_t = 0
	};

	cam_data cam_data = {
		.rate = 0.2f,
		.iso_x = 0,
		.iso_y = 0,
	};

	// attempt to initialise graphics and timer system
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		return 1;
	}

	// initialise the window
	SDL_Window* win;
	if (make_window(&win, &win_data)) {
		return 1;
	}
	
	// initialise the renderer
	SDL_Renderer* rend;
	if (make_renderer(win, &rend)) {
		SDL_DestroyWindow(win);
		return 1;
	}

	// initialise the textures
	tex_data tex_data;
	if (texture_init(rend, &tex_data)) {
		SDL_DestroyWindow(win);
		SDL_DestroyRenderer(rend);
		free_tex(&tex_data);
		return 1;
	}

	// initialise the map
	map_data map_data;
	if (map_init(&win_data, &map_data)) {
		SDL_DestroyWindow(win);
		SDL_DestroyRenderer(rend);
		free_tex(&tex_data);
		return 1;
	}

	// begin animation
	animate(win, rend, &win_data, &map_data, &tex_data, &cam_data);

	// free resources
	free_map(&map_data);
	free_tex(&tex_data);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	return 0;
}
