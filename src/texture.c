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

// loads the each water texture
// if a given water texture does not exist because it is a duplicate,
// replaces it with its duplicate
int water_load(SDL_Renderer* rend, tex_data* tex_d, char* path,  unsigned i) {
	unsigned dup = i;
	if (!(i & 1) && (i & (1 << 1)) && (i & (1 << 7)))
		dup |= 1;
	if (!(i & (1 << 2)) && (i & (1 << 1)) && (i & (1 << 3)))
		dup |= 1 << 2;
	if (!(i & (1 << 4)) && (i & (1 << 3)) && (i & (1 << 5)))
		dup |= 1 << 4;
	if (!(i & (1 << 6)) && (i & (1 << 5)) && (i & (1 << 7)))
		dup |= 1 << 6;

	if (dup != i)
		tex_d->water_tex[i] = tex_d->water_tex[dup];
	else
		if (load_texture(rend, &tex_d->water_tex[i], path)) {
			fprintf(stderr, "Failed to load water_%03u.png texture\n", i);
			return -1;
		}

	return 0;
}

// loads the tile textures
int tile_init(SDL_Renderer* rend, tex_data* tex_d) {
	char path[50];

	// load the water textures
	for (int i = NUM_WATER - 1; i >= 0; i--) {
		sprintf(path, "./resources/tiles/water/water_%03d.png", i);
		if (water_load(rend, tex_d, path, i))
			return -1;
	}

	// load the grass textures
	for (int i = 0; i < NUM_GRASS; i++) {
		sprintf(path, "./resources/tiles/grass/grass_%02d.png", i);
		if (load_texture(rend, &tex_d->grass_tex[i], path)) {
			fprintf(stderr, "Failed to load grass_%02d.png texture\n", i);
			return -1;
		}
	}

	return 0;
}

// loads the object textures
int obj_init(SDL_Renderer* rend, tex_data* tex_d) {
	// loads the tree texture
	if (load_texture(rend, &tex_d->obj_tex[T_TREE], "./resources/objects/tree.png")) {
		fprintf(stderr, "Failed to load TREE texture\n");
		return -1;
	}
	SDL_SetTextureBlendMode(tex_d->obj_tex[T_TREE], SDL_BLENDMODE_BLEND);

	// loads the base-building texture
	if (load_texture(rend, &tex_d->obj_tex[T_BASE], "./resources/objects/base.png")) {
		fprintf(stderr, "Failed to load BASE texture\n");
		return -1;
	}

	// load the wall textures
	char path[50];
	for (int i = 0; i < NUM_WALL; i++) {
		sprintf(path, "./resources/objects/walls/wall_%02d.png", i);
		if (load_texture(rend, &tex_d->wall_tex[i], path)) {
			fprintf(stderr, "Failed to load wall_%02d.png texture\n", i);
			return -1;
		}
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
		SDL_DestroyTexture(tex_d->tile_tex[i]);
	
	// free the grass textures
	for (int i = 0; i < NUM_GRASS; i++)
		SDL_DestroyTexture(tex_d->grass_tex[i]);

	// free the wall textures
	for (int i = 0; i < NUM_WALL; i++)
		SDL_DestroyTexture(tex_d->wall_tex[i]);

	// free the water textures
	for (int i = 0; i < NUM_WATER; i++)
		SDL_DestroyTexture(tex_d->water_tex[i]);

	// free the object textures
	for (int i = 1; i < NUM_OBJS; i++)
		SDL_DestroyTexture(tex_d->obj_tex[i]);

	// free the menu textures
	for (int i = 1; i < NUM_MENU; i++)
		SDL_DestroyTexture(tex_d->menu_tex[i]);

	// free the selector textures
	for (int i = 1; i < NUM_SELECTOR; i++)
		SDL_DestroyTexture(tex_d->selector_tex[i]);
}

// returns the index of a grass tile based on its surrounding tiles
int grass_index(map_data* map_d, int x, int y) {
	unsigned index = 0;

	// check the up-right and down-right tiles
	if (get_type(map_d->tiles, x, y - 1) == WATER && get_type(map_d->tiles, x + 1, y) == WATER)
		index |= 1;
	// check the up-right and up-left tiles
	if (get_type(map_d->tiles, x, y - 1) == WATER && get_type(map_d->tiles, x - 1, y) == WATER)
		index |= (1 << 1);
	// check the up-left and down-left tiles
	if (get_type(map_d->tiles, x - 1, y) == WATER && get_type(map_d->tiles, x, y + 1) == WATER)
		index |= (1 << 2);
	// check the down-left and down-right tiles
	if (get_type(map_d->tiles, x, y + 1) == WATER && get_type(map_d->tiles, x + 1, y) == WATER)
		index |= (1 << 3);

	return index;
}

// returns the index of a wall sprite based on its surrounding tiles
int wall_index(map_data* map_d, int x, int y) {
	unsigned index = 0;

	// check the up-right tile
	if (get_type(map_d->objs, x, y - 1) == WALL)
		index |= 1;
	// check the up-left tile
	if (get_type(map_d->objs, x - 1, y) == WALL)
		index |= (1 << 1);
	// check the down-left tile
	if (get_type(map_d->objs, x, y + 1) == WALL)
		index |= (1 << 2);
	// check the down-right tile
	if (get_type(map_d->objs, x + 1, y) == WALL)
		index |= (1 << 3);

	return index;
}

// returns the index of a water tile based on its surrounding tiles
int water_index(map_data* map_d, int x, int y) {
	unsigned index = 0;

	// check the right tile
	if (get_type(map_d->tiles, x + 1, y - 1) == WATER)
		index |= 1;
	// check the up-right tile
	if (get_type(map_d->tiles, x, y - 1) == WATER)
		index |= (1 << 1);
	// check the up tile
	if (get_type(map_d->tiles, x - 1, y - 1) == WATER)
		index |= (1 << 2);
	// check the up-left tile
	if (get_type(map_d->tiles, x - 1, y) == WATER)
		index |= (1 << 3);
	// check the left tile
	if (get_type(map_d->tiles, x - 1, y + 1) == WATER)
		index |= (1 << 4);
	// check the down-left tile
	if (get_type(map_d->tiles, x, y + 1) == WATER)
		index |= (1 << 5);
	// check the down tile
	if (get_type(map_d->tiles, x + 1, y + 1) == WATER)
		index |= (1 << 6);
	// check the down-right tile
	if (get_type(map_d->tiles, x + 1, y) == WATER)
		index |= (1 << 7);

	return index;
}
