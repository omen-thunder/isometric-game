#include "game.h"
#include <math.h>

#define SPEED (200) // speed in pixels/second
#define MAX_NPC (200)

struct npc {
	SDL_Texture* texture;
	SDL_Rect rect;
	float x_pos;
	float y_pos;
	float x_vel;
	float y_vel;
};

int draw_bg(SDL_Window* win, SDL_Renderer* rend, struct mdata* map) {
	SDL_Rect rect;
	rect.w = TILE_W;
	rect.h = TILE_H;

	for (int x = 0; x < map->win_sz; x++) {
		for (int y = 0; y < map->win_sz; y++) {
			rect.x = x * TILE_W / 2 - y * TILE_W / 2 + map->x_off;
			rect.y = y * TILE_H / 2 + x * TILE_H / 2 + map->y_off;

			if (rect.x > -TILE_W && rect.x < WINDOW_W && rect.y > -TILE_H && rect.y < WINDOW_H) {
				rect.x += map->x_off2;
				rect.y += map->y_off2;
				switch (map->tiles[x + map->x_cur][y + map->y_cur]) {
					case 0:
						SDL_RenderCopy(rend, map->textures[0], NULL, &rect);
						break;
					case 1:
						SDL_RenderCopy(rend, map->textures[1], NULL, &rect);
						break;
				}
			}
		}
	}

	return 0;
}

int texture_init(SDL_Renderer* rend, struct mdata* map) {
	if (load_texture(rend, &map->textures[0], "../resources/tiles/grassA.png")) {
		fprintf(stderr, "Failed to load texture 0\n");
		return -1;
	}
	if (load_texture(rend, &map->textures[1], "../resources/tiles/water.png")) {
		fprintf(stderr, "Failed to load texture 1\n");
		return -1;
	}
	return 0;
}

void cam_pan(struct mdata* map, struct cdata* cam, int mouse_x, int mouse_y) {
	if (mouse_x > WINDOW_W * 9 / 10 && mouse_y < WINDOW_H / 10) {
		if (--cam->frame <= 0) {
			cam_u(map);
			cam->frame = cam->rate;
		}
	} else if (mouse_x < WINDOW_W / 10 && mouse_y < WINDOW_H / 10) {
		if (--cam->frame <= 0) {
			cam_l(map);
			cam->frame = cam->rate;
		}
	} else if (mouse_x < WINDOW_W / 10 && mouse_y > WINDOW_H * 9 / 10) {
		if (--cam->frame <= 0) {
			cam_d(map);
			cam->frame = cam->rate;
		}
	} else if (mouse_x > WINDOW_W * 9 / 10 && mouse_y > WINDOW_H * 9 / 10) {
		if (--cam->frame <= 0) {
			cam_r(map);
			cam->frame = cam->rate;
		}
	} else if (mouse_y < WINDOW_H / 10) {
		if (--cam->frame <= 0) {
			cam_u(map);
			cam_l(map);
			cam->frame = cam->rate;
		}
	} else if (mouse_y > WINDOW_H * 9 / 10) {
		if (--cam->frame <= 0) {
			cam_d(map);
			cam_r(map);
			cam->frame = cam->rate;
		}
	} if (mouse_x < WINDOW_W / 10) {
		if (--cam->frame <= 0) {
			cam_l(map);
			cam_d(map);
			cam->frame = cam->rate;
		}
	} else if (mouse_x > WINDOW_W * 9 / 10) {
		if (--cam->frame <= 0) {
			cam_r(map);
			cam_u(map);
			cam->frame = cam->rate;
		}
	}
	return;
}

// processes events
int event(struct mdata* map, struct cdata* cam) {
	int button, mouse_x, mouse_y;
	button = SDL_GetMouseState(&mouse_x, &mouse_y);
	
	if (button == SDL_BUTTON(SDL_BUTTON_LEFT))
		map->tiles[get_column(map, mouse_x, mouse_y)][get_row(map, mouse_x,  mouse_y)] = 1;
	else if (button == SDL_BUTTON(SDL_BUTTON_RIGHT))
		map->tiles[get_column(map, mouse_x, mouse_y)][get_row(map, mouse_x,  mouse_y)] = 0;

	cam_pan(map, cam, mouse_x, mouse_y);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				//printf("row: %d column: %d x_cur: %d y_cur: %d\n", get_row(map, mouse_x, mouse_y), get_column(map, mouse_x, mouse_y), map->x_cur, map->y_cur);
				printf("mouse_x: %d mouse_y: %d\n", mouse_x, mouse_y);
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						cam_u(map);
						break;
					case SDLK_DOWN:
						cam_d(map);
						break;
					case SDLK_LEFT:
						cam_l(map);
						break;
					case SDLK_RIGHT:
						cam_r(map);
						break;
					case SDLK_w:
						map->y_off2 += 5;
						break;
					case SDLK_s:
						map->y_off2 -= 5;
						break;
					case SDLK_a:
						map->x_off2 -= 5;
						break;
					case SDLK_d:
						map->x_off2 += 5;
						break;
				}
				break;
		}
	}

	return 0;
}

int animate(SDL_Window* win, SDL_Renderer* rend, struct mdata* map) {
	struct cdata camera = {0, 0, 15};

	while (!event(map, &camera)) {
		// clear the window
		SDL_RenderClear(rend);

		// render the background
		draw_bg(win, rend, map);

		// display the window
		SDL_RenderPresent(rend);

		// wait 1/60th of a second
		SDL_Delay(1000/60);
	}

	return 0;
}

int main(void) {
	// attempt to initialise graphics and timer system
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* win;
	if (make_window(&win)) {
		closeSDL(NULL, NULL, NULL);
		return 1;
	}
	
	SDL_Renderer* rend;
	if (make_renderer(win, &rend)) {
		closeSDL(win, NULL, NULL);
		return 1;
	}

	struct mdata world_map;
	if (map_init(win, rend, &world_map)) {
		closeSDL(win, rend, NULL);
		return 1;
	}

	if (texture_init(rend, &world_map)) {
		closeSDL(win, rend, &world_map);
		return 1;
	}

	animate(win, rend, &world_map);

	closeSDL(win, rend, &world_map);
	return 0;
}
