#include "game.h"

// returns the on-screen x position
int screen_x(map_data* map_d, cam_data* cam_d, int x, int y) {
	return (x - y) * TILE_W / 2 + map_d->off_x - cam_d->iso_x + cam_d->iso_y;
}

// returns the on-screen y position
int screen_y(map_data* map_d, cam_data* cam_d, int x, int y) {
	return (y + x) * TILE_H / 2 + map_d->off_y - (float) cam_d->iso_x / 2.0f - (float) cam_d->iso_y / 2.0f;
}

// draws a part of a selector
void draw_selector_part(SDL_Renderer* rend, map_data* map_d, cam_data* cam_d, tex_data* tex_d, int tex_id, int x, int y) {
	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;
	rect.x = screen_x(map_d, cam_d, x, y);
	rect.y = screen_y(map_d, cam_d, x, y);
	SDL_RenderCopy(rend, tex_d->selector_tex[tex_id], NULL, &rect);
}

// draws the selector
void draw_selector(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, menu_data* menu_d, int x, int y) {
	int legal;
	switch (menu_d->mode) {
		case U_BASE:
			legal = 1;
			for (int i = x - 1; i <= x + 1; i++)
				for (int j = y - 1; j <= y + 1; j++)
					if (!editable(map_d, menu_d, i + map_d->cur_x, j + map_d->cur_y))
						legal = 0;
			if (legal) {
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W_R, x + 1, y - 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W_UR, x, y - 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W_U, x - 1, y - 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W_UL, x - 1, y);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W_L, x - 1, y + 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W_DL, x, y + 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W_D, x + 1, y + 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W_DR, x + 1, y);
			} else {
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R_R, x + 1, y - 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R_UR, x, y - 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R_U, x - 1, y - 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R_UL, x - 1, y);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R_L, x - 1, y + 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R_DL, x, y + 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R_D, x + 1, y + 1);
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R_DR, x + 1, y);
			}

			break;
		default:
			if (editable(map_d, menu_d, x + map_d->cur_x, y + map_d->cur_y))
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_W, x, y);
			else
				draw_selector_part(rend, map_d, cam_d, tex_d, T_SELECTOR_R, x, y);

	}
}

// draws a single tile
void draw_tile(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y) {
	// check if the object is within the map array
	if (x + map_d->cur_x >= map_d->map_sz || x + map_d->cur_x < 0
			|| y + map_d->cur_y >= map_d->map_sz || y + map_d->cur_y < 0)
		return;

	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;

	rect.x = screen_x(map_d, cam_d, x, y);
	rect.y = screen_y(map_d, cam_d, x, y);

	// checks if the tile is on the screen and within the map array
	if (rect.x > -TILE_W && rect.x < win_d->win_w && rect.y > -TILE_H && rect.y < win_d->win_h) {
		// check what type of tile is being drawn
		switch (get_tile_type(map_d, x + map_d->cur_x, y + map_d->cur_y)) {
			case WATER:
				SDL_RenderCopy(rend, tex_d->water_tex[get_tile_tex(map_d, x + map_d->cur_x, y + map_d->cur_y)], NULL, &rect);
				break;
			case GRASS:
				SDL_RenderCopy(rend, tex_d->grass_tex[get_tile_tex(map_d, x + map_d->cur_x, y + map_d->cur_y)], NULL, &rect);
				break;
		}
	}
}

// get the correct rect width and height for an object
void load_obj(map_data* map_d, tex_data* tex_d,  cam_data* cam_d, SDL_Rect* rect, int x, int y) {
	switch (get_obj_type(map_d, x + map_d->cur_x, y + map_d->cur_y)) {
		case TREE:
			rect->w = ZOOM_SCALE(350) / 5;
			rect->h = ZOOM_SCALE(720) / 5;
			rect->x = screen_x(map_d, cam_d, x, y) + ZOOM_SCALE(60);
			rect->y = screen_y(map_d, cam_d, x, y) - ZOOM_SCALE(90);

			// pseudo-randomly adjust the x and y position
			switch ((x + map_d->cur_x + y + map_d->cur_y) % 5) {
				case 0:
					break;
				case 1:
					rect->x -= ZOOM_SCALE(8);
					rect->y -= ZOOM_SCALE(4);
					break;
				case 2:
					rect->x += ZOOM_SCALE(8);
					rect->y += ZOOM_SCALE(4);
					break;
				case 3:
					rect->x -= ZOOM_SCALE(8);
					rect->y += ZOOM_SCALE(4);
					break;
				case 4:
					rect->x += ZOOM_SCALE(8);
					rect->y -= ZOOM_SCALE(4);
					break;
			}		

			break;
		case BASE:
			rect->w = ZOOM_SCALE(549) / 2.25f;
			rect->h = ZOOM_SCALE(882) / 2.25f;
			rect->x = screen_x(map_d, cam_d, x, y) - ZOOM_SCALE(60);
			rect->y = screen_y(map_d, cam_d, x, y) - ZOOM_SCALE(300);
			break;
		case WALL:
			rect->w = ZOOM_SCALE(96); 
			rect->h = ZOOM_SCALE(73); 
			rect->x = screen_x(map_d, cam_d, x, y) + ZOOM_SCALE(15);
			rect->y = screen_y(map_d, cam_d, x, y) - ZOOM_SCALE(15);
			break;
	}
}

// draws an object
void draw_obj(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y) {
	// check if the object is within the map array
	if (x + map_d->cur_x >= map_d->map_sz || x + map_d->cur_x < 0
			|| y + map_d->cur_y >= map_d->map_sz || y + map_d->cur_y < 0)
		return;

	// check if the object is empty
	if (get_obj_type(map_d, x + map_d->cur_x, y + map_d->cur_y) == EMPTY
			|| get_obj_type(map_d, x + map_d->cur_x, y + map_d->cur_y) == OCCUPIED)
		return;

	SDL_Rect rect;
	load_obj(map_d, tex_d, cam_d, &rect, x, y);

	// checks if the object is on the screen
	if (rect.x > -rect.w && rect.x < win_d->win_w  && rect.y > -rect.h && rect.y < win_d->win_h) {
		// check what type of object is being drawn
		switch (get_obj_type(map_d, x + map_d->cur_x, y + map_d->cur_y)) {
			case WALL:
				SDL_RenderCopy(rend, tex_d->wall_tex[get_obj_tex(map_d, x + map_d->cur_x, y + map_d->cur_y)], NULL, &rect);
				break;
			default:
				SDL_RenderCopy(rend, tex_d->obj_tex[get_obj_tex(map_d, x + map_d->cur_x, y + map_d->cur_y)], NULL, &rect);
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
void draw_menu(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, menu_data* menu_d) {
	if (menu_d->mode != U_DEFAULT)
		draw_selector(rend, win_d, map_d, tex_d, cam_d, menu_d, get_column(map_d, cam_d, win_d->mouse_x, win_d->mouse_y), get_row(map_d, cam_d, win_d->mouse_x, win_d->mouse_y));
}

// the main animation loop
int animate(SDL_Window* win, SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, menu_data* menu_d) {
	while (event(win_d, map_d, cam_d, menu_d)) {
		win_d->old_t = win_d->pres_t;
		win_d->pres_t = SDL_GetTicks();

		// clear the window
		SDL_RenderClear(rend);

		// render the background
		draw_bg(rend, win_d, map_d, tex_d, cam_d);

		// render the foreground
		draw_fg(rend, win_d, map_d, tex_d, cam_d);

		// render the menu
		draw_menu(rend, win_d, map_d, tex_d, cam_d, menu_d);

		// display the window
		SDL_RenderPresent(rend);
		
		/*
		if (win_d->pres_t < SDL_GetTicks())
			printf("FPS: %d\n", 1000 / (SDL_GetTicks() - win_d->pres_t));
		*/

		// wait 1/fps of a second
		//SDL_Delay(1000 / win_d->fps);
	}

	return 0;
}
