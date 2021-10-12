#include "game.h"

// returns the on-screen x position
int screen_x(map_data* map_d, cam_data* cam_d, int x, int y) {
	return (x - y) * TILE_W / 2 + map_d->x_off - cam_d->iso_x + cam_d->iso_y;
}

// returns the on-screen y position
int screen_y(map_data* map_d, cam_data* cam_d, int x, int y) {
	return (y + x) * TILE_H / 2 + map_d->y_off - cam_d->iso_x / 2 - cam_d->iso_y / 2;
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
		// for debugging
		rect.x += map_d->x_off2;
		rect.y += map_d->y_off2;

		// check what type of tile is being drawn
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

// draws an object
void draw_obj(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y) {
	SDL_Rect rect;
	SDL_QueryTexture(tex_d->obj_tex[TREE], NULL, NULL,  &rect.w, &rect.h);
	rect.w /= 5;
	rect.h /= 5;

	rect.x = screen_x(map_d, cam_d, x, y) + 30;
	rect.y = screen_y(map_d, cam_d, x, y) - 90;

	// pseudo-randomly adjust the x and y position
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

	// checks if the object is on the screen
	if (rect.x > -rect.w && rect.x < win_d->win_w  && rect.y > -rect.h && rect.y < win_d->win_h) {
		// add the offset of the current camera position on the map
		rect.x += map_d->x_off2;
		rect.y += map_d->y_off2;

		// check what type of object is being drawn
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

		// wait 1/fps of a second
		//SDL_Delay(1000 / win_d->fps);
	};

	return 0;
}
