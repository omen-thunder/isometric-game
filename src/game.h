#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TILE_W (128)
#define TILE_H (64)
#define NUM_TILES (48)
#define NUM_OBJS (2)

// contains window related variables
typedef struct {
	int win_w;		// the window width
	int win_h;		// the window height
} win_data;

// containts map related variables
typedef struct {
	int** tiles;	// 2D array representing the background tiles
	int** objs;	// 2D array representing the objects on the map
	unsigned win_sz;	// the number of tiles in one edge of the background rhombus
	unsigned map_sz;	// the number of tiles in one edge of the map
	int x_off;	// x-axis offset of the background rhombus
	int y_off;	// y-axis offset of the background rhombus
	int x_off2;	// used for testing
	int y_off2;	// used for testing
	int x_cur;	// the x-axis cursor for the current camera location on the map
	int y_cur;	// the x-axis cursor for the current camera location on the map
} map_data;

// contains textures
typedef struct {
	SDL_Texture* tile_tex[NUM_TILES];	// array of tile textures
	SDL_Texture* obj_tex[NUM_OBJS];		// array of object textures
} tex_data;

// contains camera related variables
typedef struct {
	int frame;	// frame counter
	int rate;	// scroll rate when edge-panning
	int x_dir;	// last x-axis edge-pan direction the mouse was in
	int y_dir;	// last x-axis edge-pan direction the mouse was in
} cam_data;

// in main.c
int iso_x(int x, int y);
int iso_y(int x, int y);

// in setup.c
void closeSDL(SDL_Window* win, SDL_Renderer* rend, map_data* map_d, tex_data* tex_d);
int make_window(SDL_Window** win, win_data* win_d);
int make_renderer(SDL_Window* win, SDL_Renderer** rend);
int load_texture(SDL_Renderer* rend, SDL_Texture** tex, char* path);

// in map.c
int map_init(win_data* win_d, map_data* map_d);
int get_row(map_data* map_d, cam_data* cam_d, float mouse_x, float mouse_y);
int get_column(map_data* map_d, cam_data* cam_d, float mouse_x, float mouse_y);

// in camera.c
void move_r(map_data* map_d, cam_data* cam_d);
void move_ur(map_data* map_d, cam_data* cam_d);
void move_u(map_data* map_d, cam_data* cam_d);
void move_ul(map_data* map_d, cam_data* cam_d);
void move_l(map_data* map_d, cam_data* cam_d);
void move_dl(map_data* map_d, cam_data* cam_d);
void move_d(map_data* map_d, cam_data* cam_d);
void move_dr(map_data* map_d, cam_data* cam_d);
int pan_dir(win_data* win_d, int mouse_x, int mouse_y);
void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y);

// in water.c
int water_count(map_data* map_d, int x, int y);
size_t water_sel_1(map_data* map_d, int x, int y);
size_t water_sel_2(map_data* map_d, int x, int y);
size_t water_sel_3(map_data* map_d, int x, int y);
size_t water_sel_4(map_data* map_d, int x, int y);
size_t water_sel_5(map_data* map_d, int x, int y);
size_t water_sel_6(map_data* map_d, int x, int y);
size_t water_sel_7(map_data* map_d, int x, int y);
size_t water_index(map_data* map_d, int x, int y);

#endif
