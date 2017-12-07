#ifndef MOVE_H_
#define MOVE_H_

#define MAX_MOVE 12

typedef
struct game_move
{
	int from;
	int dest;
} moove; /* ncurses already defines "move" as a function */

typedef
struct move_sequence
{
	moove moves[MAX_MOVE];
    int length;
} mseq;

#endif /* MOVE_H_ */
