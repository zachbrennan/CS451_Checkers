/*
    Description:
        This program can be used to test the user interface functionality,
        by:
            getting a piece selection from the player 

    To Compile:
        gcc display.c user_interface.h TEST_user_interface.c -lncurses

    To Run:
        ./a.out
        ***Press any key to continue the program after each display***
*/

#include <stdio.h>
#include <game/board.h>
#include <game/user_interface.h>


moove*
create_test_moves(board b)
{
		static moove m[7];
		m[0].from = 20;
		m[0].dest = 16;
		m[1].from = 20;
		m[1].dest = 17;
		m[2].from = 21;
		m[2].dest = 17;
		m[3].from = 21;
		m[3].dest = 18;
		m[4].from = 22;
		m[4].dest = 18;
		m[5].from = 22;
		m[5].dest = 19;
		m[6].from = 23;
		m[6].dest = 19;
		
		return m;
}



void
test_piece_selection(board b)
{
		moove *m = create_test_moves(b);
		
		int i = get_move_selection(b, m, 7);

		char msg[100];
		sprintf(msg, "User selected a move: %d -> %d", m[i].from, m[i].dest);
		display_message(msg, FALSE);
    getch();
}





int
main(int argc, char** argv)
{
    init_display();
		board b = init_board();
    
    display_board(b);

    test_piece_selection(b);

		free_board(b);
    close_display();	

    return TRUE;
}
