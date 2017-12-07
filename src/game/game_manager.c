#include <stdio.h>
#include <ncurses.h>
#include <game/game_manager.h>


mseq
take_turn(board b, int player)
{ 
    FILE *fp = fopen("move_list.txt", "a+");

    // Test board to apply test moves to
    board sandbox = copy_board(b);
    display_board(sandbox);

    // Initialize the move list to return
    mseq selected_moves;
    selected_moves.length = 0;


    // Get the list of possible moves
    int size;
    moove * choices = get_possible_moves(sandbox, player, &size);

    // Check for inability to make a move
    if (size == 0)
        return selected_moves;


    for(int i = 0; i < size; i++) {
      fprintf(fp, "    Move %d: (%d, %d)\n", i+1, choices[i].from+1, choices[i].dest+1);
    }

    int choice = get_move_selection(b, choices, size);


    int result = make_move(sandbox, player, choices[choice]); // Apply the move
    selected_moves.moves[selected_moves.length] = choices[choice];
    selected_moves.length++;

    //Update the display
    display_board(sandbox);

    while (result == 2)
    {
        int n;
        choices = get_possible_move(sandbox, choices[choice].dest, 1, &n);
        if (n != 0)
        {
            choice = get_move_selection(sandbox, choices, n);
            selected_moves.moves[selected_moves.length] = choices[choice];
            selected_moves.length++;
            result = make_move(sandbox, player, choices[choice]);
        } else
            result = -1;
        display_board(sandbox);
    }
    free_board(sandbox);
    fclose(fp);
    return selected_moves;
}



void
single_player_game()
{
    int player = 0;
    char red_player_msg[50] = "Red Player, It's Your Turn!";
    char white_player_msg[50] = "White Player, It's Your Turn!";
    board b = init_board();

    int winner;
    while ((winner = has_winner(b)) == -1)
    {
        display_board(b);
        if(player)
            display_message(white_player_msg, 0);
        else
            display_message(red_player_msg, 0);

        mseq turn = take_turn(b, player); // Take a turn

        if (turn.length == 0)
        {
            display_message("Player can't make a move! They Forfeit :(\n", 1, b);
            free_board(b);
            return;
        }

        //Apply the moves
        for (int i = 0; i < turn.length; i++)
            make_move(b, player, turn.moves[i]);

        player = (player + 1) % 2;
    }

    switch (winner)
    {
    case 0:
        display_message("RED WINS", 1, b);
        break;
    case 1:
        display_message("WHITE WINS", 1, b);
        break;
    default:
        display_message("ERROR", 1, b);
    }
    free_board(b);
}
