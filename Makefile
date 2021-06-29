CC = gcc
CFLAGS = -g -O0 -Wall  
LDFLAGS = -lm

SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer)
SDL_LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer)
override CFLAGS += $(SDL_CFLAGS)
override LDFLAGS += $(SDL_LDFLAGS)

test:
	$(CC) game.c -o exec $(CFLAGS) $(LDFLAGS) 
