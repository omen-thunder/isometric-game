#include "game.h"

#define Y_INTER(x, y, off) ((y) - ((x) / 2) - (off))
#define SCREEN_X(x, y) ((x - y) * ZOOM_SCALE(TILE_W) / 2 + OFF_X - data_p->iso_x + data_p->iso_y)
#define SCREEN_Y(x, y) ((y + x) * ZOOM_SCALE(TILE_H) / 2 + OFF_Y - (float) data_p->iso_x / 2.0f - (float) data_p->iso_y / 2.0f)

// draws a part of a selector
/*
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
*/

/*
void draw_npc(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, npc* npcp) {
	// calculate the npc texture coordinates based on the current view
	int pos_x = 0;
	int pos_y = 0;
	switch (map_d->view) {
		case 0:
			pos_x = npcp->x + map_d->cur_x;
			pos_y = npcp->y + map_d->cur_y;
			break;
		case 1:
			pos_x = npcp->y + map_d->cur_x;
			pos_y = map_d->win_sz - 1 - npcp->x + map_d->cur_y;
			break;
		case 2:
			pos_x = map_d->win_sz - 1 - npcp->x + map_d->cur_x;
			pos_y = map_d->win_sz - 1 - npcp->y + map_d->cur_y;
			break;
		case 3:
			pos_x = map_d->win_sz - 1 - npcp->y + map_d->cur_x;
			pos_y = npcp->x + map_d->cur_y;
			break;
	}

	// get the texture array and texture index
	SDL_Rect rect;
	rect.w = ZOOM_SCALE(96);
	rect.h = ZOOM_SCALE(73);
	rect.x = screen_x(map_d, cam_d, pos_x, pos_y) + ZOOM_SCALE(15);
	rect.y = screen_y(map_d, cam_d, pos_x, pos_y) - ZOOM_SCALE(15);

	// checks if the object is on the screen
	if (rect.x < -rect.w || rect.x > win_d->win_w || rect.y < -rect.h || rect.y > win_d->win_h)
		return;

	// render the object
	SDL_RenderCopy(rend, tex_d->pleb_tex[0], NULL, &rect);
}
*/

void rend_sprite(SDL_Renderer* rend, Settings* settings_p, Data* data_p, Sprite** sprite_arr, int col, int row) {
	Sprite sprite = sprite_arr[col][row];
	if (sprite.tab_id == L_EMPTY)
		return;

	SDL_Rect rect;
	rect.w = data_p->tab_rect_w[sprite.tab_id];
	rect.h = data_p->tab_rect_h[sprite.tab_id];
	switch (data_p->view) {
		case 0:
			rect.x = SCREEN_X(col + data_p->cur_x, row + data_p->cur_y) + data_p->tab_rect_x[sprite.tab_id];
			rect.y = SCREEN_Y(col + data_p->cur_x, row + data_p->cur_y) + data_p->tab_rect_y[sprite.tab_id];
			break;
		case 1:
			rect.x = SCREEN_X(row + data_p->cur_x, data_p->win_sz - 1 - col + data_p->cur_y) + data_p->tab_rect_y[sprite.tab_id];
			rect.y = SCREEN_Y(row + data_p->cur_x, data_p->win_sz - 1 - col + data_p->cur_y) - data_p->tab_rect_x[sprite.tab_id];
			break;
		case 2:
			rect.x = SCREEN_X(data_p->win_sz - 1 - col + data_p->cur_x, data_p->win_sz - 1 - row + data_p->cur_y) - data_p->tab_rect_x[sprite.tab_id];
			rect.y = SCREEN_Y(data_p->win_sz - 1 - col + data_p->cur_x, data_p->win_sz - 1 - row + data_p->cur_y) - data_p->tab_rect_y[sprite.tab_id];
			break;
		case 3:
			rect.x = SCREEN_X(data_p->win_sz - 1 - row + data_p->cur_x, col + data_p->cur_y) - data_p->tab_rect_y[sprite.tab_id];
			rect.y = SCREEN_Y(data_p->win_sz - 1 - row + data_p->cur_x, col + data_p->cur_y) + data_p->tab_rect_x[sprite.tab_id];
			break;
		default:
			rect.x = SCREEN_X(col + data_p->cur_x, row + data_p->cur_y) + data_p->tab_rect_x[sprite.tab_id];
			rect.y = SCREEN_Y(col + data_p->cur_x, row + data_p->cur_y) + data_p->tab_rect_y[sprite.tab_id];
	}

	/*
	if (sprite.type == GRASS)
		printf("tab_id %d, index %u, w %d, h %d, x %d, y% d\n", sprite.tab_id, sprite.tex_index, rect.w, rect.h, rect.x, rect.y);
	*/

	// checks if the object is on the screen
	if (rect.x < -rect.w || rect.x > settings_p->win_w || rect.y < -rect.h || rect.y > settings_p->win_h)
		return;

	// rotate the texture
	unsigned mask = 0b11111111;
	unsigned tex_index = sprite.tex_index;
	unsigned temp = tex_index;
	tex_index >>= 2 * data_p->view;
	temp <<= 2 * (4 - data_p->view);
	tex_index |= temp;
	tex_index &= mask;

	SDL_RenderCopy(rend, data_p->tab_tex[sprite.tab_id][tex_index], NULL, &rect);
}

// returns the row the given point is in
int get_row(Settings* settings_p, Data* data_p, int x, int y) {
	/* Derived from the cartesian equation y = x / 2 + b.
	To find the row, solve for the y-intercept of the above line
	using the mouse's x and y position and subtract the y-axis
	offset of the background. Using this y-intercept and the x-axis
	offset of the background, solve for the y value in the original
	equation for the line. Divide this y value by the tile height
	to get the row index */
	return floor(Y_INTER((float) (-(OFF_X + data_p->iso_x + data_p->iso_y)),
			Y_INTER(x, y, OFF_Y + data_p->iso_x / 2 - data_p->iso_y / 2),
			ZOOM_SCALE(TILE_H) / -2.0f) / (float) ZOOM_SCALE(TILE_H));
}

// returns the column the given point is in
int get_column(Settings* settings_p, Data* data_p, int x, int y) {
	/* Derived from the cartesian equation y = -x / 2 + b.
	To find the column, solve for the y-intercept of the above line
	using the mouse's x and y position and subtract the y-axis
	offset of the background. Using this y-intercept and the x-axis
	offset of the background, solve for the y value in the original
	equation for the line. Divide this y value by the tile height
	to get the column index */
	return floor(Y_INTER((float) (OFF_X - data_p->iso_x - data_p->iso_y),
			Y_INTER(-x, y, OFF_Y - data_p->iso_x / 2 + data_p->iso_y / 2),
			ZOOM_SCALE(TILE_H) / 2.0f) / (float) ZOOM_SCALE(TILE_H));
}

int animate(SDL_Window* win, SDL_Renderer* rend, Settings* settings_p, Textures* textures_p, Maps* maps_p, Data* data_p) {
	while (event(settings_p, data_p)) {
		data_p->old_t = data_p->pres_t;
		data_p->pres_t = SDL_GetTicks();
		data_p->mouse_button = SDL_GetMouseState(&data_p->mouse_x, &data_p->mouse_y);
		data_p->mouse_col = get_column(settings_p, data_p, data_p->mouse_x, data_p->mouse_y);
		data_p->mouse_row = get_row(settings_p, data_p, data_p->mouse_x, data_p->mouse_y);
		switch (data_p->view) {
			case 0:
				data_p->mouse_adj_col = get_column(settings_p, data_p, data_p->mouse_x, data_p->mouse_y) + data_p->cur_x;
				data_p->mouse_adj_row = get_row(settings_p, data_p, data_p->mouse_x, data_p->mouse_y) + data_p->cur_y;
				break;
			case 1:
				data_p->mouse_adj_col = get_row(settings_p, data_p, data_p->mouse_x, data_p->mouse_y) + data_p->cur_x;
				data_p->mouse_adj_row = data_p->win_sz - 1 - get_column(settings_p, data_p, data_p->mouse_x, data_p->mouse_y) + data_p->cur_y;
				break;
			case 2:
				data_p->mouse_adj_col = data_p->win_sz - 1 - get_column(settings_p, data_p, data_p->mouse_x, data_p->mouse_y) + data_p->cur_x;
				data_p->mouse_adj_row = data_p->win_sz - 1 - get_row(settings_p, data_p, data_p->mouse_x, data_p->mouse_y) + data_p->cur_y;
				break;
			case 3:
				data_p->mouse_adj_col = data_p->win_sz - 1 - get_row(settings_p, data_p, data_p->mouse_x, data_p->mouse_y) + data_p->cur_x;
				data_p->mouse_adj_row = get_column(settings_p, data_p, data_p->mouse_x, data_p->mouse_y) + data_p->cur_y;
				break;
		}

		SDL_RenderClear(rend);

		// render the background
		for (int i = -GAP; i < data_p->win_sz + GAP; i++)
			for (int j = -GAP; j < data_p->win_sz + GAP; j++)
				rend_sprite(rend, settings_p, data_p, maps_p->tiles, i + data_p->cur_x, j + data_p->cur_y);

		// render the foreground
		for (int i = -GAP; i < data_p->win_sz + GAP; i++)
			for (int j = -GAP; j < data_p->win_sz + GAP; j++)
				rend_sprite(rend, settings_p, data_p, maps_p->objs, i + data_p->cur_x, j + data_p->cur_y);

		// draws the menu
		/*
		if (data_p->mode != U_DEFAULT)
			draw_selector(rend, win_d, map_d, tex_d, cam_d, menu_d, get_column(map_d, cam_d, win_d->mouse_x, win_d->mouse_y), get_row(map_d, cam_d, win_d->mouse_x, win_d->mouse_y));
		*/

		// render the npcs
		/*
		npc* npcp = map_d->npc_head;
		while (npcp) {
			printf("npc!\n");
			draw_npc(rend, win_d, map_d, tex_d, cam_d, npcp);
			npcp = npcp->next;
		}
		*/

		// render the menu

		SDL_RenderPresent(rend);

		/*
		if (win_d->pres_t < SDL_GetTicks())
			printf("FPS: %d\n", 1000 / (SDL_GetTicks() - win_d->pres_t));
		*/

		//SDL_Delay(1000 / win_d->fps);

		mouse(settings_p, maps_p, data_p);
		cam_pan(settings_p, maps_p, data_p);
	}

	return 0;
}
