/*============================================================*\
Filename : src/fonts.c
Purpose  : Multiplayer, networked Checkers game for CS451.
Authors  : Corwin Belser <cmb539@drexel.edu>
           Zach Brennan  < zab37@drexel.edu>
           Kris Horsey   < kth37@drexel.edu>
           Zach van Rijn < zwv23@drexel.edu>
License  : MIT/X (excl. ext. libs; see respective licenses).
Revision : 20170707
\*============================================================*/

/*
* Some routines to print strings in ASCII-art style.
*/

/*------------------------------------------------------------*/

#include <stdlib.h>                     /* exit()             */
#include <string.h>                     /* strlen()           */
#include <curses.h>

#include "fonts.h"

/*------------------------------------------------------------*/
/* Available fonts. */

#include "fonts/arcade.h"

#define BLOX            ' '|A_REVERSE   /* block character    */

/*------------------------------------------------------------*/
/* Supporting routines. */

/*
* Rather than compute and draw each row sequentially we can just
* tell ncurses where to draw the next character. The 'render()'
* routine only draws one character at a time, and it will start
* at the position of the cursor. If you want to draw another
* character, you need to either tell it where it should go, or
* in this case, it will shift the cursor to the end of the block
* that it just rendered.
*/

void
render(charset_t type)
{
    int s, t;

    int y, x;
    getyx(stdscr, y, x);                /* current position   */

    /* set current colors (per-character basis) */
    USE_COLR(arcade, type);

    for (s = 0; s < TALL; s++)
    {
        for (t = 0; t < WIDE; t++)
        {
            /* reverse FG/BG space achieves the effect */
            attron (
                COLOR_PAIR(USE_FONT(arcade, type)[s][t] + 1)
            );
            mvaddch(s + y, t + x, BLOX);
            attroff(
                COLOR_PAIR(USE_FONT(arcade, type)[s][t] + 1)
            );
        }
    }
    move(y, x + WIDE + MOVE);
}

/*
* Draws a string, at coordinates (y, x), using whichever font is
* hardcoded into the 'render()' routine.
* TODO: don't hardcode that in.
*/

void
draw_font(int y, int x, char *string, align_t a)
{
    /* compute alignment */
    switch (a)
    {
    case DEFAULT:
        move(y, x);
        break;
    case CENTER_X:
        x += (COLS - (strlen(string) * WIDE)) / 2;
        move(y, x);
        break;
    case CENTER_Y:
        break;
    case CENTER_Z:
        break;
    case ALIGN_LT:
        break;
    case ALIGN_RT:
        break;
    case ALIGN_TP:
        break;
    case ALIGN_BM:
        break;
    default:
        fprintf(stderr, "ERROR: invalid alignment\n");
        exit(1);
        break;
    }

    while (*string)
    {
        switch (*string++)
        {
#if defined(UC) & (UC == 1)
        case 'A': render(uc_a); break;
        case 'B': render(uc_b); break;
        case 'C': render(uc_c); break;
        case 'D': render(uc_d); break;
        case 'E': render(uc_e); break;
        case 'F': render(uc_f); break;
        case 'G': render(uc_g); break;
        case 'H': render(uc_h); break;
        case 'I': render(uc_i); break;
        case 'J': render(uc_j); break;
        case 'K': render(uc_k); break;
        case 'L': render(uc_l); break;
        case 'M': render(uc_m); break;
        case 'N': render(uc_n); break;
        case 'O': render(uc_o); break;
        case 'P': render(uc_p); break;
        case 'Q': render(uc_q); break;
        case 'R': render(uc_r); break;
        case 'S': render(uc_s); break;
        case 'T': render(uc_t); break;
        case 'U': render(uc_u); break;
        case 'V': render(uc_v); break;
        case 'W': render(uc_w); break;
        case 'X': render(uc_x); break;
        case 'Y': render(uc_y); break;
        case 'Z': render(uc_z); break;
#endif
#if defined(LC) & (LC == 1)
        case 'a': render(lc_a); break;
        case 'b': render(lc_b); break;
        case 'c': render(lc_c); break;
        case 'd': render(lc_d); break;
        case 'e': render(lc_e); break;
        case 'f': render(lc_f); break;
        case 'g': render(lc_g); break;
        case 'h': render(lc_h); break;
        case 'i': render(lc_i); break;
        case 'j': render(lc_j); break;
        case 'k': render(lc_k); break;
        case 'l': render(lc_l); break;
        case 'm': render(lc_m); break;
        case 'n': render(lc_n); break;
        case 'o': render(lc_o); break;
        case 'p': render(lc_p); break;
        case 'q': render(lc_q); break;
        case 'r': render(lc_r); break;
        case 's': render(lc_s); break;
        case 't': render(lc_t); break;
        case 'u': render(lc_u); break;
        case 'v': render(lc_v); break;
        case 'w': render(lc_w); break;
        case 'x': render(lc_x); break;
        case 'y': render(lc_y); break;
        case 'z': render(lc_z); break;
#endif
#if defined(SH) & (SH == 1)
        case '!': render(sh_1); break;
        case '@': render(sh_2); break;
        case '#': render(sh_3); break;
        case '$': render(sh_4); break;
        case '%': render(sh_5); break;
        case '^': render(sh_6); break;
        case '&': render(sh_7); break;
        case '*': render(sh_8); break;
        case '(': render(sh_9); break;
        case ')': render(sh_0); break;
#endif
#if defined(EX) & (EX == 1)
        case ',' : render(xa_1); break;
        case '.' : render(xa_2); break;
        case '/' : render(xa_3); break;
        case ';' : render(xa_4); break;
        case '\'': render(xa_5); break;
        case '[' : render(xa_6); break;
        case ']' : render(xa_7); break;
        case '\\': render(xa_8); break;
        case '-' : render(xa_9); break;
        case '=' : render(xa_0); break;

        case '<' : render(xb_1); break;
        case '>' : render(xb_2); break;
        case '?' : render(xb_3); break;
        case ':' : render(xb_4); break;
        case '"' : render(xb_5); break;
        case '{' : render(xb_6); break;
        case '}' : render(xb_7); break;
        case '|' : render(xb_8); break;
        case '_' : render(xb_9); break;
        case '+' : render(xb_0); break;
#endif
		case ' ' : move(y, x + WIDE); break;
        case '\n': move(y + TALL, x); break;
        }
        x += WIDE;
    }
}
