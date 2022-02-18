#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
#include <math.h>
#include <stdint.h>

#define ZOOM_SCALE(x) (x << data_p->zoom >> 2)
#define TILE_W (settings_p->tile_h * 2)
#define TILE_H (settings_p->tile_h)
#define NUM_TILES (0)
#define NUM_WATER (256)
#define NUM_OBJS (2)
#define NUM_MENU (0)
#define NUM_SELECTOR (18)
#define NUM_WALL (256)
#define NUM_GRASS (256)
#define NUM_PLEB (64)
#define NUM_SPRITES (9)
#define GAP (4)
#define BUF_SZ (60)
#define OFF_X (settings_p->win_w / 2 - ZOOM_SCALE(TILE_W) / 2)
#define OFF_Y (-settings_p->win_w / 4)
#define DIST(x1, y1, x2, y2) (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)))
// calculate the size of each side of the background rhombus by
// finding its side length based on the window height and width
// and dividing it by the side length of one tile rhombus
#define WIN_SZ (ceil(DIST(settings_p->win_h * -1.0, settings_p->win_h / 2.0, settings_p->win_w / 2.0, settings_p->win_w / -4.0) / DIST(0.0, ZOOM_SCALE(TILE_W) / 2.0, ZOOM_SCALE(TILE_H) / 2.0f, 0.0)))
#define OUT_OF_BOUNDS(x, y) (x <= settings_p->border_sz || y <= settings_p->border_sz || x >= settings_p->map_sz - settings_p->border_sz || y >= settings_p->map_sz - settings_p->border_sz)

enum selector_tex_enum {
	T_SELECTOR_W, T_SELECTOR_W_R, T_SELECTOR_W_UR, T_SELECTOR_W_U, T_SELECTOR_W_UL,
	T_SELECTOR_W_L, T_SELECTOR_W_DL, T_SELECTOR_W_D, T_SELECTOR_W_DR,
	T_SELECTOR_R, T_SELECTOR_R_R, T_SELECTOR_R_UR, T_SELECTOR_R_U, T_SELECTOR_R_UL,
	T_SELECTOR_R_L, T_SELECTOR_R_DL, T_SELECTOR_R_D, T_SELECTOR_R_DR
};

// enumerations for sprites
enum type_enum {GRASS, WATER, EMPTY, OCCUPIED, TREE, WALL, BASE};
enum tab_id_enum {L_EMPTY = -1, L_GRASS, L_WATER, L_TREE_0, L_TREE_1, L_TREE_2, L_TREE_3, L_TREE_4, L_WALL, L_BASE};
enum obj_tex_enum {T_EMPTY = -1, T_TREE, T_BASE};

// enumerations for the menu modes
enum mode_enum {U_DEFAULT, U_WATER, U_TREE, U_BASE, U_WALL, U_PLEB};

/*
typedef struct npc_struct {
	struct npc_struct* next;	// pointer to the next npc in the list
	int x;				// the x-coordinate of the npc
	int y;				// the y-coordinate of the npc
} Npc;
*/

typedef struct {
	int win_w;
	int win_h;
	int fullscreen;
	int borderless;
	int grab;
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
	// mouse variables
	int mouse_x;
	int mouse_y;
	int mouse_col;
	int mouse_row;
	int mouse_adj_col;
	int mouse_adj_row;
	int mouse_button;

	// camera variables
	Uint32 old_t;
	Uint32 pres_t;
	int iso_x;
	int iso_y;
	int buf;
	int view;
	int zoom;
	int prev_dir;

	// map variables
	int cur_x;
	int cur_y;
	int mode;

	int win_sz;

	// lookup tables
	SDL_Texture** tab_tex[NUM_SPRITES];
	int tab_rect_w[NUM_SPRITES];
	int tab_rect_h[NUM_SPRITES];
	int tab_rect_x[NUM_SPRITES];
	int tab_rect_y[NUM_SPRITES];
} Data;

typedef struct {
	int type;
	int tab_id;
	unsigned tex_index;
} Sprite;

typedef struct {
	Sprite** tiles;	// 2D array representing the background tiles
	Sprite** objs;	// 2D array representing the objects on the map
	//npc* npc_head;		// the linked-list head for npcs
} Maps;

// in init.c
int init(SDL_Window* win, SDL_Renderer* rend, Settings* settings_p, Textures* textures_p, Maps* maps_p, Data* data_p);
int deinit(SDL_Window* win, SDL_Renderer* rend, Settings* settings_p, Textures* textures_p, Maps* maps_p, Data* data_p);

// in camera.c
void cam_pan(Settings* settings_p, Maps* maps_p, Data* data_p);

// in animate.c
int animate(SDL_Window* win, SDL_Renderer* rend, Settings* settings_p, Textures* textures_p, Maps* maps_p, Data* data_p);

// in event.c
void mouse(Settings* settings_p, Maps* maps_p, Data* data_p);
int event(Settings* settings_p, Data* data_p);

// in npc.c

#endif
