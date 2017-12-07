#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ncurses.h>
#include <pthread.h>
#include <game/board.h>
#include <game/move.h>
#include <game/game_manager.h>
#include <network/client.h>

/*--------------------------------------------------------------------*/
/*
board receive_board(int sockServer);
piece receive_piece(int sockServer);
int connect_to_server(char *ipAddress);
int start_client();
void send_move_sequence(int sockServer, mseq ms);
void send_move(int sockServer, moove m);
*/
/*--------------------------------------------------------------------*/

/*
* Slightly more sane way of maintaining an errors-message list. It will
* allow the compiler to warn if something is used incorrectly.
*/

typedef
enum MessageType
{
    RECV_ERR,
    S_CR_ERR,
    S_CO_ERR,
    S_CO_SUC,
} Message;


static const char *
MSG(Message e)
{
    static const char *warnings[] =
    {
        /* RECV_ERR */ "Disconnected from server."                     ,
        /* S_CR_ERR */ "Could not create socket."                      ,
        /* S_CO_ERR */ "Connection to server failed."                  ,
        /* S_CO_SUC */ "Connection to server successful."              ,
    };

    return warnings[e];
}

/*--------------------------------------------------------------------*/

static const int MAXSTRLEN = 200;  /* Max length of a string */
static const int PORT = 8888; /* Port for client to connect to */

/*--------------------------------------------------------------------*/

#ifdef TEST
int main(int argc, char* argv[])
{
    return 0;
}
#endif

int start_client(char *ipAddress)
{
    int sockConnect;
    char reply[MAXSTRLEN];

    sockConnect = connect_to_server(ipAddress);
    if (-1 == sockConnect)
    {
        return 1;
    }

    /* Receives initial connection message from server  */
    int read_size = recv(sockConnect, reply, MAXSTRLEN, 0);
    if (read_size <= 0)
    {
		printf("%s\n", MSG(RECV_ERR));
        return 1;
    }
    reply[read_size] = '\0';
    printf("%s\n", reply);

    /* Get PlayerID*/
    int playerID;
    recv(sockConnect, &playerID, sizeof(playerID), 0);

    /* Receive initial board*/
    board b = receive_board(sockConnect);
    display_board(b);
    /* Multiplayer game loop */
    while (1)
    {
        if (-1 == b.playerTurn)
        {
            //Win by capturing all pieces
            if (has_winner(b) == 0)
            {   
                display_board(b);
                display_message("RED WINS", 1, b);
                break;
            }
            else if (has_winner(b) == 1)
            {               
                display_board(b);
                display_message("WHITE WINS", 1, b);
                break;
            }
        }
        else if (-2 == b.playerTurn)
        {            
               display_board(b);
               display_message("RED WINS", 1, b);
               break;
        }
        else if (-3 == b.playerTurn)       
        {
                display_board(b);
                display_message("WHITE WINS", 1, b);
                break;
        }
        else if (playerID == b.playerTurn)
        {   
            display_message("Make your move.", 0);
            mseq ms = take_turn(b, b.playerTurn);        
            send_move_sequence(sockConnect, ms);
            b = receive_board(sockConnect);
            
            /* Take Turn */
            //printf("It's my turn!\n");
        }
        else
        {   
            display_message("Waiting for the other player to make their move.", 0);
            b = receive_board(sockConnect);
            /* Not my turn, wait for board to be sent again */
            //printf("It is Player %ds turn.\n", b.playerTurn);
        }
    }

    free_board(b);
    close(sockConnect);
    return 0;
}

/*--------------------------------------------------------------------*/

int connect_to_server(char *ipAddress)
{
    int sockConnect;
    struct sockaddr_in server;

    sockConnect = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockConnect)
    {
        printf("%s\n", MSG(S_CR_ERR));
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(ipAddress);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sockConnect, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("%s\n", MSG(S_CO_ERR));
        return -1;
    }

    printf("%s\n", MSG(S_CO_SUC));
    return sockConnect;
}

/*--------------------------------------------------------------------*/

board receive_board(int sockServer)
{
    int i;
    board b;

    b = init_board();
    recv(sockServer, &b.playerTurn, sizeof(b.playerTurn), 0);
    for (i = 0; i < 32; i++)
    {
        b.pieces[i] = receive_piece(sockServer);
    }
    return b;
}

/*--------------------------------------------------------------------*/

piece receive_piece(int sockServer)
{
    piece p;
    recv(sockServer, &p.state, sizeof(p.state), 0);
    recv(sockServer, &p.color, sizeof(p.color), 0);

    return p;
}

/*--------------------------------------------------------------------*/

void send_move_sequence(int sockServer, mseq ms)
{
    send(sockServer, &ms.length, sizeof(ms.length), 0);
    for (int i = 0; i < ms.length; i++)
    {
        send_move(sockServer, ms.moves[i]);
    }
}

/*--------------------------------------------------------------------*/

void send_move(int sockServer, moove m)
{
    send(sockServer, &m.from, sizeof(m.from), 0);
    send(sockServer, &m.dest, sizeof(m.dest), 0);
}

/*--------------------------------------------------------------------*/
