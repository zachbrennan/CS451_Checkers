/*============================================================*\
Filename : src/game/board.c
Purpose  : Multiplayer, networked Checkers game for CS451.
Authors  : Corwin Belser <cmb539@drexel.edu>
           Zach Brennan  < zab37@drexel.edu>
           Zach van Rijn < zwv23@drexel.edu>
License  : MIT/X (excl. external libs; see respective licenses).
Revision : 20170628
\*============================================================*/

#ifndef _SRC_GAME_BOARD_H_
#define _SRC_GAME_BOARD_H_



/*------------------------------------------------------------*/

/*
* We will eventually want to render the checker board and pieces
* in some less-boring way. In this case, we can use alternative
* colors to distinguish the two pieces, even allowing players to
* pick their favorite colors. Note: do not add any colors to the
* enumerated type below; there can *only* be two "colors" at any
* given time. If you want to add color options (for example, for
* the graphical user interface), we can discuss that later.
*/

typedef enum CHECKERS_PIECE_COLORS
{
    RED,
    WHITE,
    NONE //Added this for pieces that have empty as the state
} color;



/*------------------------------------------------------------*/

/*
* Possibly a little counterintuitive, we are going to construct
* a board of *pieces* and not of *squares* because each piece is
* effectively just the "state" of a square and we can store the
* state within a piece and treat it as a square.
*
* Depending on how complicated the scoring system needs to be,
* we may need to add extra logic to keep track of the number of
* pieces on the board when a player has one or more "kings" --
* this means fewer squares than pieces are occupied, but there
* are not enough "dead" players off the board.
*/

typedef enum CHECKERS_PIECE_STATE
{
    ERROR,                              /* move validation    */
    EMPTY,                              /* square is empty    */
    LOWLY,                              /* regular player     */
    ROYAL                               /* kinged  player     */
} state;

typedef struct CHECKERS_PIECE
{
    state state;
    color color;
} piece;

typedef struct CHECKERS_BOARD
{
    piece *pieces;                  /* 1D uninitialized   */
    int playerTurn;                 /* Stores which player
                                       needs to make a move  */
} board;



/*
	Creates a board, initializing each piece to the initial
		board layout	
*/
board init_board();


/*
	Given a board, free's the memory allocated for the
		pieces
*/
void free_board(board board);


/*
	Given a board, returns a new board that is a copy of the
		provided board.
*/
board copy_board(board board);



/*
	Given an index and an xy pair (passed by reference),
		sets "x" and "y" to be the row and column of the
		piece in the squished board (8 rows, 4 columns)
*/
void get_position(int index, int *x, int *y);


#endif /* _SRC_GAME_BOARD_H_ */
