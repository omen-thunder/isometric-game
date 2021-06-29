#include <stdio.h>
#include<stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_W (640 * 2)
#define WINDOW_H (480* 2)
#define SPEED (200) // speed in pixels/second
#define SCALE(x) (x *= 2)
#define MAX_NPC (200)

struct npc {
	SDL_Texture* texture;
	SDL_Rect rect;
	float x_pos;
	float y_pos;
	float x_vel;
	float y_vel;
};

// destroys resources and exits SDL
void closeSDL(SDL_Renderer* rend, SDL_Window* win) {
	if (rend)
		SDL_DestroyRenderer(rend);
	if (win)
		SDL_DestroyWindow(win);
	SDL_Quit();
}

// creates an SDL window
int make_window(SDL_Window** win) {
	*win = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, (Uint32) 0);  
	if (!*win) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

// creates a SDL renderer
int make_renderer(SDL_Renderer** rend, SDL_Window* win) {
	*rend = SDL_CreateRenderer(win, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!*rend) {
		printf("Error creating renderer: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

// creates and returns an SDL texture from a given file path
int load_texture(SDL_Renderer* rend, SDL_Texture** tex, char* path) {
	// load the image into memory using SDL_image library function
	SDL_Surface* surface = IMG_Load("resources/man.gif");
	if (!surface) {
		fprintf(stderr, "Error creating surface: %s\n", SDL_GetError());
		return -1;
	}

	// load the image data into the graphics hardware's memory
	*tex = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);
	if (!*tex) {
		fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
		return -1;
	}
	
	return 0;
}

int main(void) {
	// attempt to initialise graphics and timer system
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) {
		fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* win;
	if (make_window(&win)) {
		closeSDL(NULL, NULL);
		return 1;
	}
	
	SDL_Renderer* rend;
	if (make_renderer(&rend, win)) {
		closeSDL(NULL, win);
		return 1;
	}

	SDL_Texture* man_tex;
	if (load_texture(rend, &man_tex, "resources/man.gif")) {
		closeSDL(rend, win);
		return 1;
	}

	//struct to hold the position and size of the sprite
	SDL_Rect man_dest;

	// get and scale the dimensions of texture
	SDL_QueryTexture(man_tex, NULL, NULL, &man_dest.w, &man_dest.h);
	SCALE(man_dest.w);
	SCALE(man_dest.h);

	bool close_requested = false;

	// array of NPCs
	struct npc npc_arr[MAX_NPC];
	unsigned num_npc = 0;

	// animation loop
	while (!close_requested) {
		// get cursor position
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);

		// process events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					close_requested = true;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT && num_npc < MAX_NPC) {
						unsigned i = num_npc;
						npc_arr[i].texture = man_tex;
						npc_arr[i].rect = man_dest;
						npc_arr[i].x_pos = (float) (mouse_x - man_dest.w / 2.0);
						npc_arr[i].y_pos = (float) (mouse_y - man_dest.h / 2.0);
						npc_arr[i].x_vel = SPEED;
						npc_arr[i].y_vel = SPEED;
						num_npc++;
					}
					break;
			}
		}

		// clear the window
		SDL_RenderClear(rend);

		for (int i = 0; i < num_npc && i < MAX_NPC; i++) {
			float* x_pos = &npc_arr[i].x_pos;
			float* y_pos = &npc_arr[i].y_pos;
			float* x_vel = &npc_arr[i].x_vel;
			float* y_vel = &npc_arr[i].y_vel;

			// collision detection
			if (*x_pos <= 0) {
				*x_vel = SPEED;
			}
			if (*y_pos <= 0) {
				*y_vel = SPEED;
			}
			if (*x_pos >= WINDOW_W - npc_arr[i].rect.w) {
				*x_vel = -SPEED;
			}
			if (*y_pos >= WINDOW_H - npc_arr[i].rect.h) {
				*y_vel = -SPEED;
			}

			// update positions
			*x_pos += *x_vel / 60;
			*y_pos += *y_vel / 60;

			// set the positions in the struct
			npc_arr[i].rect.x = (int) *x_pos;
			npc_arr[i].rect.y = (int) *y_pos;

			SDL_RenderCopy(rend, npc_arr[i].texture, NULL, &npc_arr[i].rect);
		}


		// display the window
		SDL_RenderPresent(rend);

		// wait 1/60th of a second
		SDL_Delay(1000/60);
	}

	// clean up resources before exiting
	SDL_DestroyTexture(man_tex);
	closeSDL(rend, win);
	return 0;
}
