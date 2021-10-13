#include "game.h"

#define SCREEN_R (win_d->win_w * 9 / 10)
#define SCREEN_U (win_d->win_h / 10)
#define SCREEN_L (win_d->win_w / 10)
#define SCREEN_D (win_d->win_h * 9 / 10)

enum directions {CENTRE, R, UR, U, UL, L, DL, D, DR};

// returns the edge-pan direction based on the mouse position
int direction(win_data* win_d) {
	if ((win_d->mouse_x >= SCREEN_R && win_d->mouse_y <= SCREEN_U * 2) 
			|| (win_d->mouse_x >= SCREEN_R - SCREEN_L && win_d->mouse_y <= SCREEN_U))
		return UR;
	if ((win_d->mouse_x <= SCREEN_L && win_d->mouse_y <= SCREEN_U * 2) 
			|| (win_d->mouse_x <= SCREEN_L * 2 && win_d->mouse_y <= SCREEN_U))
		return UL;
	if ((win_d->mouse_x <= SCREEN_L && win_d->mouse_y >= SCREEN_D - SCREEN_U) 
			|| (win_d->mouse_x <= SCREEN_L * 2 && win_d->mouse_y >= SCREEN_D))
		return DL;
	if ((win_d->mouse_x >= SCREEN_R && win_d->mouse_y >= SCREEN_D - SCREEN_U) 
			|| (win_d->mouse_x >= SCREEN_R - SCREEN_L && win_d->mouse_y >= SCREEN_D))
		return DR;
	if (win_d->mouse_x >= SCREEN_R)
		return R;
	if (win_d->mouse_y <= SCREEN_U)
		return U;
	if (win_d->mouse_x <= SCREEN_L)
		return L;
	if (win_d->mouse_y >= SCREEN_D)
		return D;
	return CENTRE;
}

float speed_r(win_data* win_d, cam_data* cam_d) {
	return cam_d->rate * ((float) (win_d->mouse_x - SCREEN_R) * cam_d->accel / (float) (win_d->win_w - SCREEN_R - 1) + 1) / 2;
}

float speed_ur(win_data* win_d, cam_data* cam_d) {
	float x_rate = cam_d->rate * ((float) (win_d->mouse_x - SCREEN_R) * cam_d->accel / (float) (win_d->win_w - SCREEN_R - 1) + 1);
	float y_rate = cam_d->rate * ((float) (SCREEN_U - win_d->mouse_y) * cam_d->accel / (float) SCREEN_U + 1);
	if (x_rate > y_rate)
		return x_rate;
	else
		return y_rate;
}

float speed_u(win_data* win_d, cam_data* cam_d) {
	return cam_d->rate * ((float) (SCREEN_U - win_d->mouse_y) * cam_d->accel / (float) SCREEN_U + 1);
}

float speed_ul(win_data* win_d, cam_data* cam_d) {
	float x_rate = cam_d->rate * ((float) (SCREEN_L - win_d->mouse_x) * cam_d->accel / (float) SCREEN_L + 1);
	float y_rate = cam_d->rate * ((float) (SCREEN_U - win_d->mouse_y) * cam_d->accel / (float) SCREEN_U + 1);
	if (x_rate > y_rate)
		return x_rate;
	else
		return y_rate;
}

float speed_l(win_data* win_d, cam_data* cam_d) {
	return cam_d->rate * ((float) (SCREEN_L - win_d->mouse_x) * cam_d->accel / (float) SCREEN_L + 1) / 2;
}

float speed_dl(win_data* win_d, cam_data* cam_d) {
	float x_rate = cam_d->rate * ((float) (SCREEN_L - win_d->mouse_x) * cam_d->accel / (float) SCREEN_L + 1);
	float y_rate = cam_d->rate * ((float) (win_d->mouse_y - SCREEN_D) * cam_d->accel / (float) (win_d->win_h - SCREEN_D - 1) + 1);
	if (x_rate > y_rate)
		return x_rate;
	else
		return y_rate;
}

float speed_d(win_data* win_d, cam_data* cam_d) {
	return cam_d->rate * ((float) (win_d->mouse_y - SCREEN_D) * cam_d->accel / (float) (win_d->win_h - SCREEN_D - 1) + 1);
}

float speed_dr(win_data* win_d, cam_data* cam_d) {
	float x_rate = cam_d->rate * ((float) (win_d->mouse_x - SCREEN_R) * cam_d->accel / (float) (win_d->win_w - SCREEN_R - 1) + 1);
	float y_rate = cam_d->rate * ((float) (win_d->mouse_y - SCREEN_D) * cam_d->accel / (float) (win_d->win_h - SCREEN_D - 1) + 1);
	if (x_rate > y_rate)
		return x_rate;
	else
		return y_rate;
}


// pans the camera up and right
void pan_ur(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->y_cur > 4)
		cam_d->buf += (win_d->pres_t - win_d->old_t) * speed_ur(win_d, cam_d);

	if (cam_d->buf >= BUF_SZ) {
		cam_d->iso_y -= cam_d->buf / BUF_SZ;
		cam_d->buf -= cam_d->buf /  BUF_SZ * BUF_SZ;
	}
}

// pans the camera up and left
void pan_ul(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur > 4)
		cam_d->buf += (win_d->pres_t - win_d->old_t) * speed_ul(win_d, cam_d);

	if (cam_d->buf >= BUF_SZ) {
		cam_d->iso_x -= cam_d->buf / BUF_SZ;
		cam_d->buf -= cam_d->buf /  BUF_SZ * BUF_SZ;
	}
}

// pans the camera down and left
void pan_dl(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->y_cur < map_d->map_sz - map_d->win_sz - 4)
		cam_d->buf += (win_d->pres_t - win_d->old_t) * speed_dl(win_d, cam_d);
	
	if (cam_d->buf >= BUF_SZ) {
		cam_d->iso_y += cam_d->buf / BUF_SZ;
		cam_d->buf -= cam_d->buf /  BUF_SZ * BUF_SZ;
	}
}

// pans the camera down and right
void pan_dr(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4)
		cam_d->buf += (win_d->pres_t - win_d->old_t) * speed_dr(win_d, cam_d);

	if (cam_d->buf >= BUF_SZ) {
		cam_d->iso_x += cam_d->buf / BUF_SZ;
		cam_d->buf -= cam_d->buf /  BUF_SZ * BUF_SZ;
	}
}

// pans the camera right
void pan_r(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4 && map_d->y_cur > 4) {
		cam_d->buf += (win_d->pres_t - win_d->old_t) * speed_r(win_d, cam_d);

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (cam_d->buf >= BUF_SZ) {
			cam_d->iso_x += cam_d->buf / BUF_SZ;
			cam_d->iso_y -= cam_d->buf / BUF_SZ;
			cam_d->buf -= cam_d->buf /  BUF_SZ * BUF_SZ;
		}

	} else {
		pan_ur(win_d, map_d, cam_d);
		pan_dr(win_d, map_d, cam_d);
	}
}

// pans the camera up
void pan_u(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur > 4 && map_d->y_cur > 4) {
		cam_d->buf += (win_d->pres_t - win_d->old_t) * speed_u(win_d, cam_d);

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (cam_d->buf >= BUF_SZ) {
			cam_d->iso_x -= cam_d->buf / BUF_SZ;
			cam_d->iso_y -= cam_d->buf / BUF_SZ;
			cam_d->buf -= cam_d->buf /  BUF_SZ * BUF_SZ;
		}

	} else {
		pan_ur(win_d, map_d, cam_d);
		pan_ul(win_d, map_d, cam_d);
	}
}

// pans the camera left
void pan_l(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur > 4 && map_d->y_cur < map_d->map_sz - map_d->win_sz - 4) {
		cam_d->buf += (win_d->pres_t - win_d->old_t) * speed_l(win_d, cam_d);

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (cam_d->buf >= BUF_SZ) {
			cam_d->iso_x -= cam_d->buf / BUF_SZ;
			cam_d->iso_y += cam_d->buf / BUF_SZ;
			cam_d->buf -= cam_d->buf /  BUF_SZ * BUF_SZ;
		}

	} else {
		pan_ul(win_d, map_d, cam_d);
		pan_dl(win_d, map_d, cam_d);
	}
}

// pans the camera down
void pan_d(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4 && map_d->y_cur < map_d->map_sz - map_d->win_sz - 4) {
		cam_d->buf += (win_d->pres_t - win_d->old_t) * speed_d(win_d, cam_d);

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (cam_d->buf >= BUF_SZ) {
			cam_d->iso_x += cam_d->buf / BUF_SZ;
			cam_d->iso_y += cam_d->buf / BUF_SZ;
			cam_d->buf = 0;
		}

	} else {
		pan_dl(win_d, map_d, cam_d);
		pan_dr(win_d, map_d, cam_d);
	}
}

// moves the map cursor on the x-axis
// if the camera has panned a whole tile's width,
// reset the x offset and change the map cursor
void map_shift_x(map_data* map_d, cam_data* cam_d) {
	if (cam_d->iso_x >= TILE_H) {
		cam_d->iso_x -= TILE_H;
		move_dr(map_d);
	} else if (cam_d->iso_x <= -TILE_H) {
		cam_d->iso_x += TILE_H;
		move_ul(map_d);
	}
}

// moves the map cursor on the y-axis
// if the camera has panned a whole tile's height,
// reset the y offset and change the map cursor
void map_shift_y(map_data* map_d, cam_data* cam_d) {
	if (cam_d->iso_y >= TILE_H) {
		cam_d->iso_y -= TILE_H;
		move_dl(map_d);
	} else if (cam_d->iso_y <= -TILE_H) {
		cam_d->iso_y += TILE_H;
		move_ur(map_d);
	}
}

// resets the iso buffer
void buf_clear(cam_data* cam_d, int dir) {
	if (cam_d->prev_dir != dir) {
		cam_d->buf = 0;
	}
}

// edge-pans the camera
void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d) {

	buf_clear(cam_d, direction(win_d));

	// check if the mouse is on the edge of the screen and, if so, pan the camera
	switch (direction(win_d)) {
		case CENTRE:
			break;
		case R:
			pan_r(win_d, map_d, cam_d);
			break;
		case UR:
			pan_ur(win_d, map_d, cam_d);
			break;
		case U:
			pan_u(win_d, map_d, cam_d);
			break;
		case UL:
			pan_ul(win_d, map_d, cam_d);
			break;
		case L:
			pan_l(win_d, map_d, cam_d);
			break;
		case DL:
			pan_dl(win_d, map_d, cam_d);
			break;
		case D:
			pan_d(win_d, map_d, cam_d);
			break;
		case DR:
			pan_dr(win_d, map_d, cam_d);
			break;
	}

	cam_d->prev_dir = direction(win_d);
	map_shift_x(map_d, cam_d);
	map_shift_y(map_d, cam_d);
}
