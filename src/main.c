/*============================================================*\
Filename : src/main.c
Purpose  : Multiplayer, networked Checkers game for CS451.
Authors  : Corwin Belser <cmb539@drexel.edu>
           Zach Brennan  < zab37@drexel.edu>
           Kris Horsey   < kth37@drexel.edu>
           Zach van Rijn < zwv23@drexel.edu>
License  : MIT/X (excl. ext. libs; see respective licenses).
Revision : 20170812
\*============================================================*/

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

#define _BSD_SOURCE                     /* enable usleep()    */

/*------------------------------------------------------------*/

/*
* Standard headers.
*/

#include <stdio.h>
#include <stdlib.h>                     /* calloc()           */
#include <signal.h>                     /* signal()           */
#include <unistd.h>                     /* sleep()            */

/*
* Curses.
* http://invisible-island.net/ncurses/man/menu.3x.html
*/

#include <ncurses.h>
#include <menu.h>

/*
* Internal.
*/

#include <game/display.h>
#include <game/game_manager.h>
#include <network/client.h>
#include <network/server.h>
#include <fonts.h>

/*------------------------------------------------------------*/

#define ARRAY_SIZE(a)   (sizeof(a) / sizeof(a[0]))

char *opts[] =
{
    "Multiplayer (alternate turns)",
    "Multiplayer (play over network)",
    "Create Server",
    "Options",
    "Exit",
};

void process_menu_selection(char *name);

int display_menu(WINDOW * w)
{
		werase(w);
    ITEM **my_items;
    int c;
    MENU *my_menu;
    int n_choices;
    int i;

    /* Initialize items */
    n_choices = ARRAY_SIZE(opts);
    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for (i = 0; i < n_choices; ++i)
    {
        my_items[i] = new_item(opts[i], opts[i]);
        /* Set the user pointer */
        //set_item_userptr(my_items[i], process_menu_selection);
    }
    my_items[n_choices] = (ITEM *)NULL;

    /* Create menu */
    my_menu = new_menu((ITEM **)my_items);

		/* Set the menu's window */
		set_menu_win(my_menu, w);
		set_menu_sub(my_menu, derwin(w, ARRAY_SIZE(opts) * 2, 32, 2, 2));

    /* Post the menu */
    mvwprintw(w, LINES - 3, 0, "Press <ENTER> to see the option selected");
    mvwprintw(w, LINES - 2, 0, "Up and Down arrow keys to naviage (F1 to Exit)");
    post_menu(my_menu);
    wrefresh(w);

		keypad(w, TRUE);

    while ((c = wgetch(w)) != 10) /* Enter Key */
    {
        switch (c)
        {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case 10: /* Enter */
        {	ITEM *cur;
        void(*p)(char *);

        cur = current_item(my_menu);
        p = item_userptr(cur);
        p((char *)item_name(cur));
        pos_menu_cursor(my_menu);
        break;
        }
        break;
        }
				wrefresh(w);
    }

		ITEM * cur = current_item(my_menu);
		int index = item_index(cur);

    unpost_menu(my_menu);
    for (i = 0; i < n_choices; ++i)
        free_item(my_items[i]);
    free_menu(my_menu);
    keypad(stdscr, TRUE);
		return index;
}

void process_menu_selection(char *name)
{
    printw("Item selected is : %s", name);
}

/*------------------------------------------------------------*/
/*
* Catches changes to window size, can trigger re-render.
*/

void
catch_refresh(int sig)
{
    signal(SIGWINCH, SIG_IGN);

    endwin();
    initscr();
    refresh();

    signal(SIGWINCH, catch_refresh);
}


int
main(void)
{
    FILE *fp = fopen("move_list.txt", "a+");
    fprintf(fp, "\n\nBeginning a new game!\n");
    fclose(fp);
    int rows;
    //signal(SIGWINCH, catch_refresh);

		init_display(); /* initialize ncurses */
		
    for (rows = LINES - 1; rows > 0; rows--)
    {
        clear();
        draw_font(rows, 0, "CHECKERS\n", CENTER_X);
        refresh();
				usleep(100000);
    }


	/**********************************************************/
	/* menu */
    int y, x;
	(void)x; /* unused */
    getyx(stdscr, y, x);
    int pads = 4;

	WINDOW *w = newwin(ARRAY_SIZE(opts) * 3, 40, y + pads, COLS / 2 - 31 / 2);
 	box(w, 0, 0);

    int choice = 0;
	while(choice != -1)
    {
        choice = display_menu(w);
		switch(choice)
        {
			case 0:
				single_player_game();
			break;
			case 1:
                clear();
                refresh();
		        mvwprintw(w, 0, 0, "Connecting to the server...");

                //This is to connect to a server on the same computer
                start_client("73.112.153.22"); 
				wrefresh(w);
			break;
            case 2:
                clear();
                refresh();
			    mvwprintw(w, 0, 0, "Creating the server...");
                start_server();
                refresh();
            break;
		    case 3:
		        mvwprintw(w, 0, 0, "Gurl we 'aint got those");
				wrefresh(w);
				getch();
			break;
			default:
				choice = -1;
			break;
		}
        clear();
        draw_font(1, 0, "CHECKERS\n", CENTER_X);
        refresh();
	}
    delwin(w);
    close_display();
    return 0;
}
