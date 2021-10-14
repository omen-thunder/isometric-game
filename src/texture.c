#include "game.h"

// creates an SDL texture from a given file path
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

// loads the tile textures
int tile_init(SDL_Renderer* rend, tex_data* tex_d) {
	// load the water textures
	char path[50];
	for (int i = 0; i < NUM_WATER; i++) {
		sprintf(path, "./resources/tiles/water/water_%02d.png", i);
		if (load_texture(rend, &tex_d->water_tex[i], path)) {
			fprintf(stderr, "Failed to load water_%02d.png texture\n", i);
			return -1;
		}
	}

	// load the grass texture
	if (load_texture(rend, &tex_d->tile_tex[T_GRASS], "./resources/tiles/grass.png")) {
		fprintf(stderr, "Failed to load GRASS texture\n");
		return -1;
	}

	return 0;
}

// loads the object textures
int obj_init(SDL_Renderer* rend, tex_data* tex_d) {
	if (load_texture(rend, &tex_d->obj_tex[T_TREE], "./resources/objects/tree.png")) {
		fprintf(stderr, "Failed to load TREE texture\n");
		return -1;
	}
	SDL_SetTextureBlendMode(tex_d->obj_tex[T_TREE], SDL_BLENDMODE_BLEND);

	if (load_texture(rend, &tex_d->obj_tex[T_BASE], "./resources/objects/base.png")) {
		fprintf(stderr, "Failed to load BASE texture\n");
		return -1;
	}

	return 0;
}

// loads the menu textures
int menu_init(SDL_Renderer* rend, tex_data* tex_d) {
	// load the selector textures
	char path[50];
	for (int i = 0; i < NUM_SELECTOR; i++) {
		sprintf(path, "./resources/menu/selector/selector_%02d.png", i);
		if (load_texture(rend, &tex_d->selector_tex[i], path)) {
			fprintf(stderr, "Failed to load selector_%02d.png texture\n", i);
			return -1;
		}

		SDL_SetTextureBlendMode(tex_d->selector_tex[i], SDL_BLENDMODE_BLEND);
	}

	return 0;
}

// initialise the textures
int texture_init(SDL_Renderer* rend, tex_data* tex_d) {
	if (tile_init(rend, tex_d))
		return -1;

	if (obj_init(rend, tex_d)) {
		return -1;
	}

	if (menu_init(rend, tex_d)) {
		return -1;
	}

	return 0;
}

// frees resources in the tex_data struct
void texture_free(tex_data* tex_d) {
	// free the tile textures
	for (int i = 0; i < NUM_TILES; i++)
		if (tex_d->tile_tex[i])
			SDL_DestroyTexture(tex_d->tile_tex[i]);
	
	// free the water textures
	for (int i = 0; i < NUM_WATER; i++)
		if (tex_d->water_tex[i])
			SDL_DestroyTexture(tex_d->water_tex[i]);

	// free the object textures
	for (int i = 1; i < NUM_OBJS; i++)
		if (tex_d->obj_tex[i])
			SDL_DestroyTexture(tex_d->obj_tex[i]);

	// free the menu textures
	for (int i = 1; i < NUM_MENU; i++)
		if (tex_d->menu_tex[i])
			SDL_DestroyTexture(tex_d->menu_tex[i]);

	// free the selector textures
	for (int i = 1; i < NUM_SELECTOR; i++)
		if (tex_d->selector_tex[i])
			SDL_DestroyTexture(tex_d->selector_tex[i]);
}
