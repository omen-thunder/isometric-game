#include "game.h"

// returns the on-screen x position
int screen_x(map_data* map_d, cam_data* cam_d, int x, int y) {
	return (x - y) * TILE_W / 2 + map_d->off_x - cam_d->iso_x + cam_d->iso_y;
}

// returns the on-screen y position
int screen_y(map_data* map_d, cam_data* cam_d, int x, int y) {
	return (y + x) * TILE_H / 2 + map_d->off_y - (float) cam_d->iso_x / 2.0f - (float) cam_d->iso_y / 2.0f;
}

// draws the selector
void draw_selec(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y) {
	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;

	rect.x = screen_x(map_d, cam_d, x, y);
	rect.y = screen_y(map_d, cam_d, x, y);

	SDL_RenderCopy(rend, tex_d->menu_tex[SELECTOR], NULL, &rect);
}

// draws a single tile
void draw_tile(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y) {
	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;

	rect.x = screen_x(map_d, cam_d, x, y);
	rect.y = screen_y(map_d, cam_d, x, y);

	// checks if the tile is on the screen
	if (rect.x > -TILE_W && rect.x < win_d->win_w && rect.y > -TILE_H && rect.y < win_d->win_h) {
		// check what type of tile is being drawn
		switch (map_d->tiles[x + map_d->cur_x][y + map_d->cur_y]) {
			case 0:
				SDL_RenderCopy(rend, tex_d->tile_tex[GRASS], NULL, &rect);
				break;
			case 1:
				SDL_RenderCopy(rend, tex_d->tile_tex[water_index(map_d, x + map_d->cur_x, y + map_d->cur_y)], NULL, &rect);
				break;
		}
	}
}

// get the correct rect width and height for an object
int load_obj(map_data* map_d, tex_data* tex_d,  cam_data* cam_d, SDL_Rect* rect, int x, int y) {
	switch (map_d->objs[x + map_d->cur_x][y + map_d->cur_y]) {
		case EMPTY:
			return EMPTY;
		case TREE:
			SDL_QueryTexture(tex_d->obj_tex[TREE], NULL, NULL,  &rect->w, &rect->h);
			rect->w /= 5;
			rect->h /= 5;
			rect->x = screen_x(map_d, cam_d, x, y) + 30;
			rect->y = screen_y(map_d, cam_d, x, y) - 90;

			// pseudo-randomly adjust the x and y position
			switch ((x + map_d->cur_x + y + map_d->cur_y) % 5) {
				case 0:
					break;
				case 1:
					rect->x -= 8;
					rect->y -= 4;
					break;
				case 2:
					rect->x += 8;
					rect->y += 4;
					break;
				case 3:
					rect->x -= 8;
					rect->y += 4;
					break;
				case 4:
					rect->x += 8;
					rect->y -= 4;
					break;
			}		

			return TREE;
		case BASE:
			SDL_QueryTexture(tex_d->obj_tex[BASE], NULL, NULL,  &rect->w, &rect->h);
			rect->h /= rect->w / TILE_W;
			rect->w = TILE_W;
			rect->x = screen_x(map_d, cam_d, x, y);
			rect->y = screen_y(map_d, cam_d, x, y) - rect->h + TILE_H;
			return BASE;
		default:
			return EMPTY;
	}
}

// draws an object
void draw_obj(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y) {
	SDL_Rect rect;
	int tex_id = load_obj(map_d, tex_d, cam_d, &rect, x, y);

	// check if the object is empty
	if (tex_id == EMPTY)
		return;

	// checks if the object is on the screen
	if (rect.x > -rect.w && rect.x < win_d->win_w  && rect.y > -rect.h && rect.y < win_d->win_h)
		SDL_RenderCopy(rend, tex_d->obj_tex[tex_id], NULL, &rect);
}

// draws the background
void draw_bg(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d) {
	for (int x = -2; x < map_d->win_sz + 2; x++)
		for (int y = -2; y < map_d->win_sz + 2; y++)
			draw_tile(rend, win_d, map_d, tex_d, cam_d, x, y);
}

// draws the foreground
void draw_fg(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d) {
	for (int x = -4; x < map_d->win_sz + 4; x++)
		for (int y = -4; y < map_d->win_sz + 4; y++)
			draw_obj(rend, win_d, map_d, tex_d, cam_d, x, y);
}

// draws the menu
void draw_menu(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d) {
	draw_selec(rend, win_d, map_d, tex_d, cam_d, get_column(map_d, cam_d, win_d->mouse_x, win_d->mouse_y), get_row(map_d, cam_d, win_d->mouse_x, win_d->mouse_y));
}

// the main animation loop
int animate(SDL_Window* win, SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d) {
	while (!event(win_d, map_d, cam_d)) {
		win_d->old_t = win_d->pres_t;
		win_d->pres_t = SDL_GetTicks();

		// clear the window
		SDL_RenderClear(rend);

		// render the background
		draw_bg(rend, win_d, map_d, tex_d, cam_d);

		// render the foreground
		draw_fg(rend, win_d, map_d, tex_d, cam_d);

		// render the menu
		draw_menu(rend, win_d, map_d, tex_d, cam_d);

		// display the window
		SDL_RenderPresent(rend);
		
		/*
		if (win_d->pres_t < SDL_GetTicks())
			printf("FPS: %d\n", 1000 / (SDL_GetTicks() - win_d->pres_t));
		*/

		// wait 1/fps of a second
		//SDL_Delay(1000 / win_d->fps);
	};

	return 0;
}
