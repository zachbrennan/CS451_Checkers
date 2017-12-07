#include "display.h"


int piece_basic [H][W] = 
{
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}
};

/*
    0 - Transparent
    1 - Player Color
*/
int piece_king [H][W] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
    { 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0},
    { 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0},
    { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0},
    { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


int was_initted = 0;
int 
init_display()
{
    if(was_initted)
      return 0;
    was_initted = 1;
    initscr();          /* Initialize the screen */
    cbreak();           /* Send input to program, excluding control sequences */
    keypad(stdscr, TRUE); /* Enable reading of arrow keys */
    noecho();           /* Disable input being echo'd */
    curs_set(0);

		char msg[200];
    if(has_colors() == FALSE)
    {
        sprintf(msg, "Your terminal does not support colors!");
    }
    else
    {
        sprintf(msg, "Your terminal does support colors!");
        start_color();
        if(can_change_color())
        {
            init_color(COLOR_GREEN, 589, 589, 589); /* Override a color to use as grey */
        } else {
          sprintf(msg, "Your terminal does support colors, but not the changing of them :(");
        }
        init_pair(4, COLOR_WHITE, COLOR_WHITE); /* White Square */
        init_pair(5, COLOR_RED, COLOR_RED); /* Red Piece */
        init_pair(6, COLOR_GREEN, COLOR_GREEN); /* Black Piece */
        init_pair(7, COLOR_BLACK, COLOR_BLACK); /* Black Square */
        init_pair(8, COLOR_CYAN, COLOR_CYAN); /* Highlight Color */
        init_pair(9, COLOR_MAGENTA, COLOR_MAGENTA); /* Selection Color */
    }
    display_message(msg, FALSE);
    return TRUE;
}

int
close_display()
{
    if(!was_initted)
      return FALSE;
    start_color(); /* Reset any colors we modified */
    endwin();

    was_initted = 0;
    return TRUE;
}


int
draw_square(int y, int x, int color)
{
	int i;

    attron(COLOR_PAIR(color));
    for (i = 0; i < H; i++)
    {
        mvhline(y+i, x, ACS_FULL, W);
    }
    attroff(COLOR_PAIR(color));

    return TRUE;
}

int draw_piece(board board, int index)
{
	int r, c;
	piece p = board.pieces[index];

  int (*shape)[W];
  if(p.state == ROYAL)
    shape = piece_king;
  else if(p.state == LOWLY)
    shape = piece_basic;
  else
    return 1;

  int color;
  if(p.color == RED)
    color = 5;
  else if(p.color == WHITE)
    color = 6;
  else
    return 1;

  int y_offset = index / 4 * H + OFFSET;
  int x_offset = ((index / 4 + 1)% 2 * W) + (index % 4 * 2 * W) + OFFSET;

  /* Blank out the square */
  draw_square(y_offset, x_offset, 7);

  /* Draw the piece */
  attron(COLOR_PAIR(color));

  /* iterate over the shape */
  for(r = 0; r < H; r++)
  {
    for(c = 0; c < W; c++)
    {
      if(shape[r][c])
      {
        mvaddch(r + y_offset, c + x_offset, ACS_FULL);
      }
    }
  }

  attroff(COLOR_PAIR(color));

  return TRUE;
}

int draw_highlight(int index, int is_blinking)
{
    if (is_blinking)
    {
        attron(A_BLINK);
        attron(COLOR_PAIR(9));
    }
    else
    {
        attron(COLOR_PAIR(8));
    }

    /* Calculate the start positions (one left & up from corner */
    int y_offset = index / 4 * H + OFFSET - 1;
    int x_offset = ((index / 4 + 1) % 2 * W) + (index % 4 * 2 * W ) + OFFSET - 1;

    /* Draw the two horizontal and vertical lines */
    mvhline(y_offset, x_offset, ACS_FULL, W + 1); /* Top horizontal line */
    mvhline(y_offset + H + 1, x_offset, ACS_FULL, W + 1); /* Bottom horizontal line */
    mvvline(y_offset, x_offset, ACS_FULL, H + 2); /* Left vertical line */
    mvvline(y_offset, x_offset + W + 1, ACS_FULL, H + 2); /* Right vertical line */

    if (is_blinking)
    {
        attroff(A_BLINK);
        attroff(COLOR_PAIR(9));
    }
    else
    {
        attroff(COLOR_PAIR(8));
    }

    return TRUE;
}



int draw_board()
{
	int r, c;

		/* Clear out the board */
		attron(COLOR_PAIR(7));
		for(r = OFFSET-1; r < H_MAX + 2 * OFFSET; r++)
			mvhline(r, 0, ACS_FULL, W_MAX + 2*OFFSET);
		attroff(COLOR_PAIR(7));

    /* for each S/2 rows, draw S/2 squares W apart, shifting even rows forward by W */
    for(r = 0; r < S; r++)
    {
        int v_offset = r * H + OFFSET; /* move down r rows of hight H */
        int h_offset = r % 2; /* 0 if even row */
        h_offset = h_offset * W + OFFSET; /* either OFFSET or W+OFFSET for initial offset */
        
        /* For S/2 squares, draw a square, then move the offset forward */
        for(c = 0; c < (S / 2); c++)
        {
            draw_square(v_offset, h_offset, 4); /* Draw white square at location */
            h_offset += 2 * W;
        }

    }

    return TRUE;
}


void display_board(board board) {
	int i;

  draw_board();
  refresh();

	for(i = 0; i < 32; i++) {
		draw_piece(board, i);
	}
	refresh();
}



int display_message(char *msg, int is_urgent, ...)
{
    int top;
    size_t s = strlen(msg);

    if(is_urgent)
    {
        int y = H_MAX / 2;
        
		// Display the message
		draw_font(y, 0, msg, CENTER_X);
        //mvprintw(y, x, msg);
				
		// Wait until user presses a key
        refresh();
		getch();

		// Redraw the board
		va_list args;
		va_start(args, is_urgent);
		board b = va_arg(args, board);
		display_board(b);
    }
    else
    {
        attron(COLOR_PAIR(7));
        for(top = 0; top < OFFSET; top++)
            mvhline(top, 0, ACS_FULL, W_MAX); /* Clear out the row */
        attroff(COLOR_PAIR(7));
        mvprintw(0, (W_MAX / 2 - s / 2), msg);
        refresh();
    }

    return TRUE;
}
