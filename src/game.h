#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_W (1280)
#define WINDOW_H (720)
#define NUM_TEX (2)

struct mdata {
	int** tiles;
	int** objs;
	unsigned size;
	SDL_Texture* textures[NUM_TEX];
	int x_off;
	int y_off;
};

void closeSDL(SDL_Window* win, SDL_Renderer* rend, struct mdata* map);
int make_window(SDL_Window** win);
int make_renderer(SDL_Window* win, SDL_Renderer** rend);
int load_texture(SDL_Renderer* rend, SDL_Texture** tex, char* path);
