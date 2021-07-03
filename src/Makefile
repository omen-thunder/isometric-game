CC = gcc
CFLAGS = -g -O4 -Wall  
LDFLAGS = -lm

DEPS = game.h
_OBJ = game.o setup.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

ODIR=obj

SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer)
SDL_LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer)
override CFLAGS += $(SDL_CFLAGS)
override LDFLAGS += $(SDL_LDFLAGS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS) 

exec: $(OBJ)
	$(CC) -o ../$@ $^ $(CFLAGS) $(LDFLAGS) 
