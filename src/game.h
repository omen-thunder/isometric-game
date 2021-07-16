#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_W (1280)
#define WINDOW_H (720)
#define TILE_W (128)
#define TILE_H (64)
#define NUM_TEX (2)

// containts map related variables
struct mdata {
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
};

// contains camera related variables
struct cdata {
	int frame;	// frame counter
	int dir;	// last edge-pan direction the mouse was in
	int rate;	// scroll rate when edge-panning
};

// in setup.c
void closeSDL(SDL_Window* win, SDL_Renderer* rend, struct mdata* map);
int make_window(SDL_Window** win);
int make_renderer(SDL_Window* win, SDL_Renderer** rend);
int load_texture(SDL_Renderer* rend, SDL_Texture** tex, char* path);

// in map.c
int map_init(SDL_Window* win, SDL_Renderer* rend, struct mdata* map);
int get_row(struct mdata* map, float mouse_x, float mouse_y);
int get_column(struct mdata* map, float mouse_x, float mouse_y);
void cam_l(struct mdata* map);
void cam_u(struct mdata* map);
void cam_r(struct mdata* map);
void cam_d(struct mdata* map);
