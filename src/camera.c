#include "game.h"

// returns the edge-pan direction based on the mouse position
int pan_dir(win_data* win_d, int mouse_x, int mouse_y) {
	if ((mouse_x > win_d->win_w * 9 / 10 && mouse_y < win_d->win_h / 5) 
			|| (mouse_x > win_d->win_w * 8 / 10 && mouse_y < win_d->win_h / 10))
		return 2;
	if ((mouse_x < win_d->win_w / 10 && mouse_y < win_d->win_h / 5) 
			|| (mouse_x < win_d->win_w / 5 && mouse_y < win_d->win_h / 10))
		return 4;
	if ((mouse_x < win_d->win_w / 10 && mouse_y > win_d->win_h * 8 / 10) 
			|| (mouse_x < win_d->win_w / 5 && mouse_y > win_d->win_h * 9 / 10))
		return 6;
	if ((mouse_x > win_d->win_w * 9 / 10 && mouse_y > win_d->win_h * 8 / 10) 
			|| (mouse_x > win_d->win_w * 8 / 10 && mouse_y > win_d->win_h * 9 / 10))
		return 8;
	if (mouse_x > win_d->win_w * 9 / 10)
		return 1;
	if (mouse_y < win_d->win_h / 10)
		return 3;
	if (mouse_x < win_d->win_w / 10)
		return 5;
	if (mouse_y > win_d->win_h * 9 / 10)
		return 7;
	return 0;
}

// edge-pans the camera
void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y) {
	switch (pan_dir(win_d, mouse_x, mouse_y)) {
		case 0:
			break;
		case 1:
			if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4 && map_d->y_cur > 4) {
				cam_d->iso_x += cam_d->rate / 2;
				cam_d->iso_y -= cam_d->rate / 2;
			} else if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4) {
				cam_d->iso_x += cam_d->rate;
			} else if (map_d->y_cur > 4) {
				cam_d->iso_y -= cam_d->rate;
			}
			break;
		case 2:
			if (map_d->y_cur > 4)
				cam_d->iso_y -= cam_d->rate;
			break;
		case 3:
			if (map_d->x_cur > 4)
				cam_d->iso_x -= cam_d->rate;
			if (map_d->y_cur > 4)
				cam_d->iso_y -= cam_d->rate;
			break;
		case 4:
			if (map_d->x_cur > 4)
				cam_d->iso_x -= cam_d->rate;
			break;
		case 5:
			if (map_d->x_cur > 4 && map_d->y_cur < map_d->map_sz - map_d->win_sz - 4) {
				cam_d->iso_x -= cam_d->rate / 2;
				cam_d->iso_y += cam_d->rate / 2;
			} else if (map_d->x_cur > 4) {
				cam_d->iso_x -= cam_d->rate;
			} else if (map_d->y_cur < map_d->map_sz - map_d->win_sz - 4) {
				cam_d->iso_y += cam_d->rate;
			}
			break;
		case 6:
			if (map_d->y_cur < map_d->map_sz - map_d->win_sz - 4)
				cam_d->iso_y += cam_d->rate;
			break;
		case 7:
			if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4)
				cam_d->iso_x += cam_d->rate;
			if (map_d->y_cur < map_d->map_sz - map_d->win_sz - 4)
				cam_d->iso_y += cam_d->rate;
			break;
		case 8:
			if (map_d->x_cur < map_d->map_sz - map_d->win_sz - 4)
				cam_d->iso_x += cam_d->rate;
			break;
	}

	if (cam_d->iso_x >= TILE_H) {
		cam_d->iso_x -= TILE_H;
		move_dr(map_d);
	} else if (cam_d->iso_x <= -TILE_H) {
		cam_d->iso_x += TILE_H;
		move_ul(map_d);
	}

	if (cam_d->iso_y >= TILE_H) {
		cam_d->iso_y -= TILE_H;
		move_dl(map_d);
	} else if (cam_d->iso_y <= -TILE_H) {
		cam_d->iso_y += TILE_H;
		move_ur(map_d);
	}
}
