#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_W (640 * 2)
#define WINDOW_H (480* 2)

void closeSDL(SDL_Window* win, SDL_Renderer* rend);
int make_window(SDL_Window** win);
int make_renderer(SDL_Window* win, SDL_Renderer** rend);
int load_texture(SDL_Renderer* rend, SDL_Texture** tex, char* path);
