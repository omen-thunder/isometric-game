#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define TILE_W (128)
#define TILE_H (64)
#define NUM_TILES (1)
#define NUM_WATER (47)
#define NUM_OBJS (2)
#define NUM_MENU (0)
#define NUM_SELECTOR (18)
#define BUF_SZ (60)

// enumerations for the texture arrays
enum tile_tex_enum {T_GRASS};
enum obj_tex_enum {T_EMPTY = -1, T_TREE, T_BASE};
enum selector_tex_enum {
	T_SELECTOR_W, T_SELECTOR_W_R, T_SELECTOR_W_UR, T_SELECTOR_W_U, T_SELECTOR_W_UL,
	T_SELECTOR_W_L, T_SELECTOR_W_DL, T_SELECTOR_W_D, T_SELECTOR_W_DR,
	T_SELECTOR_R, T_SELECTOR_R_R, T_SELECTOR_R_UR, T_SELECTOR_R_U, T_SELECTOR_R_UL,
	T_SELECTOR_R_L, T_SELECTOR_R_DL, T_SELECTOR_R_D, T_SELECTOR_R_DR
};

// enumerations for the map arrays
enum tile_map_enum {GRASS, WATER};
enum obj_map_enum {EMPTY, TREE, BASE, OCCUPIED};

// enumerations for the menu modes
enum mode_enum {U_DEFAULT, U_WATER, U_TREE, U_BASE};

// contains window related variables
typedef struct {
	int win_w;	// the window width
	int win_h;	// the window height
	int mouse_x;	// the mouse's x position
	int mouse_y;	// the mouse's x position
	int fps;	// the number of frames displayed per second
	Uint32 old_t;	// the number of milliseconds from SDL library initialization to the last frame
	Uint32 pres_t;	// the number of milliseconds from SDL library initialization to the current frame
} win_data;

// containts map related variables
typedef struct {
	int** tiles;	// 2D array representing the background tiles
	int** objs;	// 2D array representing the objects on the map
	int** npcs;	// 2D array representing the npcs on the map
	int win_sz;	// the number of tiles in one edge of the background rhombus
	int map_sz;	// the number of tiles in one edge of the map
	int off_x;	// x-axis offset of the background rhombus
	int off_y;	// y-axis offset of the background rhombus
	int cur_x;	// the x-axis cursor for the current camera location on the map
	int cur_y;	// the x-axis cursor for the current camera location on the map
	int boarder;	// the size of the boarder
} map_data;

// contains textures
typedef struct {
	SDL_Texture* tile_tex[NUM_TILES];		// array of tile textures
	SDL_Texture* water_tex[NUM_WATER];		// array of tile textures
	SDL_Texture* obj_tex[NUM_OBJS];			// array of object textures
	SDL_Texture* menu_tex[NUM_MENU];		// array of menu textures
	SDL_Texture* selector_tex[NUM_SELECTOR];	// array of selector textures
} tex_data;

// contains camera related variables
typedef struct {
	int rate;		// the base scroll rate when edge-panning
	int accel;		// the acceleration of edge-panning
	int sensitivity;	// the distance to the edge of the screen when edge-panning starts
	int iso_x;		// isometric x-axis offset for rendering when the camera is moving
	int iso_y;		// isometric y-axis offset for rendering when the camera is moving
	int buf;		// buffer for the isometric offset
	int prev_dir;		// the previous pan direction
} cam_data;

// contains menu related variables
typedef struct {
	int mode;	// the current mode the menu is in

} menu_data;

// in map.c
int map_init(win_data* win_d, map_data* map_d);
int out_of_bounds(map_data* map_d, int x, int y);
int editable(map_data* map_d, menu_data* menu_d, int x, int y);
int get_row(map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y);
int get_column(map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y);
void move_ur(map_data* map_d);
void move_ul(map_data* map_d);
void move_dl(map_data* map_d);
void move_dr(map_data* map_d);
void map_free(map_data* map_d);

// in camera.c
void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d);

// in water.c
int water_index(map_data* map_d, int x, int y);

// in texture.c
int texture_init(SDL_Renderer* rend, tex_data* tex_d);
void texture_free(tex_data* tex_d);

// in animate.c
int animate(SDL_Window* win, SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, menu_data* menu_d);

// in event.c
int event(win_data* win_d, map_data* map_d, cam_data* cam_d, menu_data* menu_d);

#endif
