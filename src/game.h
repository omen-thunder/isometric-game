#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TILE_W (128)
#define TILE_H (64)
#define NUM_TEX (2)

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
	SDL_Texture* textures[NUM_TEX];	// array of textures
	int x_off;	// x-axis offset of the background rhombus
	int y_off;	// y-axis offset of the background rhombus
	int x_off2;	// used for testing
	int y_off2;	// used for testing
	int x_cur;	// the x-axis cursor for the current camera location on the map
	int y_cur;	// the x-axis cursor for the current camera location on the map
} map_data;

// contains camera related variables
typedef struct {
	int frame;	// frame counter
	int dir;	// last edge-pan direction the mouse was in
	int rate;	// scroll rate when edge-panning
} cam_data;

// in setup.c
void closeSDL(SDL_Window* win, SDL_Renderer* rend, map_data* map_d);
int make_window(SDL_Window** win, win_data* win_d);
int make_renderer(SDL_Window* win, SDL_Renderer** rend);
int load_texture(SDL_Renderer* rend, SDL_Texture** tex, char* path);

// in map.c
int map_init(win_data* win_d, map_data* map_d);
int get_row(map_data* map_d, float mouse_x, float mouse_y);
int get_column(map_data* map_d, float mouse_x, float mouse_y);
void move_l(map_data* map_d);
void move_u(map_data* map_d);
void move_r(win_data* win_d, map_data* map_d);
void move_d(win_data* win_d, map_data* map_d);
