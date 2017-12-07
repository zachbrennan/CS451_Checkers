#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <game/game_logic.h>
#include <game/board.h>



/*
  Returns:
    1 - basic move made
    2 - jump was made
    3 - jump was made and a piece was kinged
*/
int
make_move(board board, int player, moove move)
{
    FILE * fp = fopen("move_list.txt", "a+");
    int action = 1;
    int c = crowning_check(board, move);    

    piece tmp;
    tmp.state = board.pieces[move.from].state;
    tmp.color = board.pieces[move.from].color;
 
    board.pieces[move.from].state = board.pieces[move.dest].state;
    board.pieces[move.from].color = board.pieces[move.dest].color;

    board.pieces[move.dest].state = tmp.state;
    board.pieces[move.dest].color = tmp.color;

    fprintf(fp, "Moving from %d to %d.", move.from, move.dest);
    if(abs(move.dest - move.from) > 6)
    {
        int col = ( 
                    (
                        ((move.from / 4 + 1) % 2)
                        +
                        move.from % 4 * 2
                    )
                    +
                    (
                        ((move.dest / 4 + 1) % 2)
                        +
                        move.dest % 4 * 2
                    )
                  )
                  / 2; // Average of columns
        int row = ((move.from / 4) + (move.dest / 4)) / 2; // Average of rows
        int index = (4 * row) + (col / 2);

        fprintf(fp, " Jumped! Removing %d (row: %d, col: %d)\n", index, row, col);
        
        board.pieces[index].state = EMPTY;
        board.pieces[index].color = NONE;
        action = 2;
    }
    if(c)
    {
        fprintf(fp, " Crowning the piece at %d.", move.dest);
        board.pieces[move.dest].state = ROYAL;
        if(action == 2)
            action++;
    }
    fprintf(fp, "\n");
    fclose(fp);
    return action;
}

int
force_jump(board board, int player)
{
    FILE *fp = fopen("move_list.txt", "a+");
    int jump_index = -1;
    for (int j = 0; j < 32; j++)
    {
        if(!player && board.pieces[j].color == RED)
        {
            if (
                j <= 22
                  &&
                  j % 4 != 3
                  &&
                  board.pieces[j + 9].state == EMPTY
                  &&
                  (
                    (j / 4 % 2 == 0 && board.pieces[j + 5].color == WHITE)
                    ||
                    (j / 4 % 2 == 1 && board.pieces[j + 4].color == WHITE)
                  )
                )
                jump_index = j+9;
                     else if (
                       j <= 23
                       &&
                       j % 4 != 0
                       &&
                       board.pieces[j + 7].state == EMPTY
                       &&
                       (
                         (j / 4 % 2 == 0 && board.pieces[j + 4].color == WHITE)
                         ||
                         (j / 4 % 2 == 1 && board.pieces[j + 3].color == WHITE)
                       )
                     )
                     jump_index = j+7;

             else if (board.pieces[j].state == ROYAL)
             {
                 if (
                      j >= 9
                      &&
                      j % 4 != 0
                      &&
                      board.pieces[j - 9].state == EMPTY
                      &&
                      (
                        (j / 4 % 2 == 0 && board.pieces[j - 4].color == WHITE)
                        ||
                        (j / 4 % 2 == 1 && board.pieces[j - 5].color == WHITE)
                      )
                    )
                    jump_index = j-5;

                 else if (
                           j >=8
                           &&
                           j % 4 != 3
                           &&
                           board.pieces[j - 7].state == EMPTY
                           &&
                           (
                             (j / 4 % 2 == 0 && board.pieces[j - 3].color == WHITE)
                             ||
                             (j / 4 % 2 == 1 && board.pieces[j - 4].color == WHITE)
                           )
                         )
                         jump_index = j - 4;
                 }
                 if(jump_index != -1)
                 { 
                     fprintf(fp, "Piece: %d owned by Red player thinks it can jump to %d (also add +1 for negative values.\n", j, jump_index);
                     fclose(fp);
                     return 1;
                 }
          } else if(player && board.pieces[j].color == WHITE)
          {
                  if (
                       j >= 9
                       &&
                       j % 4 != 0
                       && board.pieces[j - 9].state == EMPTY
                       &&
                       (
                         (j / 4 % 2 == 0 && board.pieces[j - 4].color == RED)
                         ||
                         (j / 4 % 2 == 1 && board.pieces[j - 5].color == RED)
                       )
                     )
                     jump_index = j - 9;

                  else if (
                            j >= 8
                            &&
                            j % 4 != 3
                            &&
                            board.pieces[j - 7].state == EMPTY
                            &&
                            (
                              (j / 4 % 2 == 0 && board.pieces[j - 3].color == RED)
                              ||
                              (j / 4 % 2 == 1 && board.pieces[j - 4].color == RED)
                            )
                          )
                          jump_index = j - 7;

                  else if (board.pieces[j].state == ROYAL)
                  {
                      if (
                           j <= 22
                           &&
                           j % 4 != 3
                           &&
                           board.pieces[j + 9].state == EMPTY
                           &&
                           (
                             (j / 4 % 2 == 1 && board.pieces[j + 4].color == RED)
                             ||
                             (j / 4 % 2 == 0 && board.pieces[j + 5].color == RED)
                           )
                         )
                         jump_index = j + 9;

                      else if (
                                j <= 23
                                &&
                                j % 4 != 0
                                &&
                                board.pieces[j + 7].state == EMPTY
                                &&
                                (
                                  (j / 4 % 2 == 1 && board.pieces[j + 3].color == RED)
                                  ||
                                  (j / 4 % 2 == 0 && board.pieces[j + 4].color == RED)
                                )
                              )
                              jump_index = j + 7;
                  }
                  if(jump_index != -1)
                  { 
                      fprintf(fp, "Piece: %d owned by White player thinks it can jump to %d (also add +1 for negative values.\n", j, jump_index);
                      fclose(fp);
                      return 1;
                  }
            }
    }
    return 0;
}



int has_winner(board b) {
    int p_0 = 0;
    int p_1 = 0;

    for (int i = 0; i < 32; i++) {
        if (b.pieces[i].color == RED)
            p_0 = 1;
        else if (b.pieces[i].color == WHITE)
            p_1 = 1;

        if (p_0 && p_1)
            return -1;
    }
    if (p_0 && !p_1)
        return 0;
    else if (!p_0 && p_1)
        return 1;
    else // Code execution should not hit this point
        return -1;
}



int
is_valid_move(board board, moove move)
{
    if (board.pieces[move.from].state) //TODO: This was == true (????)
    {
        return 0;
    }
    if (board.pieces[move.from].state == board.pieces[move.dest].state)
    {
        return 0;
    }

    if (board.pieces[move.from].color == RED)
    {
        if ((board.pieces[move.from].state == LOWLY)
            && ((board.pieces[move.from + 4].state == EMPTY || board.pieces[move.from + 5].state == EMPTY)
                || (board.pieces[move.from + 3].state == EMPTY || board.pieces[move.from + 4].state == EMPTY)
                || ((board.pieces[move.from + 4].state != EMPTY || board.pieces[move.from + 5].state != EMPTY) && board.pieces[move.from + 9].state == EMPTY)
                || ((board.pieces[move.from + 3].state != EMPTY ||    board.pieces[move.from + 4].state != EMPTY) && board.pieces[move.from + 7].state == EMPTY))
            )
        {
            return 1;
        }
        if ((board.pieces[move.from].state == LOWLY || board.pieces[move.from].state == ROYAL) && board.pieces[move.from].color == WHITE)
        {
            return 0;
        }
    }
    else if (board.pieces[move.from].color == WHITE)
    {
        if (board.pieces[move.from].state == LOWLY &&
            ((board.pieces[move.from - 3].state == EMPTY || board.pieces[move.from - 4].state == EMPTY) ||
            (board.pieces[move.from - 4].state == EMPTY || board.pieces[move.from - 5].state == EMPTY) ||
                ((board.pieces[move.from - 3].state != EMPTY || board.pieces[move.from - 4].state != EMPTY) && board.pieces[move.from - 7].state == EMPTY) ||
                ((board.pieces[move.from - 4].state != EMPTY || board.pieces[move.from - 5].state != EMPTY) && board.pieces[move.from - 9].state == EMPTY))
            )
        {
            return 1;
        }
        if ((board.pieces[move.from].state == LOWLY || board.pieces[move.from].state == ROYAL) && board.pieces[move.from].color == RED)
        {
            return 0;
        }
    }

    if (board.pieces[move.from].state == ROYAL &&
        ((board.pieces[move.from + 4].state == EMPTY || board.pieces[move.from + 5].state == EMPTY) ||
        (board.pieces[move.from + 3].state == EMPTY || board.pieces[move.from + 4].state == EMPTY) ||
            (board.pieces[move.from - 3].state == EMPTY || board.pieces[move.from - 4].state == EMPTY) ||
            (board.pieces[move.from - 4].state == EMPTY || board.pieces[move.from - 5].state == EMPTY) ||
            ((board.pieces[move.from + 4].state != EMPTY || board.pieces[move.from + 5].state != EMPTY) && board.pieces[move.from + 9].state == EMPTY) ||
            ((board.pieces[move.from + 3].state != EMPTY || board.pieces[move.from + 4].state != EMPTY) && board.pieces[move.from + 7].state == EMPTY) ||
            ((board.pieces[move.from - 3].state != EMPTY || board.pieces[move.from - 4].state != EMPTY) && board.pieces[move.from - 7].state == EMPTY) ||
            ((board.pieces[move.from - 4].state != EMPTY || board.pieces[move.from - 5].state != EMPTY) && board.pieces[move.from - 9].state == EMPTY))
        )
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

/* move[] get_all_moves(int player){return mseq;} */
/* move[] get_moves(coord pos, int onlyJumps){return mseq;} */
moove *
get_possible_move(board board, int j, int onlyjumps, int *n)
{
    FILE *fp = fopen("move_list.txt", "a+");
    moove moves[4];
    piece piece = board.pieces[j];
    fprintf(fp, "Piece: %d, Color: %d, onlyjumps: %d\n", j, piece.color, onlyjumps);

    if(board.pieces[j].color == NONE)
    {
        *n = 0;
        return NULL;
    }

    int total = 0;
    if (onlyjumps)
    {
        if(board.pieces[j].color == RED)
        {
            if (
                j <= 22
                  &&
                  j % 4 != 3
                  &&
                  board.pieces[j + 9].state == EMPTY
                  &&
                  (
                    (j / 4 % 2 == 0 && board.pieces[j + 5].color == WHITE)
                    ||
                    (j / 4 % 2 == 1 && board.pieces[j + 4].color == WHITE)
                  )
                )
                {
                    moves[total].from = j;
                    moves[total].dest = j+9;
                    total++;  
                }
            if (
                   j <= 23
                   &&
                   j % 4 != 0
                   &&
                   board.pieces[j + 7].state == EMPTY
                   &&
                   (
                     (j / 4 % 2 == 0 && board.pieces[j + 4].color == WHITE)
                     ||
                     (j / 4 % 2 == 1 && board.pieces[j + 3].color == WHITE)
                    )
                )
                {
                    moves[total].from = j;
                    moves[total].dest = j+7;
                    total++;  
                }
             if (board.pieces[j].state == ROYAL)
             {
                 if (
                      j >= 9
                      &&
                      j % 4 != 0
                      &&
                      board.pieces[j - 9].state == EMPTY
                      &&
                      (
                        (j / 4 % 2 == 0 && board.pieces[j - 4].color == WHITE)
                        ||
                        (j / 4 % 2 == 1 && board.pieces[j - 5].color == WHITE)
                      )
                    )
                {
                    moves[total].from = j;
                    moves[total].dest = j - 9;
                    total++;  
                }

                if (
                           j >= 8
                           &&
                           j % 4 != 3
                           &&
                           board.pieces[j - 7].state == EMPTY
                           &&
                           (
                             (j / 4 % 2 == 0 && board.pieces[j - 3].color == WHITE)
                             ||
                             (j / 4 % 2 == 1 && board.pieces[j - 4].color == WHITE)
                           )
                         )
                    {
                        moves[total].from = j;
                        moves[total].dest = j - 7;
                        total++;  
                    }

                 }

          } else if(board.pieces[j].color == WHITE)
          {
                  if (
                       j >= 9
                       &&
                       j % 4 != 0
                       && board.pieces[j - 9].state == EMPTY
                       &&
                       (
                         (j / 4 % 2 == 0 && board.pieces[j - 4].color == RED)
                         ||
                         (j / 4 % 2 == 1 && board.pieces[j - 5].color == RED)
                       )
                     )
                {
                    moves[total].from = j;
                    moves[total].dest = j - 9;
                    total++;  
                }

                if (
                            j >= 8
                            &&
                            j % 4 != 3
                            &&
                            board.pieces[j - 7].state == EMPTY
                            &&
                            (
                              (j / 4 % 2 == 0 && board.pieces[j - 3].color == RED)
                              ||
                              (j / 4 % 2 == 1 && board.pieces[j - 4].color == RED)
                            )
                          )
                {
                    moves[total].from = j;
                    moves[total].dest = j - 7;
                    total++;  
                }

                if (board.pieces[j].state == ROYAL)
                  {
                      if (
                           j <= 22
                           &&
                           j % 4 != 3
                           &&
                           board.pieces[j + 9].state == EMPTY
                           &&
                           (
                             (j / 4 % 2 == 1 && board.pieces[j + 4].color == RED)
                             ||
                             (j / 4 % 2 == 0 && board.pieces[j + 5].color == RED)
                           )
                         )
                    {
                        moves[total].from = j;
                        moves[total].dest = j + 9;
                        total++;  
                    }

                    if (
                                j <= 23
                                &&
                                j % 4 != 0
                                &&
                                board.pieces[j + 7].state == EMPTY
                                &&
                                (
                                  (j / 4 % 2 == 1 && board.pieces[j + 3].color == RED)
                                  ||
                                  (j / 4 % 2 == 0 && board.pieces[j + 4].color == RED)
                                )
                              )
                    {
                        moves[total].from = j;
                        moves[total].dest = j + 7;
                        total++;  
                    }

                  }

            }

    } else
    {
        if (piece.state == ROYAL)
        {
            if (j % 4 != 0 && j / 4 % 2 == 1 && board.pieces[j + 3].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j + 3;
                
                total++;
            }
            if (j <= 27 && board.pieces[j + 4].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j + 4;
                
                total++;
            }
            if (j / 4 % 2 == 0 && j % 4 != 3 && board.pieces[j + 5].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j + 5;
                
                total++;
            }
            if (j / 4 % 2 == 0 && j % 4 != 3 && board.pieces[j - 3].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j - 3;
                
                total++;
            }
            if (j >= 4 && board.pieces[j - 4].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j - 4;
                
                total++;
            }
            if (j / 4 % 2 == 1 && j % 4 != 0 && board.pieces[j - 5].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j - 5;
                
                total++;
            }
        }
        else if (piece.color == RED)
        {
            if (j % 4 != 0 && j / 4 % 2 == 1 && board.pieces[j + 3].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j + 3;
                
                total++;
            }
            if (j <= 27 && board.pieces[j + 4].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j + 4;
                
                total++;
            }
            if (j / 4 % 2 == 0 && j % 4 != 3 && board.pieces[j + 5].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j + 5;
                
                total++;
            }

        }
        else if (piece.color == WHITE)
        {
            if (j / 4 % 2 == 0 && j % 4 != 3 && board.pieces[j - 3].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j - 3;
                
                total++;
            }
            if (j >= 4 && board.pieces[j - 4].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j - 4;
                
                total++;
            }
            if (j / 4 % 2 == 1 && j % 4 != 0 && board.pieces[j - 5].state == EMPTY)
            {
                moves[total].from = j;
                moves[total].dest = j - 5;
                
                total++;
            }

        }
    }
    fprintf(fp, "    Found %d moves.\n", total);
    fclose(fp);
    if(total == 0) {
        *n = 0;
        return NULL;
    }
    else
    {
        moove *results = malloc(total * sizeof(moove));
        for(int i = 0; i < total; i++) {
            results[i].from = moves[i].from;
            results[i].dest = moves[i].dest;
        }
        *n = total;
        return results;
    }
}



/* Evaluates all moves an individual piece can make. If only jumps is 1, then only moves consisting of a jump are returned. */
moove *
get_possible_moves(board board, int player, int *n)
{
    FILE *fp = NULL;
    int i, j, size;
    int total = 0;

    moove *moves = NULL, *mp, *foo = NULL;
    moves = malloc(sizeof(moove) * 48); /* max 48 ever possible */
    mp = moves;

    /* Check to see if a player has to jump */
    int onlyjumps = force_jump(board, player);

    /* Check each piece's moves and return them. */
    for (i = 0; i < 32; i++)
    {
        if(board.pieces[i].color == player) {
            foo = get_possible_move(board, i, onlyjumps, &size);
                for(j = 0; j < size; j++)
                {
                fp = fopen("move_list.txt", "a+");
                fprintf(fp, "    Adding Move - Piece: %d, Move - from: %d, dest: %d, total: %d\n", i, foo[j].from, foo[j].dest, total);
                fclose(fp);
                      mp[total] = foo[j];
                total++;
                }
        }
    }
    // Reallocate the array to only what was used
    moves = realloc(moves, total * sizeof(moove));
    *n = total;
    fp = fopen("move_list.txt", "a+");
    fprintf(fp, "      Found %d total moves for player\n", total);
    fclose(fp);
    return moves;
}

/* Evaluates each piece of the provided player for available moves, returning a list of all 1st generation moves. If a jump can be made, only jumps will be returned. */

int
crowning_check(board b, moove move)
{
      int row = move.dest / 4;
    int player = b.pieces[move.from].color;

      if (b.pieces[move.from].state == LOWLY && ((player && row == 0) || (!player && row == 7)))
        return 1;
     else
        return 0;
}
