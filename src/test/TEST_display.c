/*
	Description:
		This program can be used to test the display functionality,
		by:
			drawing the intitial board
			drawing 24 basic pieces at start positions
			drawing 8 king pieces at end rows
			drawing highlight around top-left piece
			drawing blinking highlight around 2nd row, 3rd column piece

	To Compile:
		gcc display.c TEST_display.c -lncurses
	To Run
		./a.out
		***Press any key to continue the program after each display***
*/


#include <game/board.h>
#include <game/display.h>


void
test_display_piece_king(board board)
{
	int c;

	for(c = 0; c < 4; c++) {
		board.pieces[c].state = ROYAL;
		board.pieces[31-c].state = ROYAL;
	}
	display_board(board);
	getch();
}



void
test_highlight_piece(board board)
{
	draw_highlight(0, FALSE);
	draw_highlight(11, TRUE);

	refresh();
	getch();
}



void test_display_message(board b) {
	char *passive_msg = "This is a test passive message!";
	display_message(passive_msg, FALSE);

	getch();

	char *active_msg = "THIS IS SUPER IMPORTANT!";
	display_message(active_msg, TRUE, b);
	getch();
}

int
main(int argc, char** argv)
{
	init_display();

	board b = init_board();

	display_board(b);
	getch();

	test_display_piece_king(b);

	test_highlight_piece(b);

	test_display_message(b);
	
  free_board(b);
	close_display();
	return 0;	
}
