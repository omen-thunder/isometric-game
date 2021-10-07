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

// enumerations for the texture arrays
enum tile_tex {GRASS = 47};
enum obj_tex {NONE, TREE};
enum menu_tex {SELECTOR};

// contains window related variables
typedef struct {
	int win_w;	// the window width
	int win_h;	// the window height
	int mouse_x;	// the mouse's x position
	int mouse_y;	// the mouse's y position
	int fps;	// the number of frames displayed per second
	Uint32 old_t;	// the number of milliseconds from SDL library initialization to the last frame
	Uint32 pres_t;	// the number of milliseconds from SDL library initialization to the current frame
} win_data;

// containts map related variables
typedef struct {
	int** tiles;	// 2D array representing the background tiles
	int** objs;	// 2D array representing the objects on the map
	int win_sz;	// the number of tiles in one edge of the background rhombus
	int map_sz;	// the number of tiles in one edge of the map
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
	SDL_Texture* menu_tex[NUM_MENU];	// arrary of menu textures
} tex_data;

// contains camera related variables
typedef struct {
	float rate;	// scroll rate when edge-panning
	int iso_x;	// isometric x-axis offset for rendering when the camera is moving
	int iso_y;	// isometric y-axis offset for rendering when the camera is moving
	float buf;	// buffer for the isometric offset
	int prev_dir;	// the previous pan direction
} cam_data;

// in map.c
int map_init(win_data* win_d, map_data* map_d);
int get_row(map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y);
int get_column(map_data* map_d, cam_data* cam_d, int mouse_x, int mouse_y);
void move_ur(map_data* map_d);
void move_ul(map_data* map_d);
void move_dl(map_data* map_d);
void move_dr(map_data* map_d);
void free_map(map_data* map_d);

// in camera.c
void cam_pan(win_data* win_d, map_data* map_d, cam_data* cam_d);

// in water.c
int water_count(map_data* map_d, int x, int y);
int water_sel_1(map_data* map_d, int x, int y);
int water_sel_2(map_data* map_d, int x, int y);
int water_sel_3(map_data* map_d, int x, int y);
int water_sel_4(map_data* map_d, int x, int y);
int water_sel_5(map_data* map_d, int x, int y);
int water_sel_6(map_data* map_d, int x, int y);
int water_sel_7(map_data* map_d, int x, int y);
int water_index(map_data* map_d, int x, int y);

// in texture.c
void texture_free(SDL_Texture** tex, size_t size);
int load_texture(SDL_Renderer* rend, SDL_Texture** tex, char* path);
int tile_init(SDL_Renderer* rend, tex_data* tex_d);
int obj_init(SDL_Renderer* rend, tex_data* tex_d);
int texture_init(SDL_Renderer* rend, tex_data* tex_d);
void free_tex(tex_data* tex_d);

// in animate.c
int screen_x(map_data* map_d, cam_data* cam_d, int x, int y);
int screen_y(map_data* map_d, cam_data* cam_d, int x, int y);
void draw_tile(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y);
void draw_obj(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d, int x, int y);
void draw_bg(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d);
void draw_fg(SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d);
int animate(SDL_Window* win, SDL_Renderer* rend, win_data* win_d, map_data* map_d, tex_data* tex_d, cam_data* cam_d);

// in event.c
int event(win_data* win_d, map_data* map_d, cam_data* cam_d);

#endif
