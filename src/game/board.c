#include <stdlib.h>
#include <game/board.h>

board
init_board()
{
	int i;

	board b;
	b.playerTurn = 0;
	b.pieces = malloc(32 * sizeof(piece));

	for (i = 0; i < 12; i++)
	{
		b.pieces[i].state = LOWLY;
		b.pieces[i].color = RED;
	}
	for (i = 12; i < 20; i++)
	{
		b.pieces[i].state = EMPTY;
		b.pieces[i].color = NONE;
	}
	for (i = 20; i < 32; i++)
	{
		b.pieces[i].state = LOWLY;
		b.pieces[i].color = WHITE;
	}

	return b;
}

void free_board(board board)
{
	free(board.pieces);
}



board
copy_board(board b)
{
	board n = init_board();
	n.playerTurn = b.playerTurn;
	for (int i = 0; i < 32; i++)
	{
		n.pieces[i].state = b.pieces[i].state;
		n.pieces[i].color = b.pieces[i].color;
	}
	return n;
}



void get_position(int index, int *x, int *y)
{
	*x = index / 4;
	*y = index % 4;
}
