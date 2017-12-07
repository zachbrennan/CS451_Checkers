#include <game/user_interface.h>

int
get_piece_selection(int *pieces, int n)
{
    int i;

    //display_message("having user select a piece...", 0);

    /* Blinking highlight for the first piece */
    draw_highlight(pieces[0], TRUE);

    /* Highlight remaining pieces */
    for (i = 1; i < n; i++)
        draw_highlight(pieces[i], FALSE);

    /* Get user input */
    int choice = 0;
    int ch;
    /*char msg[200];
    sprintf(msg, "Selection: %d / %d", choice+1, n);
    display_message(msg, FALSE);
    refresh();*/

    while ((ch = getch()) != ENTER_KEY)
    {
        if (ch == KEY_LEFT && choice != 0)
        {
            draw_highlight(pieces[choice], FALSE); /* Clear highlight */
            choice--;
            draw_highlight(pieces[choice], TRUE); /* Add highlight */
        }
        else if (ch == KEY_RIGHT && choice < n - 1)
        {
            draw_highlight(pieces[choice], FALSE); /* Clear highlight */
            choice++;
            draw_highlight(pieces[choice], TRUE); /* Add highlight */
        }
        else if (ch == KEY_BACKSPACE || ch == BACKSPACE_KEY || ch == DELETE_KEY)
            return -1;
        /*sprintf(msg, "Selection: %d / %d", choice, n-1);
        display_message(msg, FALSE);*/
        refresh();
    }

    return pieces[choice];
}



int
get_move_selection(board b, moove *m, int n)
{
    int i;
    int s_f, s_d;

    if (n == 0 || m == NULL)
        return -1;

    //Have user select start position
    int *froms = unique_indecies(-1, m, n, &s_f);
    while(1)
    {
        int from = get_piece_selection(froms, s_f);
        display_board(b); // Redraw the board
        if(from != -1)
        {
            draw_highlight(from, TRUE); // Keep the from piece selected

            //Have user select target position
            int *dests = unique_indecies(from, m, n, &s_d);
            int dest = get_piece_selection(dests, s_d);

            display_board(b);
            if(dest != -1)
            {
                //Find which move this was
                for (i = 0; i < n; i++)
                {
                    if (m[i].from == from && m[i].dest == dest)
                        return i;
                }
                return -1;
            }
        }
    }
}

int *
unique_indecies(int from, moove *m, int n, int *s)
{
    int i;
    int count = 0;
    int *l = malloc(n * sizeof(int));

    if (from == -1)
    {
        for (i = 0; i < n; i++)
        {
            if (!exists(m[i].from, l, count))
            {
                l[count] = m[i].from;
                count++;
            }
        }
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            if (m[i].from == from && !exists(m[i].dest, l, count))
            {
                l[count] = m[i].dest;
                count++;
            }
        }
    }
    *s = count;
    return l;
}



int
exists(int k, int *l, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (l[i] == k)
            return TRUE;
    }
    return FALSE;
}
