#include "game.h"

// creates an SDL window
int make_window(SDL_Window** win, win_data* win_d) {
	*win = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, win_d->win_w, win_d->win_h, win_d->options);  
	if (!*win) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

// creates a SDL renderer
int make_renderer(SDL_Window* win, SDL_Renderer** rend, int vsync) {
	if (vsync)
		*rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
	else
		*rend = SDL_CreateRenderer(win, -1, 0U);

	if (!*rend) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

int ld_settings(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	FILE* setf;
	if ((setf = fopen("settings.txt", "r"))) {
		fscanf(setf, "Resolution width=%d", &win_d->win_w);
		fscanf(setf, "\nResolution height=%d", &win_d->win_h);
		fscanf(setf, "\nFullscreen=%d", &win_d->fullscreen); 
		fscanf(setf, "\nBorderless window=%d", &win_d->borderless); 
		fscanf(setf, "\nInput grab=%d", &win_d->grab); 
		fscanf(setf, "\nVSync=%d", &win_d->vsync); 
		fscanf(setf, "\nFPS=%d", &win_d->fps); 
		fscanf(setf, "\nCamera base velocity=%d", &cam_d->rate); 
		fscanf(setf, "\nCamera acceleration=%d", &cam_d->accel); 
		fscanf(setf, "\nCamera sensitivity=%d", &cam_d->sensitivity); 
		fscanf(setf, "\nMap border=%d", &map_d->border); 
		fscanf(setf, "\nMap size=%d", &map_d->map_sz); 
		if (fclose(setf)) {
			fprintf(stderr, "Error closing settings.txt\n");
			return -1;
		}
	} else {
		win_d->win_w = 1920;
		win_d->win_h = 1080;
		win_d->fullscreen = 2;
		win_d->grab = 1;
		win_d->vsync = 1;
		win_d->fps = 60;
		cam_d->rate = 15;
		cam_d->accel = 2;
		cam_d->sensitivity = 10;
		map_d->border = 10; 
		map_d->map_sz = 200; 
	}

	win_d->options = 0;
	if (win_d->fullscreen == 1)
		win_d->options |= SDL_WINDOW_FULLSCREEN;
	if (win_d->fullscreen == 2)
		win_d->options |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	if (win_d->borderless)
		win_d->options |= SDL_WINDOW_BORDERLESS;
	if (win_d->grab)
		win_d->options |= SDL_WINDOW_INPUT_GRABBED;

	return 0;
}

int sv_settings(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	FILE* setf;
	if ((setf = fopen("settings.txt", "w+"))) {
		fprintf(setf, "Resolution width=%d", win_d->win_w);
		fprintf(setf, "\nResolution height=%d", win_d->win_h);
		fprintf(setf, "\nFullscreen=%d", win_d->fullscreen); 
		fprintf(setf, "\nBorderless window=%d", win_d->borderless); 
		fprintf(setf, "\nInput grab=%d", win_d->grab); 
		fprintf(setf, "\nVSync=%d", win_d->vsync); 
		fprintf(setf, "\nFPS=%d", win_d->fps); 
		fprintf(setf, "\nCamera base velocity=%d", cam_d->rate); 
		fprintf(setf, "\nCamera acceleration=%d", cam_d->accel); 
		fprintf(setf, "\nCamera sensitivity=%d", cam_d->sensitivity); 
		fprintf(setf, "\nMap border=%d", map_d->border); 
		fprintf(setf, "\nMap size=%d", map_d->map_sz); 
		if (fclose(setf)) {
			fprintf(stderr, "Error closing settings.txt\n");
			return -1;
		}

		return 0;
	}
	
	fprintf(stderr, "Error creating/opening settings.txt\n");
	return -1;

}

int main(void) {
	win_data win_data = {
		.old_t = 0,
		.pres_t = 0,
	};

	cam_data cam_data = {
		.iso_x = 0,
		.iso_y = 0,
		.buf = 0
	};

	menu_data menu_data = {
		.mode = U_DEFAULT
	};

	map_data map_data;

	if (ld_settings(&win_data, &map_data, &cam_data)) {
		fprintf(stderr, "Error loading settings\n");
		return 1;
	}

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
	if (make_renderer(win, &rend, win_data.vsync)) {
		SDL_DestroyWindow(win);
		return 1;
	}

	// initialise the textures
	tex_data tex_data;
	if (texture_init(rend, &tex_data)) {
		SDL_DestroyWindow(win);
		SDL_DestroyRenderer(rend);
		texture_free(&tex_data);
		return 1;
	}

	// initialise the map
	if (map_init(&win_data, &map_data)) {
		SDL_DestroyWindow(win);
		SDL_DestroyRenderer(rend);
		texture_free(&tex_data);
		return 1;
	}

	// begin animation
	animate(win, rend, &win_data, &map_data, &tex_data, &cam_data, &menu_data);

	sv_settings(&win_data, &map_data, &cam_data);

	// free resources
	map_free(&map_data);
	texture_free(&tex_data);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	return 0;
}
