/*
  Description:
		This file describes methods for displaying a game
		of checkers to the terminal.

	Example Usage:
		init(); // Initialize ncurses
		draw_board(); // Draw the game board
		draw_piece(piece); // Draw a piece
		render(); // Render to the screen
		getch(); // Wait for a key to be pressed
		close(); // Close down ncurses

	To Compile:
		gcc <<file.c>> -lncurses
*/


#ifndef _SRC_GAME_DISPLAY_H_
#define _SRC_GAME_DISPLAY_H_


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ncurses.h>


#include <fonts.h>
#include <game/board.h>


#define W 14 /* Width of a square on the board  */
#define H 7 /* Height of a square on the board */
#define S 8 /* Number of squares in a row on the board */
#define OFFSET 2 /* Horizontal & Vertical offset from top-left before board is drawn */
#define W_MAX (W * S + 2 * OFFSET) /* Maximum width of the board */
#define H_MAX (H * S + 2 * OFFSET) /* Maximum height of the board */

#define ENTER_KEY 10 /* Use this instead of KEY_ENTER as it is unreliable*/
#define BACKSPACE_KEY 127 /* Use this instead of KEY_BACKSPACE */
#define DELETE_KEY 8
#define ACS_FULL (' ' |A_REVERSE)


/*
	This data structure describes the shape of basic
	game pieces.

	draw_piece(piece piece) uses this to draw the shapes.

	Defined in display.c
	
	0 - Transparent
	1 - Player Color
*/
int piece_basic [H][W];



/*
	This data structure describes the shape of basic
	game pieces.

	draw_piece(piece piece) uses this to draw the shapes.

	Defined in display.c
	
	0 - Transparent
	1 - Player Color
*/
int piece_king [H][W];


/*
	Initializes the terminal to work with ncurses. This
	includes:
		initializing stdscr
		initializes colors
		overwrites COLOR_GREEN to be a gray for black player
		Sets input to pass each character, not wait until
			return
		Sets keyboard mode to allow reading of arrow keys

	*** MUST CALL close() WHEN DONE ***
*/
int init_display();


/*
	Cleans up ncurses window, reverting the terminal to
	standard input.
	Includes:
		Closes the ncurses window
		Re-initializes terminal colors (this prevents
			changed colors from remaining changed after the
			program exits).

	*** MUST CALL init() FIRST ***
*/
int close_display();


/*
	Draws a square of the provided COLOR_PAIR index at the
	coordinate. The "y" and "x" should correspond to the
	row and column of both black and white squares
	(0~7, not 0~3). This allows drawing black and white
	squares (black squares are useful for clearing a
	square).

	*** MUST CALL init() FIRST ***

	*** DOES NOT CAUSE A RENDER ***

	Returns 0 on success
*/
int draw_square(int y, int x, int color);



/*
	Draws a piece to the window. "index" should be the
	index in "board" of the piece to draw

	*** MUST CALL init() FIRST ***

	*** DOES NOT CAUSE A RENDER ***

	Returns 0 on success.
*/
int draw_piece(board board, int index);



/*
	Draws a highlight around the square containing the piece.
	"index" should be the index of the piece in board

	is_blinking should be a 0 if the highlight should have a
	blinking effect, or 1 to have no blink

	*** MUST CALL init() FIRST***

	*** DOES NOT CAUSE A RENDER ***
*/
int draw_highlight(int index, int is_blinking);



/*
	Draws the game board to the window using black and white
	squares

	*** MUST CALL init() FIRST ***

	*** DOES NOT CAUSE A RENDER ***

	Returns 0 on success.
*/
int draw_board();



/*
	Display the board to the screen

	*** MUST CALL init() FIRST ***

	*** THIS WILL CAUSE A RENDER ***
*/
void display_board(board board);



/*
	Display a message msg to the window.
		is_urgent == 0 : Displays the message over the board.
			requires the enter key be pressed to continue
		is_urgent == 1 : Displays the message at the top of
			the screen.

		Optional:
			board - the board to use when redrawing after an
				active message

	*** MUST CALL init() FIRST ***

	*** WILL CAUSE A RENDER ***
*/
int display_message(char *msg, int is_urgent, ...);


#endif /* _SRC_GAME_DISPLAY_H_ */
