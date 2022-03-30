#include "game.h"

#define Y_INTER(x, y, off) ((y) - ((x) / 2) - (off))

void set_rect_xy (Settings* settings_p, Data* data_p, int col, int row, Sprite* sprite_p, SDL_Rect* rect_p) {
	double iso_x = (data_p->cam_iso_x + cos(data_p->cam_view / 2.0 * M_PI) * ZOOM_SCALE(sprite_p->adj_x) + sin(data_p->cam_view / 2.0 * M_PI) * ZOOM_SCALE(sprite_p->adj_y));
	double iso_y = (data_p->cam_iso_y + sin(data_p->cam_view / 2.0 * M_PI) * ZOOM_SCALE(sprite_p->adj_x) - cos(data_p->cam_view / 2.0 * M_PI) * ZOOM_SCALE(sprite_p->adj_y));
	/*
	if (col == 5 && row == 5)
		printf("iso_x: %f, iso_y: %f\n", iso_x, iso_y);
	*/

	rect_p->x = (col - row) * ZOOM_SCALE(TILE_W) / 2 + OFF_X - iso_x + iso_y + ZOOM_SCALE(data_p->tab_rect_x[sprite_p->tab_id]);
	rect_p->y = (row + col) * ZOOM_SCALE(TILE_H) / 2 + OFF_Y - iso_x / 2.0 - iso_y / 2.0 + ZOOM_SCALE(data_p->tab_rect_y[sprite_p->tab_id]);
}

void rend_sprite(SDL_Renderer* rend, Settings* settings_p, Data* data_p, int col, int row, Sprite* sprite_p) {
	if (sprite_p->tab_id == L_EMPTY)
		return;

	SDL_Rect rect = { .w = ZOOM_SCALE(data_p->tab_rect_w[sprite_p->tab_id]),
					  .h = ZOOM_SCALE(data_p->tab_rect_h[sprite_p->tab_id])};

	set_rect_xy(settings_p, data_p, col, row, sprite_p, &rect);

	// checks if the object is on the screen
	if (rect.x < -rect.w || rect.x > settings_p->win_w || rect.y < -rect.h || rect.y > settings_p->win_h)
		return;

	unsigned tex_index = sprite_p->tex_index;

	// rotate the texture
	if (sprite_p->tab_id == L_WATER || sprite_p->tab_id == L_GRASS || sprite_p->tab_id == L_WALL) {
		unsigned mask = 0b11111111;
		unsigned temp = sprite_p->tex_index;
		tex_index >>= 2 * data_p->cam_view;
		temp <<= 8 - 2 * data_p->cam_view;
		tex_index |= temp;
		tex_index &= mask;
	}

	SDL_RenderCopy(rend, data_p->tab_tex[sprite_p->tab_id][tex_index], NULL, &rect);
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
	return floor(Y_INTER((float) (-(OFF_X + data_p->cam_iso_x + data_p->cam_iso_y)),
			Y_INTER(x, y, OFF_Y + data_p->cam_iso_x / 2 - data_p->cam_iso_y / 2),
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
	return floor(Y_INTER((float) (OFF_X - data_p->cam_iso_x - data_p->cam_iso_y),
			Y_INTER(-x, y, OFF_Y - data_p->cam_iso_x / 2 + data_p->cam_iso_y / 2),
			ZOOM_SCALE(TILE_H) / 2.0f) / (float) ZOOM_SCALE(TILE_H));
}

void rend_selector(SDL_Renderer* rend, Settings* settings_p, Textures* textures_p, Data* data_p) {
	int col = data_p->mouse_col;
	int row = data_p->mouse_row;
	int size = data_p->menu_selec_sz;
	int legal = 1;
	for (int i = data_p->mouse_adj_col - size; i <= data_p->mouse_adj_col + size; i++)
		for (int j = data_p->mouse_adj_row - size; j <= data_p->mouse_adj_row + size; j++)
			if (!editable(settings_p, data_p, i, j))
				legal = 0;

	Sprite sprite_ur = {.type = MENU,
						.tab_id = L_SELECTOR,
						.tex_index = T_SEL_W_UR,
						.adj_x = 0,
						.adj_y = 0};

	Sprite sprite_ul = {.type = MENU,
						.tab_id = L_SELECTOR,
						.tex_index = T_SEL_W_UL,
						.adj_x = 0,
						.adj_y = 0};

	Sprite sprite_dl = {.type = MENU,
						.tab_id = L_SELECTOR,
						.tex_index = T_SEL_W_DL,
						.adj_x = 0,
						.adj_y = 0};

	Sprite sprite_dr = {.type = MENU,
						.tab_id = L_SELECTOR,
						.tex_index = T_SEL_W_DR,
						.adj_x = 0,
						.adj_y = 0};

	if (!legal) {
		sprite_ur.tex_index += 4;
		sprite_ul.tex_index += 4;
		sprite_dl.tex_index += 4;
		sprite_dr.tex_index += 4;
	}

	for (int i = col - size; i <= col + size; i++) {
			rend_sprite(rend, settings_p, data_p, i, row + size, &sprite_dl);
			rend_sprite(rend, settings_p, data_p, i, row - size, &sprite_ur);
		}

		for (int j = row - size; j <= row + size; j++) {
			rend_sprite(rend, settings_p, data_p, col + size, j, &sprite_dr);
			rend_sprite(rend, settings_p, data_p, col - size, j, &sprite_ul);
		}
}

int animate(SDL_Window* win, SDL_Renderer* rend, Settings* settings_p, Textures* textures_p, Data* data_p) {
	while (event(settings_p, data_p)) {
		data_p->old_t = data_p->pres_t;
		data_p->pres_t = SDL_GetTicks();
		data_p->mouse_prev_x = data_p->mouse_x;
		data_p->mouse_prev_y = data_p->mouse_y;
		data_p->mouse_button = SDL_GetMouseState(&data_p->mouse_x, &data_p->mouse_y);
		data_p->mouse_col = get_column(settings_p, data_p, data_p->mouse_x, data_p->mouse_y);
		data_p->mouse_row = get_row(settings_p, data_p, data_p->mouse_x, data_p->mouse_y);
		data_p->mouse_adj_col = data_p->mouse_col;
		data_p->mouse_adj_row = data_p->mouse_row;
		(*data_p->adj_arr[data_p->cam_view]) (data_p, &data_p->mouse_adj_col, &data_p->mouse_adj_row);

		SDL_RenderClear(rend);

		int adj_col = 0;
		int adj_row = 0;

		// render the background
		for (int i = -GAP; i < data_p->win_sz + GAP; i++)
			for (int j = -GAP; j < data_p->win_sz + GAP; j++) {
				int adj_col = i;
				int adj_row = j;
				(*data_p->adj_arr[data_p->cam_view]) (data_p, &adj_col, &adj_row);
				rend_sprite(rend, settings_p, data_p, i, j, &data_p->map_tiles[adj_col][adj_row]);
			}

		// render the npcs
		Npc* current = data_p->npc_head;
		while (current) {
			adj_col = current->start_col;
			adj_row = current->start_row;
			(*data_p->unadj_arr[data_p->cam_view]) (data_p, &adj_col, &adj_row);
			rend_sprite(rend, settings_p, data_p, adj_col, adj_row, &current->sprite);
			current = current->next;
		}

		// render the foreground
		for (int i = -GAP; i < data_p->win_sz + GAP; i++)
			for (int j = -GAP; j < data_p->win_sz + GAP; j++) {
				int adj_col = i;
				int adj_row = j;
				(*data_p->adj_arr[data_p->cam_view]) (data_p, &adj_col, &adj_row);
				rend_sprite(rend, settings_p, data_p, i, j, &data_p->map_objs[adj_col][adj_row]);
			}

		// render the menu
		if (data_p->menu_mode != U_DEFAULT)
			rend_selector(rend, settings_p, textures_p, data_p);

		SDL_RenderPresent(rend);

		/*
		if (win_d->pres_t < SDL_GetTicks())
			printf("FPS: %d\n", 1000 / (SDL_GetTicks() - win_d->pres_t));
		*/

		//SDL_Delay(1000 / win_d->fps);

		mouse(settings_p, data_p);
		cam_pan(settings_p, data_p);
		move_npcs(settings_p, data_p, data_p->npc_head);
	}

	return 0;
}
