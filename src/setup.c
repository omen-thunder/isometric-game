#include "game.h"

// frees resources and exits SDL
void closeSDL(SDL_Window* win, SDL_Renderer* rend, map_data* map_d, tex_data* tex_d) {
	if (rend)
		SDL_DestroyRenderer(rend);
	if (win)
		SDL_DestroyWindow(win);
	if (map_d) {
		for (int i = 0; i < map_d->map_sz; i++)
			free(map_d->tiles[i]);
		free(map_d->tiles);

		for (int i = 0; i < map_d->map_sz; i++)
			free(map_d->objs[i]);
		free(map_d->objs);

	}
	if (tex_d) {
		for (int i = 0; i < NUM_TILES; i++)
			SDL_DestroyTexture(tex_d->tile_tex[i]);
		for (int i = 1; i < NUM_OBJS; i++)
			SDL_DestroyTexture(tex_d->obj_tex[i]);
	}

	SDL_Quit();
}

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

// creates and returns an SDL texture from a given file path
int load_texture(SDL_Renderer* rend, SDL_Texture** tex, char* path) {
	// load the image into memory using SDL_image library function
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		fprintf(stderr, "Error creating surface: %s\n", SDL_GetError());
		return -1;
	}

	// load the image data into the graphics hardware's memory
	*tex = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);
	if (!*tex) {
		fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
		return -1;
	}
	
	return 0;
}
