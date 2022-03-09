#include "game.h"

int push_npc(Npc** npc_head, int col, int row) {
	Npc* new_npc;
	if (!(new_npc = malloc(sizeof(Npc)))) {
			fprintf(stderr, "malloc() failed\n");
			return -1;
	}

	new_npc->col = col;
	new_npc->row = row;
	new_npc->sprite.type = NPC;
	new_npc->sprite.tab_id = L_PLEB;
	new_npc->sprite.tex_index = 0;
	new_npc->sprite.rand_x = 0;
	new_npc->sprite.rand_y = 0;
	new_npc->next = *npc_head;
	*npc_head = new_npc;
	return 0;
}

void pop_npc(Npc** npc_head) {
	Npc* next_npc = (*npc_head)->next;
	free(*npc_head);
	*npc_head = next_npc;
}

void print_npcs(Npc* npc_head) {
	Npc* current = npc_head;
	while (current) {
		printf("col:%d row:%d\n", current->col, current->row);
		current = current->next;
	}
}
