#include "game.h"

// returns the edge-pan direction based on the mouse position
int pan_dir(win_data* win_d) {
	if ((win_d->mouse_x > win_d->win_w * 9 / 10 && win_d->mouse_y < win_d->win_h / 5) 
			|| (win_d->mouse_x > win_d->win_w * 8 / 10 && win_d->mouse_y < win_d->win_h / 10))
		return 2;
	if ((win_d->mouse_x < win_d->win_w / 10 && win_d->mouse_y < win_d->win_h / 5) 
			|| (win_d->mouse_x < win_d->win_w / 5 && win_d->mouse_y < win_d->win_h / 10))
		return 4;
	if ((win_d->mouse_x < win_d->win_w / 10 && win_d->mouse_y > win_d->win_h * 8 / 10) 
			|| (win_d->mouse_x < win_d->win_w / 5 && win_d->mouse_y > win_d->win_h * 9 / 10))
		return 6;
	if ((win_d->mouse_x > win_d->win_w * 9 / 10 && win_d->mouse_y > win_d->win_h * 8 / 10) 
			|| (win_d->mouse_x > win_d->win_w * 8 / 10 && win_d->mouse_y > win_d->win_h * 9 / 10))
		return 8;
	if (win_d->mouse_x > win_d->win_w * 9 / 10)
		return 1;
	if (win_d->mouse_y < win_d->win_h / 10)
		return 3;
	if (win_d->mouse_x < win_d->win_w / 10)
		return 5;
	if (win_d->mouse_y > win_d->win_h * 9 / 10)
		return 7;
	return 0;
}

// pans the camera up and right
void pan_ur(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->y_cur > 4)
		cam_d->buf += (win_d->pres_t - win_d->old_t) * cam_d->rate;

	if (cam_d->buf >= 1) {
		cam_d->iso_y -= trunc(cam_d->buf);
		cam_d->buf -= trunc(cam_d->buf);
	}
}

// pans the camera up and left
void pan_ul(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur > 4)
		cam_d->buf += (win_d->pres_t - win_d->old_t) * cam_d->rate;

	if (cam_d->buf >= 1) {
		cam_d->iso_x -= trunc(cam_d->buf);
		cam_d->buf -= trunc(cam_d->buf);
	}
}

// pans the camera down and left
void pan_dl(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->y_cur < map_d->map_sz - map_d->win_sz - 4)
		cam_d->buf += (win_d->pres_t - win_d->old_t) * cam_d->rate;
	
	if (cam_d->buf >= 1) {
		cam_d->iso_y += trunc(cam_d->buf);
		cam_d->buf -= trunc(cam_d->buf);
	}
}

// pans the camera down and right
void pan_dr(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4)
		cam_d->buf += (win_d->pres_t - win_d->old_t) * cam_d->rate;

	if (cam_d->buf >= 1) {
		cam_d->iso_x += trunc(cam_d->buf);
		cam_d->buf -= trunc(cam_d->buf);
	}
}

// pans the camera right
void pan_r(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4 && map_d->y_cur > 4) {
		cam_d->buf += (win_d->pres_t - win_d->old_t) * cam_d->rate / 2;

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (cam_d->buf >= 1) {
			cam_d->iso_x += trunc(cam_d->buf);
			cam_d->iso_y -= trunc(cam_d->buf);
			cam_d->buf -= trunc(cam_d->buf);
		}

	} else {
		pan_ur(win_d, map_d, cam_d);
		pan_dr(win_d, map_d, cam_d);
	}
}

// pans the camera up
void pan_u(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4 && map_d->y_cur > 4) {
		cam_d->buf += (win_d->pres_t - win_d->old_t) * cam_d->rate;

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (cam_d->buf >= 1) {
			cam_d->iso_x -= trunc(cam_d->buf);
			cam_d->iso_y -= trunc(cam_d->buf);
			cam_d->buf -= trunc(cam_d->buf);
		}

	} else {
		pan_ur(win_d, map_d, cam_d);
		pan_ul(win_d, map_d, cam_d);
	}
}

// pans the camera left
void pan_l(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4 && map_d->y_cur > 4) {
		cam_d->buf += (win_d->pres_t - win_d->old_t) * cam_d->rate / 2;

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (cam_d->buf >= 1) {
			cam_d->iso_x -= trunc(cam_d->buf);
			cam_d->iso_y += trunc(cam_d->buf);
			cam_d->buf -= trunc(cam_d->buf);
		}

	} else {
		pan_ul(win_d, map_d, cam_d);
		pan_dl(win_d, map_d, cam_d);
	}
}

// pans the camera down
void pan_d(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4 && map_d->y_cur > 4) {
		cam_d->buf += (win_d->pres_t - win_d->old_t) * cam_d->rate;

		// allows camera to move in both isometric axes
		// at the same time to avoid jittering
		if (cam_d->buf >= 1) {
			cam_d->iso_x += trunc(cam_d->buf);
			cam_d->iso_y += trunc(cam_d->buf);
			cam_d->buf -= trunc(cam_d->buf);
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
		cam_d->iso_x = floor(cam_d->iso_x - TILE_H);
		move_dr(map_d);
	} else if (cam_d->iso_x <= -TILE_H) {
		cam_d->iso_x = floor(cam_d->iso_x + TILE_H);
		move_ul(map_d);
	}
}

// moves the map cursor on the y-axis
// if the camera has panned a whole tile's height,
// reset the y offset and change the map cursor
void map_shift_y(map_data* map_d, cam_data* cam_d) {
	if (cam_d->iso_y >= TILE_H) {
		cam_d->iso_y = floor(cam_d->iso_y - TILE_H);
		move_dl(map_d);
	} else if (cam_d->iso_y <= -TILE_H) {
		cam_d->iso_y = floor(cam_d->iso_y + TILE_H);
		move_ur(map_d);
	}
}

// resets the x and y buffers
void buf_clear(cam_data* cam_d, int dir) {
	if (cam_d->prev_dir != dir) {
		cam_d->buf = 0;
	}
}

// edge-pans the camera
void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d) {
	printf("x_off: %d\ty_off: %d\tbuf: %f\t\n", cam_d->iso_x, cam_d->iso_y, cam_d->buf);

	buf_clear(cam_d, pan_dir(win_d));

	// check if the mouse is on the edge of the screen and, if so, pan the camera
	switch (pan_dir(win_d)) {
		case 0:
			cam_d->prev_dir = 0;
			break;
		case 1:
			pan_r(win_d, map_d, cam_d);
			cam_d->prev_dir = 1;
			break;
		case 2:
			pan_ur(win_d, map_d, cam_d);
			cam_d->prev_dir = 2;
			break;
		case 3:
			pan_u(win_d, map_d, cam_d);
			cam_d->prev_dir = 3;
			break;
		case 4:
			pan_ul(win_d, map_d, cam_d);
			cam_d->prev_dir = 4;
			break;
		case 5:
			pan_l(win_d, map_d, cam_d);
			cam_d->prev_dir = 5;
			break;
		case 6:
			pan_dl(win_d, map_d, cam_d);
			cam_d->prev_dir = 6;
			break;
		case 7:
			pan_d(win_d, map_d, cam_d);
			cam_d->prev_dir = 7;
			break;
		case 8:
			pan_dr(win_d, map_d, cam_d);
			cam_d->prev_dir = 8;
			break;
	}

	map_shift_x(map_d, cam_d);
	map_shift_y(map_d, cam_d);
}
