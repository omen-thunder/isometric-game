#include "game.h"
#include <math.h>

enum tile_tex {GRASS = 47};
enum obj_tex {NONE, TREE};


// converts a cartesian vector to an isometric vector
// and returns the x-axis component
int iso_x(int x, int y) {
	return -x + y;
}

// converts a cartesian vector to an isometric vector
// and returns the y-axis component
int iso_y(int x, int y) {
	return (x + y) / -2;
}

// return the number of orthogonally and diagonally
// adjacent tiles that are water
int water_count(map_data* map_d, int x, int y) {
	int water = 0;
	// check right tile
	if (map_d->tiles[x + 1][y - 1] == 1)
		water++;
	// check up-right tile
	if (map_d->tiles[x][y - 1] == 1)
		water++;
	// check up tile
	if (map_d->tiles[x - 1][y - 1] == 1)
		water++;
	// check up-left tile
	if (map_d->tiles[x - 1][y] == 1)
		water++;
	// check left tile
	if (map_d->tiles[x - 1][y + 1] == 1)
		water++;
	// check down-left tile
	if (map_d->tiles[x][y + 1] == 1)
		water++;
	// check down tile
	if (map_d->tiles[x + 1][y + 1] == 1)
		water++;
	// check down-right tile
	if (map_d->tiles[x + 1][y] == 1)
		water++;

	return water;
}

size_t water_sel_1(map_data* map_d, int x, int y) {
	// check up-right tile
	if (map_d->tiles[x][y - 1] == 1)
		return 1;
	// check up-left tile
	if (map_d->tiles[x - 1][y] == 1)
		return 2;
	// check down-left tile
	if (map_d->tiles[x][y + 1] == 1)
		return 3;
	// check down-right tile
	if (map_d->tiles[x + 1][y] == 1)
		return 4;

	return 0;
}
size_t water_sel_2(map_data* map_d, int x, int y) {
	// check up-right tile
	if (map_d->tiles[x][y - 1] == 1) {
		// check up-left tile
		if (map_d->tiles[x - 1][y] == 1)
			return 5;
		// check down-left tile
		else if (map_d->tiles[x][y + 1] == 1)
			return 6;
		// check down-right tile
		else if (map_d->tiles[x + 1][y] == 1)
			return 7;
	}
	// check up-left tile
	if (map_d->tiles[x - 1][y] == 1) {
		// check down-left tile
		if (map_d->tiles[x][y + 1] == 1)
			return 8;
		// check down-right tile
		else if (map_d->tiles[x + 1][y] == 1)
			return 9;
	}
	// check down-left and down-right tiles
	if (map_d->tiles[x][y + 1] == 1
			&& map_d->tiles[x + 1][y] == 1)
		return 10;

	return water_sel_1(map_d, x, y);
}
size_t water_sel_3(map_data* map_d, int x, int y) {
	// check right, up-right and down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1 
			&& map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 11;
	// check up-right, up and up-left tiles
	if (map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x - 1][y - 1] == 1 
			&& map_d->tiles[x - 1][y] == 1)
		return 12;
	// check up-right, up-left and down-left tiles
	if (map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x - 1][y] == 1 
			&& map_d->tiles[x][y + 1] == 1)
		return 13;
	// check up-right, up-left and down-right tiles
	if (map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x - 1][y] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 14;
	// chech up-right, down-left and down-right tiles
	if (map_d->tiles[x][y - 1] == 1 
			&& map_d->tiles[x][y + 1] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 15;
	// check up-left, left and down-left tiles
	if (map_d->tiles[x - 1][y] == 1 
			&& map_d->tiles[x - 1][y + 1] == 1 
			&& map_d->tiles[x][y + 1] == 1)
		return 16;
	// check up-left, down-left, and down-right tiles
	if (map_d->tiles[x - 1][y] == 1 
			&& map_d->tiles[x][y + 1] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 17;
	// check down-left, down and down-right tiles
	if (map_d->tiles[x][y + 1] == 1 
			&& map_d->tiles[x + 1][y + 1] == 1 
			&& map_d->tiles[x + 1][y] == 1)
		return 18;

	return water_sel_2(map_d, x, y);
}
size_t water_sel_4(map_data* map_d, int x, int y) {
	// check right, up-right, up-left, down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1
			&& map_d->tiles[x][y - 1]
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x + 1][y])
		return 19;
	// check right, up-right, down-left, down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1
			&& map_d->tiles[x][y - 1]
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y])
		return 20;
	// check up-right, up, up-left, down-left tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y - 1]
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x][y + 1])
		return 21;
	// check up-right, up, up-left, down-right tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y - 1]
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x + 1][y])
		return 22;
	// check up-right, up-left, left and down-left tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x - 1][y + 1]
			&& map_d->tiles[x][y + 1])
		return 23;
	// check up-right, up-left, down-left and down-right tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y]
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y])
		return 24;
	// check up-right, down-left, down and down-right tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y + 1]
			&& map_d->tiles[x + 1][y])
		return 25;
	// check up-left, left, down-left and down-right tiles
	if (map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x - 1][y + 1]
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y])
		return 26;
	// check up-left, down-left, down and down-right tiles
	if (map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x][y + 1]
			&& map_d->tiles[x + 1][y + 1]
			&& map_d->tiles[x + 1][y])
		return 27;
	
	return water_sel_3(map_d, x, y);
}
size_t water_sel_5(map_data* map_d, int x, int y) {
	// check up-left, left, down-left, down and down-right tiles
	if (map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x - 1][y + 1] == 1
			&& map_d->tiles[x][y + 1] == 1
			&& map_d->tiles[x + 1][y + 1] == 1
			&& map_d->tiles[x + 1][y] == 1)
		return 28;
	// check right, up and left tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0)
		return 29;
	// check right, up and down tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 30;
	// check right, left and down tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 31;
	// check up-right, up, up-left, left and down-left tiles
	if (map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y - 1] == 1
			&& map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x - 1][y + 1] == 1
			&& map_d->tiles[x][y + 1] == 1)
		return 32;
	// check right, up-right down-left, down and down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1
			&& map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x][y + 1] == 1
			&& map_d->tiles[x + 1][y + 1] == 1
			&& map_d->tiles[x + 1][y] == 1)
		return 33;
	// check up, left and down tiles
	if (map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 34;
	// check right, up-right, up, up-left and down-right tiles
	if (map_d->tiles[x + 1][y - 1] == 1
			&& map_d->tiles[x][y - 1] == 1
			&& map_d->tiles[x - 1][y - 1] == 1
			&& map_d->tiles[x - 1][y] == 1
			&& map_d->tiles[x + 1][y] == 1)
		return 35;

	return water_sel_4(map_d, x, y);
}
size_t water_sel_6(map_data* map_d, int x, int y) {
	// check right and up tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y - 1] == 0)
		return 36;
	// check right and left tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0)
		return 37;
	// check right and down tiles
	if (map_d->tiles[x + 1][y - 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 38;
	// check up and left tiles
	if (map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x - 1][y + 1] == 0)
		return 39;
	// check up and down tiles
	if (map_d->tiles[x - 1][y - 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 40;
	// check left and down tiles
	if (map_d->tiles[x - 1][y + 1] == 0 
			&& map_d->tiles[x + 1][y + 1] == 0)
		return 41;

	return water_sel_5(map_d, x, y);
}
size_t water_sel_7(map_data* map_d, int x, int y) {
	// check right tile
	if (map_d->tiles[x + 1][y - 1] == 0)
		return 42;
	// check up tile
	if (map_d->tiles[x - 1][y - 1] == 0)
		return 43;
	// check left tile
	if (map_d->tiles[x - 1][y + 1] == 0)
		return 44;
	// check down tile
	if (map_d->tiles[x + 1][y + 1] == 0)
		return 45;

	return water_sel_6(map_d, x, y);
}

size_t water_index(map_data* map_d, int x, int y) {
	switch (water_count(map_d, x, y)) {
		case 0:
			return 0;
		case 1:
			return water_sel_1(map_d, x, y);
		case 2:
			return water_sel_2(map_d, x, y);
		case 3:
			return water_sel_3(map_d, x, y);
		case 4:
			return water_sel_4(map_d, x, y);
		case 5:
			return water_sel_5(map_d, x, y);
		case 6:
			return water_sel_6(map_d, x, y);
		case 7:
			return water_sel_7(map_d, x, y);
		case 8:
			return 46;
	}

	return 46;
}

// draws a single tile
void draw_tile(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y) {
	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;

	rect.x = (x - y) * TILE_W / 2 + map_d->x_off + cam_d->frame * iso_x(cam_d->rate * cam_d->x_dir, cam_d->rate * cam_d->y_dir);
	rect.y = (y + x) * TILE_H / 2 + map_d->y_off + cam_d->frame * iso_y(cam_d->rate * cam_d->x_dir, cam_d->rate * cam_d->y_dir);

	if (rect.x > -TILE_W && rect.x < win_d->win_w && rect.y > -TILE_H && rect.y < win_d->win_h) {
		rect.x += map_d->x_off2;
		rect.y += map_d->y_off2;
		switch (map_d->tiles[x + map_d->x_cur][y + map_d->y_cur]) {
			case 0:
				SDL_RenderCopy(rend, tex_d->tile_tex[GRASS], NULL, &rect);
				break;
			case 1:
				SDL_RenderCopy(rend, tex_d->tile_tex[water_index(map_d, x + map_d->x_cur, y + map_d->y_cur)], NULL, &rect);
				break;
		}
	}
}

// draws and object
void draw_obj(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y) {
	SDL_Rect rect;
	SDL_QueryTexture(tex_d->obj_tex[TREE], NULL, NULL,  &rect.w, &rect.h);
	rect.w /= 5;
	rect.h /= 5;

	rect.x = (x - y) * TILE_W / 2 + map_d->x_off + cam_d->frame * iso_x(cam_d->rate * cam_d->x_dir, cam_d->rate * cam_d->y_dir) + 30;
	rect.y = (y + x) * TILE_H / 2 + map_d->y_off + cam_d->frame * iso_y(cam_d->rate * cam_d->x_dir, cam_d->rate * cam_d->y_dir) - 85;

	switch ((x + map_d->x_cur + y + map_d->y_cur) % 5) {
		case 0:
			break;
		case 1:
			rect.x -= 8;
			rect.y -= 4;
			break;
		case 2:
			rect.x += 8;
			rect.y += 4;
			break;
		case 3:
			rect.x -= 8;
			rect.y += 4;
			break;
		case 4:
			rect.x += 8;
			rect.y -= 4;
			break;
	}

	if (rect.x > -rect.w && rect.x < win_d->win_w  && rect.y > -rect.h && rect.y < win_d->win_h) {
		rect.x += map_d->x_off2;
		rect.y += map_d->y_off2;

		switch (map_d->objs[x + map_d->x_cur][y + map_d->y_cur]) {
			case 0:
				break;
			case 1:
				SDL_RenderCopy(rend, tex_d->obj_tex[TREE], NULL, &rect);
				break;
		}
	}
}

// draws the background
void draw_bg(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d) {
	for (int x = 0; x < map_d->win_sz; x++)
		for (int y = 0; y < map_d->win_sz; y++)
			draw_tile(rend, win_d, map_d, tex_d, cam_d, x, y);

	// if the camera is moving on the x-axis, draw an extra column
	if (cam_d->x_dir == 1) {
		for (int y = 0; y < map_d->win_sz; y++)
			draw_tile(rend, win_d, map_d, tex_d, cam_d, map_d->win_sz, y);
	} else if (cam_d->x_dir == -1) {
		for (int y = 0; y < map_d->win_sz; y++)
			draw_tile(rend, win_d, map_d, tex_d, cam_d, -1, y);
	}

	// if the camera is moving on the y-axis, draw an extra row 
	if (cam_d->y_dir == -1) {
		for (int x = 0; x < map_d->win_sz; x++)
			draw_tile(rend, win_d, map_d, tex_d, cam_d, x, -1);
	} else if (cam_d->y_dir == 1) {
		for (int x = 0; x < map_d->win_sz; x++)
			draw_tile(rend, win_d, map_d, tex_d, cam_d, x, map_d->win_sz);
	}
}

// draws the foreground
void draw_fg(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d) {
	for (int x = -2; x < (int) map_d->win_sz + 2; x++)
		for (int y = -2; y < (int) map_d->win_sz + 2; y++)
			draw_obj(rend, win_d, map_d, tex_d, cam_d, x, y);
}

// destroy textures
void texture_free(SDL_Texture** tex, size_t size) {
	for (size_t i = 0; i < size; i++)
		SDL_DestroyTexture(tex[i]);
}

// initialise the textures
int texture_init(SDL_Renderer* rend, tex_data* tex_d) {
	// load tile textures
	char path[50];
	for (int i = 0; i < 47; i++) {
		sprintf(path, "../resources/tiles/water/water_%02d.png", i);
		if (load_texture(rend, &tex_d->tile_tex[i], path)) {
			texture_free(tex_d->tile_tex, i);
			fprintf(stderr, "Failed to load water_%02d.png texture\n", i);
			return -1;
		}
	}

	if (load_texture(rend, &tex_d->tile_tex[GRASS], "../resources/tiles/grass.png")) {
		texture_free(tex_d->tile_tex, GRASS);
		fprintf(stderr, "Failed to load GRASS texture\n");
		return -1;
	}

	// load object textures
	if (load_texture(rend, &tex_d->obj_tex[TREE], "../resources/objects/tree.png")) {
		fprintf(stderr, "Failed to load TREE texture\n");
		return -1;
	} else {
		SDL_SetTextureBlendMode(tex_d->obj_tex[TREE], SDL_BLENDMODE_BLEND);
	}
	return 0;
}

// processes events
int event(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	int button, mouse_x, mouse_y;
	button = SDL_GetMouseState(&mouse_x, &mouse_y);
	
	int col = get_column(map_d, cam_d, mouse_x, mouse_y);
	int row = get_row(map_d, cam_d, mouse_x, mouse_y);
	if (col > 10 && row > 10 && col < map_d->map_sz - 10 && row < map_d->map_sz - 10) {
		if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
			map_d->tiles[col][row] = 1;
		else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
			map_d->tiles[col][row] = 0;
	}

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
					case SDLK_RIGHT:
						move_r(map_d, cam_d);
						break;
					case SDLK_UP:
						move_u(map_d, cam_d);
						break;
					case SDLK_LEFT:
						move_l(map_d, cam_d);
						break;
					case SDLK_DOWN:
						move_d(map_d, cam_d);
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

int animate(SDL_Window* win, SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d) {
	while (!event(win_d, map_d, cam_d)) {
		// clear the window
		SDL_RenderClear(rend);

		// render the background
		draw_bg(rend, win_d, map_d, tex_d, cam_d);

		// render the foreground
		draw_fg(rend, win_d, map_d, tex_d, cam_d);

		// display the window
		SDL_RenderPresent(rend);

		// wait 1/60th of a second
		SDL_Delay(1000/60);
	}

	return 0;
}

int main(void) {
	win_data win_data = {.win_w = 1280, .win_h = 720};
	
	map_data map_data;
	if (map_init(&win_data, &map_data)) {
		return 1;
	}

	map_data.tiles[20][20] = 1;

	tex_data tex_data;

	cam_data cam_data = {
		.frame = 0,
		.rate = 8,	// should be a power of 2
		.x_dir = 0,
		.y_dir = 0
	};

	// attempt to initialise graphics and timer system
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* win;
	if (make_window(&win, &win_data)) {
		closeSDL(NULL, NULL, &map_data, NULL);
		return 1;
	}
	
	SDL_Renderer* rend;
	if (make_renderer(win, &rend)) {
		closeSDL(win, NULL, &map_data, NULL);
		return 1;
	}

	if (texture_init(rend, &tex_data)) {
		closeSDL(win, rend, &map_data, NULL);
		return 1;
	}

	animate(win, rend, &win_data, &map_data, &tex_data, &cam_data);

	closeSDL(win, rend, &map_data, &tex_data);
	return 0;
}
