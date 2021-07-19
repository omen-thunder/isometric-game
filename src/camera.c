#include "game.h"

// moves the camera to the right one tile
void move_r(map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz && map_d->y_cur > 0) {
		cam_d->x_dir = 1;
		cam_d->y_dir = -1;
		cam_d->rate /= 2;
		cam_d->frame++;
	} else {
		move_ur(map_d, cam_d);
		move_dr(map_d, cam_d);
	}
}

// moves the camera up and to the right one tile
void move_ur(map_data* map_d, cam_data* cam_d) {
	if (map_d->y_cur > 0) {
		cam_d->x_dir = 0;
		cam_d->y_dir = -1;
		cam_d->frame++;
	}
}

// moves the camera up one tile
void move_u(map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur > 0 && map_d->y_cur > 0) {
		cam_d->x_dir = -1;
		cam_d->y_dir = -1;
		cam_d->frame++;
	} else {
		move_ur(map_d, cam_d);
		move_ul(map_d, cam_d);
	}
}

// moves the camera up and to the left one tile
void move_ul(map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur > 0) {
		cam_d->x_dir = -1;
		cam_d->y_dir = 0;
		cam_d->frame++;
	}
}

// moves the camera to the left one tile
void move_l(map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur > 0 && map_d->y_cur < map_d->map_sz - map_d->win_sz) {
		cam_d->x_dir = -1;
		cam_d->y_dir = 1;
		cam_d->rate /= 2;
		cam_d->frame++;
	} else {
		move_ul(map_d, cam_d);
		move_dl(map_d, cam_d);
	}
}

// moves the camera down and to the left one tile each
void move_dl(map_data* map_d, cam_data* cam_d) {
	if (map_d->y_cur < map_d->map_sz - map_d->win_sz) {
		cam_d->x_dir = 0;
		cam_d->y_dir = 1;
		cam_d->frame++;
	}
}

// moves the camera down one tile
void move_d(map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz && map_d->y_cur < map_d->map_sz - map_d->win_sz) {
		cam_d->x_dir = 1;
		cam_d->y_dir = 1;
		cam_d->frame++;
	} else {
		move_dl(map_d, cam_d);
		move_dr(map_d, cam_d);
	}
}

// moves the camera down and to the right one tile each
void move_dr(map_data* map_d, cam_data* cam_d) {
	if (map_d->x_cur < map_d->map_sz - map_d->win_sz) {
		cam_d->x_dir = 1;
		cam_d->y_dir = 0;
		cam_d->frame++;
	}
}

// returns the edge-pan direction based on the mouse position
int pan_dir(win_data* win_d, int mouse_x, int mouse_y) {
	if ((mouse_x > win_d->win_w * 9 / 10 && mouse_y < win_d->win_h / 5) 
			|| (mouse_x > win_d->win_w * 8 / 10 && mouse_y < win_d->win_h / 10))
		return 2;
	else if ((mouse_x < win_d->win_w / 10 && mouse_y < win_d->win_h / 5) 
			|| (mouse_x < win_d->win_w / 5 && mouse_y < win_d->win_h / 10))
		return 4;
	else if ((mouse_x < win_d->win_w / 10 && mouse_y > win_d->win_h * 8 / 10) 
			|| (mouse_x < win_d->win_w / 5 && mouse_y > win_d->win_h * 9 / 10))
		return 6;
	else if ((mouse_x > win_d->win_w * 9 / 10 && mouse_y > win_d->win_h * 8 / 10) 
			|| (mouse_x > win_d->win_w * 8 / 10 && mouse_y > win_d->win_h * 9 / 10))
		return 8;
	else if (mouse_x > win_d->win_w * 9 / 10)
		return 1;
	else if (mouse_y < win_d->win_h / 10)
		return 3;
	else if (mouse_x < win_d->win_w / 10)
		return 5;
	else if (mouse_y > win_d->win_h * 9 / 10)
		return 7;
	return 0;
}

// edge-pans the camera
void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y) {
	if (cam_d->frame >= TILE_H / cam_d->rate) {
		map_d->x_cur += cam_d->x_dir;
		map_d->y_cur += cam_d->y_dir;
		if ((cam_d->x_dir == 1 && cam_d->y_dir == -1) || (cam_d->x_dir == -1 && cam_d->y_dir == 1))
			cam_d->rate *= 2;
		cam_d->x_dir = 0;
		cam_d->y_dir = 0;
		cam_d->frame = 0;
	}

	if (cam_d->frame > 0) {
		cam_d->frame++;
		return;
	}

	switch (pan_dir(win_d, mouse_x, mouse_y)) {
		case 0:
			cam_d->x_dir = 0;
			cam_d->y_dir = 0;
			break;
		case 1:
			move_r(map_d, cam_d);
			break;
		case 2:
			move_ur(map_d, cam_d);
			break;
		case 3:
			move_u(map_d, cam_d);
			break;
		case 4:
			move_ul(map_d, cam_d);
			break;
		case 5:
			move_l(map_d, cam_d);
			break;
		case 6:
			move_dl(map_d, cam_d);
			break;
		case 7:
			move_d(map_d, cam_d);
			break;
		case 8:
			move_dr(map_d, cam_d);
			break;
	}
}
