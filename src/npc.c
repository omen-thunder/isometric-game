#include "game.h"

int push_npc(Npc** head, int col, int row) {
	Npc* current = *head;
	while (current) {
		if (current->start_col == col && current->start_row == row)
			return 0;

		current = current->next;
	}

	Npc* new_npc;
	if (!(new_npc = malloc(sizeof(Npc)))) {
			fprintf(stderr, "malloc() failed\n");
			return -1;
	}

	new_npc->start_col = col;
	new_npc->start_row = row;
	new_npc->goal_col = col;
	new_npc->goal_row = row;
	new_npc->sprite.type = NPC;
	new_npc->sprite.tab_id = L_PLEB;
	new_npc->sprite.tex_index = 0;
	new_npc->sprite.adj_x = 0;
	new_npc->sprite.adj_y = 0;
	new_npc->next = *head;
	*head = new_npc;
	return 0;
}

void pop_npc(Npc** head) {
	Npc* next_npc = (*head)->next;
	free(*head);
	*head = next_npc;
}

void del_npc(Npc** head, int col, int row) {
	if (!*head)
		return;

	if ((*head)->start_col == col && (*head)->start_row == row) {
		pop_npc(head);
		return;
	}

	Npc* prev = *head;
	Npc* current = (*head)->next;
	while (current) {
		if (current->start_col == col && current->start_row == row) {
			prev->next = current->next;
			free(current);
			return;
		}

		prev = current;
		current = current->next;
	}
}

Npc* find_npc(Npc* head, int col, int row) {
	Npc* current = head;
	while (current) {
		if (current->start_col == col && current->start_row == row)
			return current;
		current = current->next;
	}

	return NULL;
}

void print_npcs(Npc* head) {
	Npc* current = head;
	while (current) {
		printf("adj_x: %d, adj_y: %d\n", current->sprite.adj_x, current->sprite.adj_y);
		current = current->next;
	}
}

void move_npcs(Settings* settings_p, Data* data_p, Npc* head) {
	Npc* current = head;
	float vect_x = 0;
	float vect_y = 0;
	float mag = 0;
	float u_vect_x = 0;
	float u_vect_y = 0;
	while (current) {
		vect_x = current->goal_col - current->start_col;
		vect_y = current->goal_row - current->start_row;
		mag = sqrt(vect_x * vect_x + vect_y * vect_y);
		if (mag > 0) {
			u_vect_x = vect_x / mag;
			u_vect_y = vect_y / mag;
			current->buf_x += u_vect_x;
			current->buf_y += u_vect_y;
		}

		if (fabs(current->buf_x) >= 100) {
			current->sprite.adj_x += current->buf_x / 100;
			current->buf_x = 0;
		}

		if (fabs(current->buf_y) >= 100) {
			current->sprite.adj_y += current->buf_y / 100;
			current->buf_y = 0;
		}

		if (abs(current->sprite.adj_x) > ZOOM_SCALE(TILE_W)) {
			//current->start_col -= current->sprite.adj_x / ZOOM_SCALE(TILE_W);
			current->sprite.adj_x = 0;
		}

		if (abs(current->sprite.adj_y) > ZOOM_SCALE(TILE_H)) {
			//current->start_row -= current->sprite.adj_y / ZOOM_SCALE(TILE_H);
			current->sprite.adj_y = 0;
		}

		printf("buf_x: %f, buf_y: %f, adj_x: %d, adj_y: %d\n", current->buf_x, current->buf_y, current->sprite.adj_x, current->sprite.adj_y);
		current = current->next;
	}
}
