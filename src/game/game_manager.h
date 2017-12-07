#ifndef _SRC_GAME_GAME_MANAGER_H_
#define _SRC_GAME_GAME_MANAGER_H_


#include <math.h>


#include <game/board.h>
#include <game/move.h>
#include <game/user_interface.h>
#include <game/game_logic.h>

/*
	Inputs:
		board - game board
		player - 
			0 - RED
			1 - BLACK

	Returns:
		mseq - sequence of moves the player
			wishes to make (multiple moves if
			a sequence of jumps)
*/
mseq take_turn(board b, int player);



/*
	Plays a single player game in stdscr,
		returning when the game ends
*/
void single_player_game();


#endif /* _SRC_GAME_GAME_MANAGER_H_  */
