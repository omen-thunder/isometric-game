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
	for (int x = -2; x < map_d->win_sz + 2; x++)
		for (int y = -2; y < map_d->win_sz + 2; y++)
			draw_obj(rend, win_d, map_d, tex_d, cam_d, x, y);
}

// destroy textures
void texture_free(SDL_Texture** tex, size_t size) {
	for (int i = 0; i < size; i++)
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
