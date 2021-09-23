CC = gcc
CFLAGS = -g -O0 -Wall  
LDFLAGS = -lm

DEPS = src/game.h
_OBJ = main.o setup.o map.o camera.o water.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

SDIR=src
ODIR=$(SDIR)/obj

SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer)
SDL_LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer)
override CFLAGS += $(SDL_CFLAGS)
override LDFLAGS += $(SDL_LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS) 

exec: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 

.PHONY: clean

clean:
	-rm ./exec
	-rm $(ODIR)/*.o
