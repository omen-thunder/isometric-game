#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
#include <math.h>
#include <stdint.h>

#define ZOOM_SCALE(x) (x << data_p->cam_zoom >> 2)
#define TILE_W (settings_p->tile_h << 1)
#define TILE_H (settings_p->tile_h)
#define NUM_TILES (0)
#define NUM_WATER (256)
#define NUM_OBJS (2)
#define NUM_MENU (0)
#define NUM_SELECTOR (8)
#define NUM_WALL (256)
#define NUM_GRASS (256)
#define NUM_PLEB (64)
#define NUM_TABLE (7)
#define GAP (4)
#define BUF_SZ (60)
#define OFF_X (settings_p->win_w / 2 - ZOOM_SCALE(TILE_W) / 2)
#define OFF_Y (settings_p->win_h / 2 - WIN_SZ * ZOOM_SCALE(TILE_H) / 2)
#define DIST(x1, y1, x2, y2) (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)))
// calculate the size of each side of the background rhombus by
// finding its side length based on the window height and width
// and dividing it by the side length of one tile rhombus
#define WIN_SZ (ceil(DIST(settings_p->win_h * -1.0, settings_p->win_h / 2.0, settings_p->win_w / 2.0, settings_p->win_w / -4.0) / DIST(0.0, ZOOM_SCALE(TILE_W) / 2.0, ZOOM_SCALE(TILE_H) / 2.0, 0.0)))
#define OUT_OF_BOUNDS(x, y) (x <= settings_p->border_sz || y <= settings_p->border_sz || x >= settings_p->map_sz - settings_p->border_sz || y >= settings_p->map_sz - settings_p->border_sz)
#define RANDOM_X(x, y) (((17 * x +  23 * y + 84) ^ 1734859) % 41 - 20)
#define RANDOM_Y(x, y) (((19 * x + 27 * y + 55) ^ 8234594) % 41 - 20)

enum selector_tex_enum {
	T_SEL_W_UR, T_SEL_W_UL, T_SEL_W_DL, T_SEL_W_DR,
	T_SEL_R_UR, T_SEL_R_UL, T_SEL_R_DL, T_SEL_R_DR
};

// enumerations for sprites
enum type_enum {GRASS, WATER, EMPTY, OCCUPIED, TREE, WALL, BASE, MENU, NPC};
enum tab_id_enum {L_EMPTY = -1, L_GRASS, L_WATER, L_TREE, L_WALL, L_BASE, L_SELECTOR, L_PLEB};
enum obj_tex_enum {T_EMPTY = -1, T_TREE, T_BASE};

// enumerations for the menu modes
enum mode_enum {U_DEFAULT, U_WATER, U_TREE, U_BASE, U_WALL, U_PLEB};

typedef struct {
	int win_w;
	int win_h;
	int fullscreen;
	int borderless;
	int screen_grab;
	int vsync;
	int fps;
	int tile_h;
	int map_sz;
	int border_sz;
	int pan_rate;		// the base scroll rate when edge-panning
	int pan_accel;		// the acceleration of edge-panning
	int pan_sens;		// the distance to the edge of the screen when edge-panning starts
} Settings;

typedef struct {
	SDL_Texture* tile_tex[NUM_TILES];
	SDL_Texture* water_tex[NUM_WATER];
	SDL_Texture* obj_tex[NUM_OBJS];
	SDL_Texture* menu_tex[NUM_MENU];
	SDL_Texture* selector_tex[NUM_SELECTOR];
	SDL_Texture* wall_tex[NUM_WALL];
	SDL_Texture* grass_tex[NUM_GRASS];
	SDL_Texture* pleb_tex[NUM_PLEB];
} Textures;

typedef struct {
	int type;
	int tab_id;
	unsigned tex_index;
	int rand_x;
	int rand_y;
} Sprite;

typedef struct Npc_struct {
	int col;
	int row;
	Sprite sprite;
	struct Npc_struct* next;
} Npc;

typedef struct Data_struct {
	int win_sz;

	// mouse variables
	int mouse_x;
	int mouse_y;
	int mouse_prev_x;
	int mouse_prev_y;
	int mouse_col;
	int mouse_row;
	int mouse_adj_col;
	int mouse_adj_row;
	int mouse_button;

	// camera variables
	Uint32 old_t;
	Uint32 pres_t;
	int cam_iso_x;
	int cam_iso_y;
	int cam_buf_x;
	int cam_buf_y;
	int cam_view;
	int cam_zoom;
	int cam_drag;

	// map variables
	Sprite** map_tiles;	// 2D array representing the background tiles
	Sprite** map_objs;	// 2D array representing the objects on the map
	int map_cur_x;
	int map_cur_y;

	// lookup tables
	SDL_Texture** tab_tex[NUM_TABLE];
	int tab_rect_w[NUM_TABLE];
	int tab_rect_h[NUM_TABLE];
	int tab_rect_x[NUM_TABLE];
	int tab_rect_y[NUM_TABLE];

	int menu_selec_sz;
	int menu_mode;

	Npc* npc_head;

	void (*adj_arr[4]) (struct Data_struct* data_p, int* col, int* row);
	void (*unadj_arr[4]) (struct Data_struct* data_p, int* col, int* row);
} Data;

// in camera.c
void cam_pan(Settings* settings_p, Data* data_p);

// in animate.c
int animate(SDL_Window* win, SDL_Renderer* rend, Settings* settings_p, Textures* textures_p, Data* data_p);

// in event.c
void mouse(Settings* settings_p, Data* data_p);
int event(Settings* settings_p, Data* data_p);
int editable(Settings* settings_p, Data* data_p, int x, int y);

// in npc.c
int push_npc(Npc** npc_head, int col, int row);
void pop_npc(Npc** npc_head);
void print_npcs(Npc* npc_head);

#endif
