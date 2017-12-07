/*
  Description:
		This file describes methods for getting user
		input required for a game of checkers.

	To Compile:
		gcc display.c user_interface.c -lncurses
*/


#ifndef _SRC_GAME_USER_INTERFACE_H_
#define _SRC_GAME_USER_INTERFACE_H_


#include <stdlib.h>

#include <game/board.h>
#include <game/move.h>
#include <game/display.h>



/*
	Given a list of indexing of pieces, return the index
	of the piece the user has selected.

	*** MUST CALL init() FIRST (display.h) ***
*/
int get_piece_selection( int pieces[], int n );



/*
	Inputs:
		board - the game board
		m[]   - list of moves to select from
		n     - size of the list
	
	Returns:
		int - the index of m[] the user selected
			*** returns -1 on error ***
*/
int get_move_selection( board b, moove m[], int n );



/*
	Inputs:
		from	int:	-1 if no from to restrict search to, index
			of m[].from to restrict to otherwise
		move	m[]:	list of moves to search through
		n			int:	size of m[]
		s			int*:	pointer to an variable who's value will be
			set to the size of the resulting array
*/
int* unique_indecies( int from, moove m[], int n, int *s);



/*
	Inputs:
		k		int: 		value to search the array for
		l		int[]:	list to search through
		n		int:		size of l[]
*/
int exists( int k, int l[], int n );

#endif /* _SRC_GAME_USER_INTERFACE_H_ */
