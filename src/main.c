#include "game.h"

int ld_settings(Settings* settings_p) {
	FILE* setf;
	if ((setf = fopen("settings.txt", "r"))) {
		fscanf(setf, "Resolution width=%d\n", &settings_p->win_w);
		fscanf(setf, "Resolution height=%d\n", &settings_p->win_h);
		fscanf(setf, "Fullscreen=%d\n", &settings_p->fullscreen);
		fscanf(setf, "Borderless window=%d\n", &settings_p->borderless);
		fscanf(setf, "Input grab=%d\n", &settings_p->grab);
		fscanf(setf, "VSync=%d\n", &settings_p->vsync);
		fscanf(setf, "FPS=%d\n", &settings_p->fps);
		fscanf(setf, "Camera base velocity=%d\n", &settings_p->pan_rate);
		fscanf(setf, "Camera acceleration=%d\n", &settings_p->pan_accel);
		fscanf(setf, "Camera sensitivity=%d\n", &settings_p->pan_sens);
		fscanf(setf, "Map border=%d\n", &settings_p->border_sz);
		fscanf(setf, "Map size=%d\n", &settings_p->map_sz);
		if (fclose(setf)) {
			fprintf(stderr, "Error closing settings.txt\n");
			return -1;
		}

		printf("Settings loaded\n");
	} else {
		settings_p->win_w = 1920;
		settings_p->win_h = 1080;
		settings_p->fullscreen = 2;
		settings_p->grab = 1;
		settings_p->vsync = 1;
		settings_p->fps = 60;
		settings_p->pan_rate = 15;
		settings_p->pan_accel = 2;
		settings_p->pan_sens = 10;
		settings_p->border_sz = 10;
		settings_p->map_sz = 200;
		printf("Settings file could not be read. Using default settings\n");
	}

	return 0;
}

int sv_settings(Settings* settings_p) {
	FILE* setf;
	if ((setf = fopen("settings.txt", "w+"))) {
		fprintf(setf, "Resolution width=%d\n", settings_p->win_w);
		fprintf(setf, "Resolution height=%d\n", settings_p->win_h);
		fprintf(setf, "Fullscreen=%d\n", settings_p->fullscreen);
		fprintf(setf, "Borderless window=%d\n", settings_p->borderless);
		fprintf(setf, "Input grab=%d\n", settings_p->grab);
		fprintf(setf, "VSync=%d\n", settings_p->vsync);
		fprintf(setf, "FPS=%d\n", settings_p->fps);
		fprintf(setf, "Camera base velocity=%d\n", settings_p->pan_rate);
		fprintf(setf, "Camera acceleration=%d\n", settings_p->pan_accel);
		fprintf(setf, "Camera sensitivity=%d\n", settings_p->pan_sens);
		fprintf(setf, "Map border=%d\n", settings_p->border_sz);
		fprintf(setf, "Map size=%d\n", settings_p->map_sz);
		if (fclose(setf)) {
			fprintf(stderr, "Error closing settings.txt\n");
			return -1;
		}

		printf("Settings saved\n");
		return 0;
	}

	fprintf(stderr, "Error creating/opening settings.txt\n");
	return -1;

}

int load_texture(SDL_Renderer* rend, char* path, SDL_Texture** tex, SDL_BlendMode blend_mode) {
	// load the image into memory using SDL_image library function
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		fprintf(stderr, "Error creating surface: %s\n", SDL_GetError());
		return -1;
	}

	// load the image data into the graphics hardware's memory
	*tex = SDL_CreateTextureFromSurface(rend, surface);
	if (!*tex) {
		fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
		return -1;
	}

	/*
	if (SDL_SetTextureBlendMode(*tex, blend_mode)) {
		fprintf(stderr, "Error setting blend mode: %s\n", SDL_GetError());
		return -1;
	}
	*/

	SDL_FreeSurface(surface);
	return 0;
}

// checks if a given water texture does not exist because it is a duplicate
int water_dup(unsigned i) {
	if ((i & 1) && (i & (1 << 1)) && (i & (1 << 7)))
		i &= ~1;
	if ((i & (1 << 2)) && (i & (1 << 1)) && (i & (1 << 3)))
		i &= ~(1 << 2);
	if ((i & (1 << 4)) && (i & (1 << 3)) && (i & (1 << 5)))
		i &= ~(1 << 4);
	if ((i & (1 << 6)) && (i & (1 << 5)) && (i & (1 << 7)))
		i &= ~(1 << 6);

	return i;
}

// chekcs if a given grass texture does not exist because it is a duplicate
int grass_dup(unsigned i) {
	if ((i & (1 << 1)) && (i & (1 << 7)))
		return (i & 0b10101010);
	if ((i & (1 << 1)) && (i & (1 << 3)))
		return (i & 0b10101010);
	if ((i & (1 << 3)) && (i & (1 << 5)))
		return (i & 0b10101010);
	if ((i & (1 << 5)) && (i & (1 << 7)))
		return (i & 0b10101010);

	return 0;
}

// checks if a given grass texture does not exist because it is a duplicate
int wall_dup(unsigned i) {
	return (i & 0b10101010);
}

int tile_init(SDL_Renderer* rend, Textures* textures_p) {
	char path[50];
	unsigned dup = 0;

	// load the water textures
	for (int i = 0; i < NUM_WATER; i++) {
		sprintf(path, "./resources/tiles/water/water_%03d.png", i);
		dup = water_dup(i);
		if (i == dup) {
				if (load_texture(rend, path, &textures_p->water_tex[i], SDL_BLENDMODE_NONE)) {
					fprintf(stderr, "Failed to load water_%03d.png texture\n", i);
					return -1;
				}
				//printf("Successfully loaded water_%03d.png texture\n", i);
		} else
			textures_p->water_tex[i] = textures_p->water_tex[dup];
	}

	// load the grass textures
	for (int i = 0; i < NUM_GRASS; i++) {
		sprintf(path, "./resources/tiles/grass/grass_%03d.png", i);
		dup = grass_dup(i);
		if (i == dup) {
				if (load_texture(rend, path, &textures_p->grass_tex[i], SDL_BLENDMODE_NONE)) {
					fprintf(stderr, "Failed to load grass_%03d.png texture\n", i);
					return -1;
				}
				//printf("Successfully loaded grass_%03d.png texture\n", i);
		 } else
			textures_p->grass_tex[i] = textures_p->grass_tex[dup];
	}

	printf("Tile textures loaded\n");
	return 0;
}

int obj_init(SDL_Renderer* rend, Textures* textures_p) {
	// loads the tree texture
	if (load_texture(rend, "./resources/objects/tree.png", &textures_p->obj_tex[T_TREE], SDL_BLENDMODE_BLEND)) {
		fprintf(stderr, "Failed to load tree texture\n");
		return -1;
	}
	//printf("Successfully loaded tree texture\n");

	// loads the base-building texture
	if (load_texture(rend, "./resources/objects/base.png", &textures_p->obj_tex[T_BASE], SDL_BLENDMODE_NONE)) {
		fprintf(stderr, "Failed to load BASE texture\n");
		return -1;
	}
	//printf("Successfully loaded base texture\n");

	// load the wall textures
	char path[50];
	unsigned dup = 0;
	for (int i = 0; i < NUM_WALL; i++) {
		sprintf(path, "./resources/objects/walls/wall_%03d.png", i);
		dup = wall_dup(i);
		if (i == dup) {
				if (load_texture(rend, path, &textures_p->wall_tex[i], SDL_BLENDMODE_NONE)) {
					fprintf(stderr, "Failed to load wall_%02d.png texture\n", i);
					return -1;
				}
				//printf("Successfully loaded wall_%03d.png texture\n", i);
		 } else
			textures_p->wall_tex[i] = textures_p->wall_tex[dup];
	}

	printf("Object textures loaded\n");
	return 0;
}

int menu_init(SDL_Renderer* rend, Textures* textures_p) {
	// load the selector textures
	char path[50];
	for (int i = 0; i < NUM_SELECTOR; i++) {
		sprintf(path, "./resources/menu/selector/selector_%02d.png", i);
		if (load_texture(rend, path, &textures_p->selector_tex[i], SDL_BLENDMODE_BLEND)) {
			fprintf(stderr, "Failed to load selector_%02d.png texture\n", i);
			return -1;
		}
		//printf("Successfully loaded selector_%02d.png texture\n", i);
	}

	printf("Menu textures loaded\n");
	return 0;
}

int npc_init(SDL_Renderer* rend, Textures* textures_p) {
	char path[50];

	// load the pleb textures
	for (int i = 0; i < NUM_PLEB; i++) {
		sprintf(path, "./resources/npcs/pleb/pleb_%02d.png", i);
		if (load_texture(rend, path, &textures_p->pleb_tex[i], SDL_BLENDMODE_BLEND)) {
			fprintf(stderr, "Failed to load pleb_%02d.png texture\n", i);
			return -1;
		}
	}

	printf("NPC textures loaded\n");
	return 0;
}

int adj_col_0(Data* data_p, int col, int row) {
	return col + data_p->cur_x;
}

int adj_col_1(Data* data_p, int col, int row) {
	return row + data_p->cur_x;
}

int adj_col_2(Data* data_p, int col, int row) {
	return data_p->win_sz - 1 - col + data_p->cur_x;
}

int adj_col_3(Data* data_p, int col, int row) {
	return data_p->win_sz - 1 - row + data_p->cur_x;
}

int adj_row_0(Data* data_p, int col, int row) {
	return row + data_p->cur_y;
}

int adj_row_1(Data* data_p, int col, int row) {
	return data_p->win_sz - 1 - col + data_p->cur_y;
}

int adj_row_2(Data* data_p, int col, int row) {
	return data_p->win_sz - 1 - row + data_p->cur_y;
}

int adj_row_3(Data* data_p, int col, int row) {
	return col + data_p->cur_y;
}

int unadj_col_0(Data* data_p, int col, int row) {
	return col - data_p->cur_x;
}

int unadj_col_1(Data* data_p, int col, int row) {
	return data_p->win_sz - 1 - row + data_p->cur_y;
}

int unadj_col_2(Data* data_p, int col, int row) {
	return data_p->win_sz - 1 - col + data_p->cur_x;
}

int unadj_col_3(Data* data_p, int col, int row) {
	return row - data_p->cur_y;
}

int unadj_row_0(Data* data_p, int col, int row) {
	return row - data_p->cur_y;
}

int unadj_row_1(Data* data_p, int col, int row) {
	return col - data_p->cur_x;
}

int unadj_row_2(Data* data_p, int col, int row) {
	return data_p->win_sz - 1 - row + data_p->cur_y;
}

int unadj_row_3(Data* data_p, int col, int row) {
	return data_p->win_sz - 1 - col + data_p->cur_x;
}

int main(void) {
	SDL_Window* win;
	SDL_Renderer* rend;
	Settings settings;
	Settings* settings_p = &settings;
	Textures textures;
	Textures* textures_p = &textures;
	Maps maps;
	Maps* maps_p = &maps;
	Data data;
	Data* data_p = &data;

	printf("***Beginning initialisation***\n");
	if (ld_settings(settings_p))
		return -1;
	settings_p->tile_h = 64;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		return -1;
	}
	printf("SDL initialised\n");

	Uint32 options = 0;
	if (settings_p->fullscreen == 1)
		options |= SDL_WINDOW_FULLSCREEN;
	else if (settings_p->fullscreen == 2)
		options |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	if (settings_p->borderless)
		options |= SDL_WINDOW_BORDERLESS;
	if (settings_p->grab)
		options |= SDL_WINDOW_INPUT_GRABBED;
	win = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, settings_p->win_w, settings_p->win_h, options);
	if (!win) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return -1;
	}
	printf("Window created\n");


	if (settings_p->vsync)
		rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
	else
		rend = SDL_CreateRenderer(win, -1, 0U);
	if (!rend) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return -1;
	}
	printf("Renderer created\n");

	if (tile_init(rend, textures_p))
		return -1;
	if (obj_init(rend, textures_p))
		return -1;
	if (menu_init(rend, textures_p))
		return -1;
	if (npc_init(rend, textures_p))
		return -1;

	// allocate memory for the tile map
	if (!(maps_p->tiles = malloc(settings_p->map_sz * sizeof(Sprite*)))) {
		fprintf(stderr, "malloc() failed\n");
		return -1;
	}
	for (int i = 0; i < settings_p->map_sz; i++)
		if (!(maps_p->tiles[i] = malloc(settings_p->map_sz * sizeof(Sprite)))) {
			fprintf(stderr, "malloc() failed\n");
			return -1;
		}
	printf("Tile map memory allocated\n");

	// initialise the tile maps
	for (int x = 0; x < settings_p->map_sz; x++)
		for (int y = 0; y < settings_p->map_sz; y++) {
			maps_p->tiles[x][y].type = GRASS;
			maps_p->tiles[x][y].tab_id = L_GRASS;
			maps_p->tiles[x][y].tex_index = 0;
		}
	printf("Tile map initialised\n");

	// allocate memory for the object map
	if (!(maps_p->objs = malloc(settings_p->map_sz * sizeof(Sprite*)))) {
		fprintf(stderr, "malloc() failed\n");
		return -1;
	}
	for (int i = 0; i < settings_p->map_sz; i++)
		if (!(maps_p->objs[i] = malloc(settings_p->map_sz * sizeof(Sprite)))) {
			fprintf(stderr, "malloc() failed\n");
			return -1;
		}
	printf("Object map memory allocated\n");

	// initialise the object maps
	for (int x = 0; x < settings_p->map_sz; x++)
		for (int y = 0; y < settings_p->map_sz; y++) {
			if (OUT_OF_BOUNDS(x, y)) {
				maps_p->objs[x][y].type = TREE;
				maps_p->objs[x][y].tab_id = L_TREE;
				maps_p->objs[x][y].tex_index = T_TREE;
			} else {
				maps_p->objs[x][y].type = EMPTY;
				maps_p->objs[x][y].tab_id = L_EMPTY;
				maps_p->objs[x][y].tex_index = T_EMPTY;
			}
		}
	printf("Object map initialised\n");

	// initalise lookup tables
	data_p->tab_tex[L_GRASS] = textures_p->grass_tex;
	data_p->tab_tex[L_WATER] = textures_p->water_tex;
	data_p->tab_tex[L_SELECTOR] = textures_p->selector_tex;
	data_p->tab_tex[L_TREE] = textures_p->obj_tex;
	data_p->tab_tex[L_WALL] = textures_p->wall_tex;
	data_p->tab_tex[L_BASE] = textures_p->obj_tex;
	data_p->tab_tex[L_PLEB] = textures_p->pleb_tex;
	printf("Texture table initialised\n");

	data_p->tab_rect_w[L_GRASS] = TILE_W;
	data_p->tab_rect_w[L_WATER] = TILE_W;
	data_p->tab_rect_w[L_SELECTOR] = TILE_W;
	data_p->tab_rect_w[L_TREE] = 70;
	data_p->tab_rect_w[L_WALL] = 96;
	data_p->tab_rect_w[L_BASE] = 244;
	data_p->tab_rect_w[L_PLEB] = 60;
	printf("Rect width table initialised\n");

	data_p->tab_rect_h[L_GRASS] = TILE_H;
	data_p->tab_rect_h[L_WATER] = TILE_H;
	data_p->tab_rect_h[L_SELECTOR] = TILE_H;
	data_p->tab_rect_h[L_TREE] = 144;
	data_p->tab_rect_h[L_WALL] = 73;
	data_p->tab_rect_h[L_BASE] = 392;
	data_p->tab_rect_h[L_PLEB] = 60;
	printf("Rect height table initialised\n");

	data_p->tab_rect_x[L_GRASS] = 0;
	data_p->tab_rect_x[L_WATER] = 0;
	data_p->tab_rect_x[L_SELECTOR] = 0;
	data_p->tab_rect_x[L_TREE] = 30;
	data_p->tab_rect_x[L_WALL] = 15;
	data_p->tab_rect_x[L_BASE] = -60;
	data_p->tab_rect_x[L_PLEB] = 34;
	printf("Rect x-coordinate table initialised\n");

	data_p->tab_rect_y[L_GRASS] = 0;
	data_p->tab_rect_y[L_WATER] = 0;
	data_p->tab_rect_y[L_SELECTOR] = 0;
	data_p->tab_rect_y[L_TREE] = -96;
	data_p->tab_rect_y[L_WALL] = -15;
	data_p->tab_rect_y[L_BASE] = -300;
	data_p->tab_rect_y[L_PLEB] = -18;
	printf("Rect y-coordinate table initialised\n");

	data_p->mouse_x = 0;
	data_p->mouse_y = 0;
	data_p->mouse_col = 0;
	data_p->mouse_row = 0;
	data_p->mouse_adj_col = 0;
	data_p->mouse_adj_row = 0;
	data_p->mouse_button = 0;
	data_p->old_t = 0;
	data_p->pres_t = 0;
	data_p->iso_x = 0;
	data_p->iso_y = 0;
	data_p->buf = 0;
	data_p->view = 0;
	data_p->zoom = 2;
	data_p->prev_dir = 0;
	data_p->cur_x = settings_p->map_sz / 2;
	data_p->cur_y = settings_p->map_sz / 2;
	data_p->mode = U_DEFAULT;
	data_p->win_sz = WIN_SZ;
	data_p->selector_sz = 1;
	data_p->npc_head = NULL;
	data_p->adj_col_arr[0] = adj_col_0;
	data_p->adj_col_arr[1] = adj_col_1;
	data_p->adj_col_arr[2] = adj_col_2;
	data_p->adj_col_arr[3] = adj_col_3;
	data_p->adj_row_arr[0] = adj_row_0;
	data_p->adj_row_arr[1] = adj_row_1;
	data_p->adj_row_arr[2] = adj_row_2;
	data_p->adj_row_arr[3] = adj_row_3;
	data_p->unadj_col_arr[0] = unadj_col_0;
	data_p->unadj_col_arr[1] = unadj_col_1;
	data_p->unadj_col_arr[2] = unadj_col_2;
	data_p->unadj_col_arr[3] = unadj_col_3;
	data_p->unadj_row_arr[0] = unadj_row_0;
	data_p->unadj_row_arr[1] = unadj_row_1;
	data_p->unadj_row_arr[2] = unadj_row_2;
	data_p->unadj_row_arr[3] = unadj_row_3;
	printf("***Initialisation complete***\n");

	animate(win, rend, &settings, &textures, &maps, &data);

	printf("***Beginning deinitialisation***\n");
	if (sv_settings(settings_p))
		return -1;

	//free tile arrays
	for (int i = 0; i < settings_p->map_sz; i++)
		free(maps_p->tiles[i]);
	free(maps_p->tiles);
	printf("Tile map memory freed\n");

	//free object arrays
	for (int i = 0; i < settings_p->map_sz; i++)
		free(maps_p->objs[i]);
	free(maps_p->objs);
	printf("Object map memory freed\n");

	// free the tile textures
	for (int i = 0; i < NUM_TILES; i++)
		SDL_DestroyTexture(textures_p->tile_tex[i]);
	printf("Tile textures destroyed\n");

	// free the water textures
	for (int i = 0; i < NUM_WATER; i++)
		if (i == water_dup(i))
			SDL_DestroyTexture(textures_p->water_tex[i]);
	printf("Water textures destroyed\n");

	// free the grass textures
	for (int i = 0; i < NUM_GRASS; i++)
		if (i == grass_dup(i))
			SDL_DestroyTexture(textures_p->grass_tex[i]);
	printf("Grass textures destroyed\n");

	// free the wall textures
	for (int i = 0; i < NUM_WALL; i++)
		if (i == wall_dup(i))
			SDL_DestroyTexture(textures_p->wall_tex[i]);
	printf("Wall textures destroyed\n");

	// free the object textures
	for (int i = 1; i < NUM_OBJS; i++)
		SDL_DestroyTexture(textures_p->obj_tex[i]);
	printf("Object textures destroyed\n");

	// free the menu textures
	for (int i = 1; i < NUM_MENU; i++)
		SDL_DestroyTexture(textures_p->menu_tex[i]);
	printf("Menu textures destroyed\n");

	// free the selector textures
	for (int i = 1; i < NUM_SELECTOR; i++)
		SDL_DestroyTexture(textures_p->selector_tex[i]);
	printf("Selector textures destroyed\n");

	// free the pleb textures
	for (int i = 1; i < NUM_PLEB; i++)
		SDL_DestroyTexture(textures_p->pleb_tex[i]);
	printf("Pleb textures destroyed\n");

	while(data_p->npc_head)
		pop_npc(&data_p->npc_head);
	printf("NPC list memory freed\n");

	SDL_DestroyRenderer(rend);
	printf("Renderer destroyed\n");
	SDL_DestroyWindow(win);
	printf("Window destroyed\n");
	printf("***Deinitialisation complete***\n");

	return 0;
}
