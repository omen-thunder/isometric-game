#include "game.h"

#define Y_INTER(x, y, off) ((y) - ((x) / 2) - (off))
#define SCREEN_X(x, y) ((x - y) * ZOOM_SCALE(TILE_W) / 2 + OFF_X - data_p->iso_x + data_p->iso_y)
#define SCREEN_Y(x, y) ((y + x) * ZOOM_SCALE(TILE_H) / 2 + OFF_Y - data_p->iso_x / 2.0f - data_p->iso_y / 2.0f)
#define RANDOM_X(x, y) (((17 * x +  23 * y + 84) ^ 1734859) % 3 - 1)
#define RANDOM_Y(x, y) (((19 * x + 27 * y + 55) ^ 8234594) % 3 - 1)

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

void rend_sprite(SDL_Renderer* rend, Settings* settings_p, Data* data_p, int col, int row, int tab_id, int tex_index, int rand_x, int rand_y) {
	SDL_Rect rect = {.w = ZOOM_SCALE(data_p->tab_rect_w[tab_id]),
					 .h = ZOOM_SCALE(data_p->tab_rect_h[tab_id]),
					 .x = SCREEN_X(col, row) + ZOOM_SCALE(data_p->tab_rect_x[tab_id]),
					 .y = SCREEN_Y(col, row) + ZOOM_SCALE(data_p->tab_rect_y[tab_id])};

	if (tab_id == L_TREE)
		switch (data_p->view) {
			case 0:
				rect.x += rand_x * ZOOM_SCALE(8);
				rect.y += rand_y * ZOOM_SCALE(4);
				break;
			case 1:
				rect.x += rand_y * ZOOM_SCALE(8);
				rect.y -= rand_x * ZOOM_SCALE(4);
				break;
			case 2:
				rect.x -= rand_x * ZOOM_SCALE(8);
				rect.y -= rand_y * ZOOM_SCALE(4);
				break;
			case 3:
				rect.x -= rand_y * ZOOM_SCALE(8);
				rect.y += rand_x * ZOOM_SCALE(4);
				break;
		}

	if (tab_id == L_EMPTY)
		return;

	// checks if the object is on the screen
	if (rect.x < -rect.w || rect.x > settings_p->win_w || rect.y < -rect.h || rect.y > settings_p->win_h)
		return;

	// rotate the texture
	if (tab_id == L_WATER || tab_id == L_GRASS || tab_id == L_WALL) {
		unsigned mask = 0b11111111;
		unsigned temp = tex_index;
		tex_index >>= 2 * data_p->view;
		temp <<= 8 - 2 * data_p->view;
		tex_index |= temp;
		tex_index &= mask;
	}

	SDL_RenderCopy(rend, data_p->tab_tex[tab_id][tex_index], NULL, &rect);
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

void rend_selector(SDL_Renderer* rend, Settings* settings_p, Textures* textures_p, Maps* maps_p, Data* data_p) {
	int col = data_p->mouse_col;
	int row = data_p->mouse_row;
	int size = data_p->selector_sz;
	int legal = 1;
	for (int i = data_p->mouse_adj_col - size; i <= data_p->mouse_adj_col + size; i++)
		for (int j = data_p->mouse_adj_row - size; j <= data_p->mouse_adj_row + size; j++)
			if (!editable(settings_p, maps_p, i, j))
				legal = 0;

	if (legal) {
		for (int i = col - size; i <= col + size; i++) {
			rend_sprite(rend, settings_p, data_p, i, row + size, L_SELECTOR, T_SEL_W_DL, 0, 0);
			rend_sprite(rend, settings_p, data_p, i, row - size, L_SELECTOR, T_SEL_W_UR, 0, 0);
		}

		for (int j = row - size; j <= row + size; j++) {
			rend_sprite(rend, settings_p, data_p, col + size, j, L_SELECTOR, T_SEL_W_DR, 0, 0);
			rend_sprite(rend, settings_p, data_p, col - size, j, L_SELECTOR, T_SEL_W_UL, 0, 0);
		}
	} else {
		for (int i = col - size; i <= col + size; i++) {
			rend_sprite(rend, settings_p, data_p, i, row + size, L_SELECTOR, T_SEL_R_DL, 0, 0);
			rend_sprite(rend, settings_p, data_p, i, row - size, L_SELECTOR, T_SEL_R_UR, 0, 0);
		}

		for (int j = row - size; j <= row + size; j++) {
			rend_sprite(rend, settings_p, data_p, col + size, j, L_SELECTOR, T_SEL_R_DR, 0, 0);
			rend_sprite(rend, settings_p, data_p, col - size, j, L_SELECTOR, T_SEL_R_UL, 0, 0);
		}
	}
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
				data_p->mouse_adj_col = data_p->mouse_col + data_p->cur_x;
				data_p->mouse_adj_row = data_p->mouse_row + data_p->cur_y;
				break;
			case 1:
				data_p->mouse_adj_col = data_p->mouse_row + data_p->cur_x;
				data_p->mouse_adj_row = data_p->win_sz - 1 - data_p->mouse_col + data_p->cur_y;
				break;
			case 2:
				data_p->mouse_adj_col = data_p->win_sz - 1 - data_p->mouse_col + data_p->cur_x;
				data_p->mouse_adj_row = data_p->win_sz - 1 - data_p->mouse_row + data_p->cur_y;
				break;
			case 3:
				data_p->mouse_adj_col = data_p->win_sz - 1 - data_p->mouse_row + data_p->cur_x;
				data_p->mouse_adj_row = data_p->mouse_col + data_p->cur_y;
				break;
		}

		SDL_RenderClear(rend);

		int col_adj = 0;
		int row_adj = 0;

		// render the background
		for (int i = -GAP; i < data_p->win_sz + GAP; i++)
			for (int j = -GAP; j < data_p->win_sz + GAP; j++) {
				switch (data_p->view) {
					case 0:
						col_adj = i + data_p->cur_x;
						row_adj = j + data_p->cur_y;
						break;
					case 1:
						col_adj = j + data_p->cur_x;
						row_adj = data_p->win_sz - 1 - i + data_p->cur_y;
						break;
					case 2:
						col_adj = data_p->win_sz - 1 - i + data_p->cur_x;
						row_adj = data_p->win_sz - 1 - j + data_p->cur_y;
						break;
					case 3:
						col_adj = data_p->win_sz - 1 - j + data_p->cur_x;
						row_adj = i + data_p->cur_y;
						break;
				}
				rend_sprite(rend, settings_p, data_p, i, j, maps_p->tiles[col_adj][row_adj].tab_id, maps_p->tiles[col_adj][row_adj].tex_index, 0, 0);
			}

		// render the foreground
		for (int i = -GAP; i < data_p->win_sz + GAP; i++)
			for (int j = -GAP; j < data_p->win_sz + GAP; j++) {
				switch (data_p->view) {
					case 0:
						col_adj = i + data_p->cur_x;
						row_adj = j + data_p->cur_y;
						break;
					case 1:
						col_adj = j + data_p->cur_x;
						row_adj = data_p->win_sz - 1 - i + data_p->cur_y;
						break;
					case 2:
						col_adj = data_p->win_sz - 1 - i + data_p->cur_x;
						row_adj = data_p->win_sz - 1 - j + data_p->cur_y;
						break;
					case 3:
						col_adj = data_p->win_sz - 1 - j + data_p->cur_x;
						row_adj = i + data_p->cur_y;
						break;
				}
				rend_sprite(rend, settings_p, data_p, i, j, maps_p->objs[col_adj][row_adj].tab_id, maps_p->objs[col_adj][row_adj].tex_index, RANDOM_X(col_adj, row_adj), RANDOM_Y(col_adj, row_adj));
			}

		// draws the menu
		if (data_p->mode != U_DEFAULT)
			rend_selector(rend, settings_p, textures_p, maps_p, data_p);

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
