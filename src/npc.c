#include "game.h"

int create_npc(npc* npcp, int x, int y) {
	if(npcp) {
		printf("npc already exists\n");
		create_npc(npcp->next, x, y);
		return 0;
	}

	printf("no npc\n");

	if (!(npcp = malloc(sizeof(npc)))) {
		fprintf(stderr, "malloc() failed\n");
		return -1;
	}

	npcp->next = NULL;
	npcp->x = x;
	npcp->y = y;
	return 0;
}

int delete_npc(npc* npc_list, int x, int y) {
	return 0;
}
