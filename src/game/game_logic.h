/*
	I think we should rename this file to be "logic.h"
	Since this is in a game/ folder, it would make it
		look a little cleaner(?) Just my opinion though,
		not urgent
*/
#ifndef _SRC_GAME_GAME_LOGIC_H_
#define _SRC_GAME_GAME_LOGIC_H_

#include <stdio.h>


typedef char * String;

#include <time.h> /* Do we need a time library? */
#include <game/display.h>

#include "board.h"
#include "move.h"


/*
	I'm not sure what this method does. If it is making a
		move on the board, it should be taking a move as an
		input instead of a player, and it can then assume
		that "is_valid_move" has already been called.

	If it is getting a move from the user, then I wouldn't
		use this. The "user_interface.h" will have a method
		to get a move selection from the user. The client
		code will use "get_possible_moves" to get the moves,
		then "get_user_selection" to get their choice, and
		finally use "is_valid_move" and "make_move" to
		apply the move to the board.
*/
int make_move(board board, int player, moove move);



/*
	This method takes a board and a player, and returns
		true if it finds a piece for that player that can
		make a jump(?) If so, then I would rename this to
		"has_jump" to make it a little clearer. This would
		be called by "get_possible_moves" to determine if
		it needs to send only_jumps as true to
		"get_possible_move"
*/
int force_jump(board board, int player);



/*
  Given a board, checks if either player has run
  out of pieces. Returns the player int of the winner
  or "-1" otherwise 
*/
int has_winner(board b);



/*
	Inputs:
		board - the board to check the moves against
		moves[] - the sequence of moves (or single move in
			the case of a non_jump) to validate
		size - the length of moves[]

	Returns:
		true - the sequence of moves was valid
			(does this check for possible jumps?)
		false - the sequence of moves was invalid
*/
int is_valid_move(board board, moove moves);



/*
	I would rename this to "get_possible_moves" as it can
		return more than one move for the piece.

	I would rename "only-jumps" to "only_jumps" for
		consistancy
*/
moove *
get_possible_move(board board, int loc, int onlyjumps, int *n);


/*
	I would rename this to "get_available_moves" for better
		clarity.

	Inputs:
		board - the board to search for possible moves
		player - the player to get moves for

	Returns:
		move[] - array of possible moves
			*** if a jump is found, only returns moves
				containing jumps ***
			*** does not return jump sequences (only the
				first jump that could be made) ***
*/
moove *
get_possible_moves(board board, int player, int *n);



/*
	Inputs:
		player - the player who is making the move
		move - the move to check

	Returns:
		bool - true if the move ends in the opponents'
			back row
*/
int crowning_check(board b, moove move);


#endif /* _SRC_GAME_GAME_LOGIC_H_ */
