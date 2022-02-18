#include "game.h"

int main(void) {
	SDL_Window* win;
	SDL_Renderer* rend;
	Settings settings;
	Textures textures;
	Maps maps;
	Data data;

	if (init(win, rend, &settings, &textures, &maps, &data))
		return -1;

	animate(win, rend, &settings, &textures, &maps, &data);

	if (deinit(win, rend, &settings, &textures, &maps, &data))
		return -1;

	return 0;
}
