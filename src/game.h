#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define TILE_W (128)
#define TILE_H (64)
#define NUM_TILES (48)
#define NUM_OBJS (2)
#define NUM_MENU (1)
#define BOARDER (10)
#define BUF_SZ (60)

// enumerations for the texture arrays
enum tile_tex {GRASS = 47};
enum obj_tex {EMPTY = -1, TREE, BASE};
enum menu_tex {SELECTOR};

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
} map_data;

// contains textures
typedef struct {
	SDL_Texture* tile_tex[NUM_TILES];	// array of tile textures
	SDL_Texture* obj_tex[NUM_OBJS];		// array of object textures
	SDL_Texture* menu_tex[NUM_MENU];	// arrary of menu textures
} tex_data;

// contains camera related variables
typedef struct {
	int rate;	// the base scroll rate when edge-panning
	int accel;	// the acceleration of edge-panning
	int iso_x;	// isometric x-axis offset for rendering when the camera is moving
	int iso_y;	// isometric y-axis offset for rendering when the camera is moving
	int buf;	// buffer for the isometric offset
	int prev_dir;	// the previous pan direction
} cam_data;

// contains menu related variables
typedef struct {

} menu_data;

// in map.c
int map_init(win_data* win_d, map_data* map_d);
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
int animate(SDL_Window* win, SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d);

// in event.c
int event(win_data* win_d, map_data* map_d, cam_data* cam_d);

#endif
