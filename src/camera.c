#include "game.h"

#define SCREEN_L (settings_p->win_w / settings_p->pan_sens)
#define SCREEN_R (settings_p->win_w - SCREEN_L)
#define SCREEN_U (settings_p->win_h / settings_p->pan_sens)
#define SCREEN_D (settings_p->win_h - SCREEN_U)

enum directions {CENTRE, R, UR, U, UL, L, DL, D, DR};

// returns the edge-pan direction based on the mouse position
int direction(Settings* settings_p, Data* data_p) {
	if ((data_p->mouse_x >= SCREEN_R && data_p->mouse_y <= SCREEN_U * 2)
			|| (data_p->mouse_x >= SCREEN_R - SCREEN_L && data_p->mouse_y <= SCREEN_U))
		return UR;
	if ((data_p->mouse_x <= SCREEN_L && data_p->mouse_y <= SCREEN_U * 2)
			|| (data_p->mouse_x <= SCREEN_L * 2 && data_p->mouse_y <= SCREEN_U))
		return UL;
	if ((data_p->mouse_x <= SCREEN_L && data_p->mouse_y >= SCREEN_D - SCREEN_U)
			|| (data_p->mouse_x <= SCREEN_L * 2 && data_p->mouse_y >= SCREEN_D))
		return DL;
	if ((data_p->mouse_x >= SCREEN_R && data_p->mouse_y >= SCREEN_D - SCREEN_U)
			|| (data_p->mouse_x >= SCREEN_R - SCREEN_L && data_p->mouse_y >= SCREEN_D))
		return DR;
	if (data_p->mouse_x >= SCREEN_R)
		return R;
	if (data_p->mouse_y <= SCREEN_U)
		return U;
	if (data_p->mouse_x <= SCREEN_L)
		return L;
	if (data_p->mouse_y >= SCREEN_D)
		return D;
	return CENTRE;
}

// returns the panning speed for the right direction
float speed_r(Settings* settings_p, Data* data_p) {
	return settings_p->pan_rate * ((float) (data_p->mouse_x - SCREEN_R) * settings_p->pan_accel / (float) (settings_p->win_w - SCREEN_R - 1) + 1) / 2;
}

// returns the panning speed for the up-right direction
float speed_ur(Settings* settings_p, Data* data_p) {
	float x_rate = settings_p->pan_rate * ((float) (data_p->mouse_x - SCREEN_R) * settings_p->pan_accel / (float) (settings_p->win_w - SCREEN_R - 1) + 1);
	float y_rate = settings_p->pan_rate * ((float) (SCREEN_U - data_p->mouse_y) * settings_p->pan_accel / (float) SCREEN_U + 1);
	if (x_rate > y_rate)
		return x_rate;
	else
		return y_rate;
}

// returns the panning speed for the up direction
float speed_u(Settings* settings_p, Data* data_p) {
	return settings_p->pan_rate * ((float) (SCREEN_U - data_p->mouse_y) * settings_p->pan_accel / (float) SCREEN_U + 1);
}

// returns the panning speed for the up-left direction
float speed_ul(Settings* settings_p, Data* data_p) {
	float x_rate = settings_p->pan_rate * ((float) (SCREEN_L - data_p->mouse_x) * settings_p->pan_accel / (float) SCREEN_L + 1);
	float y_rate = settings_p->pan_rate * ((float) (SCREEN_U - data_p->mouse_y) * settings_p->pan_accel / (float) SCREEN_U + 1);
	if (x_rate > y_rate)
		return x_rate;
	else
		return y_rate;
}

// returns the panning speed for the left direction
float speed_l(Settings* settings_p, Data* data_p) {
	return settings_p->pan_rate * ((float) (SCREEN_L - data_p->mouse_x) * settings_p->pan_accel / (float) SCREEN_L + 1) / 2;
}

// returns the panning speed for the down-left direction
float speed_dl(Settings* settings_p, Data* data_p) {
	float x_rate = settings_p->pan_rate * ((float) (SCREEN_L - data_p->mouse_x) * settings_p->pan_accel / (float) SCREEN_L + 1);
	float y_rate = settings_p->pan_rate * ((float) (data_p->mouse_y - SCREEN_D) * settings_p->pan_accel / (float) (settings_p->win_h - SCREEN_D - 1) + 1);
	if (x_rate > y_rate)
		return x_rate;
	else
		return y_rate;
}

// returns the panning speed for the down direction
float speed_d(Settings* settings_p, Data* data_p) {
	return settings_p->pan_rate * ((float) (data_p->mouse_y - SCREEN_D) * settings_p->pan_accel / (float) (settings_p->win_h - SCREEN_D - 1) + 1);
}

// returns the panning speed for the down-right direction
float speed_dr(Settings* settings_p, Data* data_p) {
	float x_rate = settings_p->pan_rate * ((float) (data_p->mouse_x - SCREEN_R) * settings_p->pan_accel / (float) (settings_p->win_w - SCREEN_R - 1) + 1);
	float y_rate = settings_p->pan_rate * ((float) (data_p->mouse_y - SCREEN_D) * settings_p->pan_accel / (float) (settings_p->win_h - SCREEN_D - 1) + 1);
	if (x_rate > y_rate)
		return x_rate;
	else
		return y_rate;
}

int pan_legal(Settings* settings_p, Data* data_p, int dir) {
	// UR when view == 0
	if (dir == 0 && data_p->cur_y > GAP)
		return 1;
	// UL when view == 0
	if (dir == 1 && data_p->cur_x > GAP)
		return 1;
	// DL when view == 0
	if (dir == 2 && data_p->cur_y < settings_p->map_sz - data_p->win_sz - GAP)
		return 1;
	// DR when view == 0
	if (dir == 3 && data_p->cur_x < settings_p->map_sz - data_p->win_sz - GAP)
		return 1;

	return 0;
}


// pans the camera up and right
void pan_ur(Settings* settings_p, Maps* maps_p, Data* data_p) {
	if (pan_legal(settings_p, data_p, (0 + data_p->view) % 4))
		data_p->buf += (data_p->pres_t - data_p->old_t) * speed_ur(settings_p, data_p);

	if (data_p->buf >= BUF_SZ) {
		data_p->iso_y -= data_p->buf / BUF_SZ;
		data_p->buf -= data_p->buf /  BUF_SZ * BUF_SZ;
	}
}

// pans the camera up and left
void pan_ul(Settings* settings_p, Maps* maps_p, Data* data_p) {
	if (pan_legal(settings_p, data_p, (1 + data_p->view) % 4))
		data_p->buf += (data_p->pres_t - data_p->old_t) * speed_ul(settings_p, data_p);

	if (data_p->buf >= BUF_SZ) {
		data_p->iso_x -= data_p->buf / BUF_SZ;
		data_p->buf -= data_p->buf /  BUF_SZ * BUF_SZ;
	}
}

// pans the camera down and left
void pan_dl(Settings* settings_p, Maps* maps_p, Data* data_p) {
	if (pan_legal(settings_p, data_p, (2 + data_p->view) % 4))
		data_p->buf += (data_p->pres_t - data_p->old_t) * speed_dl(settings_p, data_p);

	if (data_p->buf >= BUF_SZ) {
		data_p->iso_y += data_p->buf / BUF_SZ;
		data_p->buf -= data_p->buf /  BUF_SZ * BUF_SZ;
	}
}

// pans the camera down and right
void pan_dr(Settings* settings_p, Maps* maps_p, Data* data_p) {
	if (pan_legal(settings_p, data_p, (3 + data_p->view) % 4))
		data_p->buf += (data_p->pres_t - data_p->old_t) * speed_dr(settings_p, data_p);

	if (data_p->buf >= BUF_SZ) {
		data_p->iso_x += data_p->buf / BUF_SZ;
		data_p->buf -= data_p->buf /  BUF_SZ * BUF_SZ;
	}
}

// pans the camera right
void pan_r(Settings* settings_p, Maps* maps_p, Data* data_p) {
	if (pan_legal(settings_p, data_p, (0 + data_p->view) % 4) && pan_legal(settings_p, data_p, (3 + data_p->view) % 4)) {
		data_p->buf += (data_p->pres_t - data_p->old_t) * speed_r(settings_p, data_p);

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (data_p->buf >= BUF_SZ) {
			data_p->iso_x += data_p->buf / BUF_SZ;
			data_p->iso_y -= data_p->buf / BUF_SZ;
			data_p->buf -= data_p->buf /  BUF_SZ * BUF_SZ;
		}

	} else {
		pan_ur(settings_p, maps_p, data_p);
		pan_dr(settings_p, maps_p, data_p);
	}
}

// pans the camera up
void pan_u(Settings* settings_p, Maps* maps_p, Data* data_p) {
	if (pan_legal(settings_p, data_p, (0 + data_p->view) % 4) && pan_legal(settings_p, data_p, (1 + data_p->view) % 4)) {
		data_p->buf += (data_p->pres_t - data_p->old_t) * speed_u(settings_p, data_p);

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (data_p->buf >= BUF_SZ) {
			data_p->iso_x -= data_p->buf / BUF_SZ;
			data_p->iso_y -= data_p->buf / BUF_SZ;
			data_p->buf -= data_p->buf /  BUF_SZ * BUF_SZ;
		}

	} else {
		pan_ur(settings_p, maps_p, data_p);
		pan_ul(settings_p, maps_p, data_p);
	}
}

// pans the camera left
void pan_l(Settings* settings_p, Maps* maps_p, Data* data_p) {
	if (pan_legal(settings_p, data_p, (1 + data_p->view) % 4) && pan_legal(settings_p, data_p, (2 + data_p->view) % 4)) {
		data_p->buf += (data_p->pres_t - data_p->old_t) * speed_l(settings_p, data_p);

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (data_p->buf >= BUF_SZ) {
			data_p->iso_x -= data_p->buf / BUF_SZ;
			data_p->iso_y += data_p->buf / BUF_SZ;
			data_p->buf -= data_p->buf /  BUF_SZ * BUF_SZ;
		}

	} else {
		pan_ul(settings_p, maps_p, data_p);
		pan_dl(settings_p, maps_p, data_p);
	}
}

// pans the camera down
void pan_d(Settings* settings_p, Maps* maps_p, Data* data_p) {
	if (pan_legal(settings_p, data_p, (2 + data_p->view) % 4) && pan_legal(settings_p, data_p, (3 + data_p->view) % 4)) {
		data_p->buf += (data_p->pres_t - data_p->old_t) * speed_d(settings_p, data_p);

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (data_p->buf >= BUF_SZ) {
			data_p->iso_x += data_p->buf / BUF_SZ;
			data_p->iso_y += data_p->buf / BUF_SZ;
			data_p->buf = 0;
		}

	} else {
		pan_dl(settings_p, maps_p, data_p);
		pan_dr(settings_p, maps_p, data_p);
	}
}

void map_move(Settings* settings_p, Data* data_p, int dir) {
	// move the map cursor up and right
	if (dir ==  0 && data_p->cur_y > GAP)
		data_p->cur_y--;

	// move the map cursor up and left
	else if (dir == 1 && data_p->cur_x > GAP)
		data_p->cur_x--;

	// move the map cursor down and left
	else if (dir == 2 && data_p->cur_y < settings_p->map_sz - data_p->win_sz - GAP)
		data_p->cur_y++;

	// move the map cursor down and right
	else if (dir == 3 && data_p->cur_x < settings_p->map_sz - data_p->win_sz - GAP)
		data_p->cur_x++;
}

void cam_pan(Settings* settings_p, Maps* maps_p, Data* data_p) {
	int cur_dir = direction(settings_p, data_p);

	// resets the iso buffer
	if (data_p->prev_dir != cur_dir)
		data_p->buf = 0;

	// check if the mouse is on the edge of the screen and, if so, pan the camera
	switch (cur_dir) {
		case CENTRE:
			break;
		case R:
			pan_r(settings_p, maps_p, data_p);
			break;
		case UR:
			pan_ur(settings_p, maps_p, data_p);
			break;
		case U:
			pan_u(settings_p, maps_p, data_p);
			break;
		case UL:
			pan_ul(settings_p, maps_p, data_p);
			break;
		case L:
			pan_l(settings_p, maps_p, data_p);
			break;
		case DL:
			pan_dl(settings_p, maps_p, data_p);
			break;
		case D:
			pan_d(settings_p, maps_p, data_p);
			break;
		case DR:
			pan_dr(settings_p, maps_p, data_p);
			break;
	}

	// moves the map cursor on the x and y axes
	// if the camera has panned a whole tile's width or height,
	// reset the x or y offset and change the map cursor
	if (data_p->iso_x >= ZOOM_SCALE(TILE_H)) {
		data_p->iso_x -= ZOOM_SCALE(TILE_H);
		map_move(settings_p, data_p, (3 + data_p->view) % 4);
	} else if (data_p->iso_x <= -TILE_H) {
		data_p->iso_x += ZOOM_SCALE(TILE_H);
		map_move(settings_p, data_p, (1 + data_p->view) % 4);
	}

	if (data_p->iso_y >= ZOOM_SCALE(TILE_H)) {
		data_p->iso_y -= ZOOM_SCALE(TILE_H);
		map_move(settings_p, data_p, (2 + data_p->view) % 4);
	} else if (data_p->iso_y <= -ZOOM_SCALE(TILE_H)) {
		data_p->iso_y += ZOOM_SCALE(TILE_H);
		map_move(settings_p, data_p, (0 + data_p->view) % 4);
	}

	data_p->prev_dir = cur_dir;
}
