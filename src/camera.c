#include "game.h"

#define SCREEN_L (settings_p->win_w / settings_p->pan_sens)
#define SCREEN_R (settings_p->win_w - SCREEN_L)
#define SCREEN_U (settings_p->win_h / settings_p->pan_sens)
#define SCREEN_D (settings_p->win_h - SCREEN_U)

void view_adj(Data* data_p, int* x, int* y) {
	int adj_x = 0;
	int adj_y = 0;
	switch (data_p->cam_view) {
		case 0:
			adj_x = *x;
			adj_y = *y;
			break;
		case 1:
			adj_x = *y;
			adj_y = -*x;
			break;
		case 2:
			adj_x = -*x;
			adj_y = -*y;
			break;
		case 3:
			adj_x = -*y;
			adj_y = *x;
			break;
	}

	*x = adj_x;
	*y = adj_y;
}

int legal(Settings* settings_p, Data* data_p, int x, int y) {
	view_adj(data_p, &x, &y);

	// UR when view == 0
	if (y < 0 && data_p->map_cur_y <= GAP)
		return 0;
	// UL when view == 0
	if (x < 0 && data_p->map_cur_x <= GAP)
		return 0;
	// DL when view == 0
	if (y > 0 && data_p->map_cur_y >= settings_p->map_sz - data_p->win_sz - GAP)
		return 0;
	// DR when view == 0
	if (x > 0 && data_p->map_cur_x >= settings_p->map_sz - data_p->win_sz - GAP)
		return 0;

	return 1;
}

float speed(Settings* settings_p, Data* data_p) {
	float right = (float) (data_p->mouse_x - SCREEN_R) * settings_p->pan_accel / (float) (settings_p->win_w - SCREEN_R - 1) + 1;
	float up = (float) (SCREEN_U - data_p->mouse_y) * settings_p->pan_accel / (float) SCREEN_U + 1;
	float left = (float) (SCREEN_L - data_p->mouse_x) * settings_p->pan_accel / (float) SCREEN_L + 1;
	float down = (float) (data_p->mouse_y - SCREEN_D) * settings_p->pan_accel / (float) (settings_p->win_h - SCREEN_D - 1) + 1;
	return fmax(fmax(right, left), fmax(up, down)) * BUF_SZ / settings_p->pan_rate;
}

int pan(Settings* settings_p, Data* data_p) {
	if ((data_p->mouse_x >= SCREEN_R && data_p->mouse_y <= SCREEN_U * 2)
			|| (data_p->mouse_x >= SCREEN_R - SCREEN_L && data_p->mouse_y <= SCREEN_U)) {
		data_p->cam_buf_y -= (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p);
		return 0;
	} else if ((data_p->mouse_x <= SCREEN_L && data_p->mouse_y <= SCREEN_U * 2)
			|| (data_p->mouse_x <= SCREEN_L * 2 && data_p->mouse_y <= SCREEN_U)) {
		data_p->cam_buf_x -= (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p);
		return 0;
	} else if ((data_p->mouse_x <= SCREEN_L && data_p->mouse_y >= SCREEN_D - SCREEN_U)
			|| (data_p->mouse_x <= SCREEN_L * 2 && data_p->mouse_y >= SCREEN_D)) {
		data_p->cam_buf_y += (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p);
		return 0;
	} else if ((data_p->mouse_x >= SCREEN_R && data_p->mouse_y >= SCREEN_D - SCREEN_U)
			|| (data_p->mouse_x >= SCREEN_R - SCREEN_L && data_p->mouse_y >= SCREEN_D)) {
		data_p->cam_buf_x += (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p);
		return 0;
	} else if (data_p->mouse_x >= SCREEN_R) {
		data_p->cam_buf_x += (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p) / 2;
		data_p->cam_buf_y -= (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p) / 2;
		return 1;
	} else if (data_p->mouse_y <= SCREEN_U) {
		data_p->cam_buf_x -= (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p);
		data_p->cam_buf_y -= (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p);
		return 1;
	} else if (data_p->mouse_x <= SCREEN_L) {
		data_p->cam_buf_x -= (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p) / 2;
		data_p->cam_buf_y += (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p) / 2;
		return 1;
	} else if (data_p->mouse_y >= SCREEN_D) {
		data_p->cam_buf_x += (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p);
		data_p->cam_buf_y += (data_p->pres_t - data_p->old_t) * speed(settings_p, data_p);
		return 1;
	}

	return 1;
}

void map_move(Settings* settings_p, Data* data_p, int x, int y) {
	view_adj(data_p, &x, &y);
	data_p->map_cur_x += x;
	data_p->map_cur_y += y;
}

int sign(int val) {
	return (val > 0) - (val < 0);
}

void cam_pan(Settings* settings_p, Data* data_p) {
	printf("speed: %f\n", speed(settings_p, data_p));
	int ortho = 0;

	if (data_p->cam_drag) {
		int diff_iso_x = (data_p->mouse_x - data_p->mouse_prev_x) * BUF_SZ / 2;
		int diff_iso_y = (data_p->mouse_prev_y - data_p->mouse_y) * BUF_SZ;
		data_p->cam_buf_x += diff_iso_y - diff_iso_x;
		data_p->cam_buf_y += diff_iso_x + diff_iso_y;
	} else
		ortho = pan(settings_p, data_p);


	if (ortho) {
		if (legal(settings_p, data_p, data_p->cam_buf_x / BUF_SZ, data_p->cam_buf_y / BUF_SZ)) {
			if (abs(data_p->cam_buf_x) >= BUF_SZ && abs(data_p->cam_buf_y) >= BUF_SZ) {
				int min = abs(data_p->cam_buf_x) < abs(data_p->cam_buf_y) ? abs(data_p->cam_buf_x) : abs(data_p->cam_buf_y);
				data_p->cam_iso_x += min / BUF_SZ * sign(data_p->cam_buf_x);
				data_p->cam_buf_x -= min / BUF_SZ * sign(data_p->cam_buf_x) * BUF_SZ;
				data_p->cam_iso_y += min / BUF_SZ * sign(data_p->cam_buf_y);
				data_p->cam_buf_y -= min / BUF_SZ * sign(data_p->cam_buf_y) * BUF_SZ;
			}
		} else {
			ortho = 0;
		}
	} 

	if (!ortho) {
		if (abs(data_p->cam_buf_x) >= BUF_SZ && legal(settings_p, data_p, data_p->cam_buf_x / BUF_SZ, 0)) {
			data_p->cam_iso_x += data_p->cam_buf_x / BUF_SZ;
			data_p->cam_buf_x -= data_p->cam_buf_x / BUF_SZ * BUF_SZ;
		}

		if (abs(data_p->cam_buf_y) >= BUF_SZ && legal(settings_p, data_p, 0, data_p->cam_buf_y / BUF_SZ)) {
			data_p->cam_iso_y += data_p->cam_buf_y / BUF_SZ;
			data_p->cam_buf_y -= data_p->cam_buf_y / BUF_SZ * BUF_SZ;
		}

		if (data_p->cam_buf_x >= BUF_SZ || data_p->cam_buf_x <= -BUF_SZ)
				data_p->cam_buf_x -= data_p->cam_buf_x / BUF_SZ * BUF_SZ;

		if (data_p->cam_buf_y >= BUF_SZ || data_p->cam_buf_y <= -BUF_SZ)
				data_p->cam_buf_y -= data_p->cam_buf_y / BUF_SZ * BUF_SZ;
	}

	// moves the map cursor on the x and y axes
	// if the camera has panned a whole tile's width or height,
	// reset the x or y offset and change the map cursor
	while (abs(data_p->cam_iso_x) >= ZOOM_SCALE(TILE_H)) {
		map_move(settings_p, data_p, sign(data_p->cam_iso_x), 0);
		data_p->cam_iso_x -= ZOOM_SCALE(TILE_H) * sign(data_p->cam_iso_x);
	}

	while (abs(data_p->cam_iso_y) >= ZOOM_SCALE(TILE_H)) {
		map_move(settings_p, data_p, 0, sign(data_p->cam_iso_y));
		data_p->cam_iso_y -= ZOOM_SCALE(TILE_H) * sign(data_p->cam_iso_y);
	}
}
