#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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
#include <game/game_logic.h>
#include <network/server.h>

/*--------------------------------------------------------------------*/

void  *game_thread(void *param);
void  send_board(int sockPlayer, board b, int playerTurn);
void  send_piece(int sockPlayer, piece p);
mseq  receive_move_sequence(int sockPlayer);
moove receive_move(int sockPlayer);
int   create_server();

/*--------------------------------------------------------------------*/

/*
* Slightly more sane way of maintaining an errors-message list. It will
* allow the compiler to warn if something is used incorrectly.
*/

typedef
enum MessageType
{
    NEW_SPEC,
    P1CONMSG,
    P2CONMSG,
    WAIT4CON,
    MAXCONNS,
    SERV_RUN,
    BIND_ERR,
    BIND_SUC,
    OPEN_ERR,
    CON_FAIL,
    TH_C_ERR,
    TH_C_SUC,
} Message;


static const char *
MSG(Message e)
{
    static const char *warnings[] =
    {
        /* NEW_SPEC */ "Adding new connection as a spectator...",
        /* P1CONMSG */ "You are player 1. Waiting for another player to connect.",
        /* P2CONMSG */ "You are player 2. Creating game now."  ,
        /* WAIT4CON */ "Waiting for player connections."       ,
        /* MAXCONNS */ "The maximum amount of connections has been reach. Try again later.",
        /* SERV_RUN */ "Server up and running."                ,
        /* BIND_ERR */ "Error binding socket."                 ,
        /* BIND_SUC */ "Successfully bound to port "           ,
        /* OPEN_ERR */ "Error opening socket."                 ,
        /* CON_FAIL */ "Accept failed."                        ,
        /* TH_C_ERR */ "Thread was not created successfully."  ,
        /* TH_C_SUC */ "Game thread has started."              ,
    };

    return warnings[e];
}

/*--------------------------------------------------------------------*/

static const int  PORT = 8888; //Port to connect to on server
static const int  MAXSTRLEN = 200;  //Max length of strings being sent
static const int  MAXCONNECT = 5;    //Max amount of client connections

/*--------------------------------------------------------------------*/

#ifdef TEST
int main(int argc, char *argv[])
{
    return 0;
}
#endif

//int main(int argc, char *argv[])
int start_server()
{
    struct sockaddr_in cAddr;       /* Client address */
    int sockConnect;
    int sockClient;
    int sockList[MAXCONNECT + 1];
    unsigned int addrLength;                 /* Length of address */
    int playerCount;                /* Number of players connected */
    int playerID;
    char message[MAXSTRLEN];
    pthread_t thread;

    addrLength = sizeof(cAddr);
    playerCount = 0;

    sockConnect = create_server();
    if (-1 == sockConnect)
    {
        return 1;
    }

    while (1)
    {
        if (0 > (sockClient = accept(sockConnect, (struct sockaddr *)&cAddr, &addrLength)))
        {
            printf("%s\n", MSG(CON_FAIL));
            return -1;
        }
        else
        {
            playerCount += 1;
            if (playerCount > MAXCONNECT)
            {
                strncpy(message, MSG(MAXCONNS), MAXSTRLEN);
                send(sockClient, message, strlen(message), 0);
                close(sockClient);

            }
            else if (1 == playerCount)
            {
                sockList[0] = sockClient;
                send(sockList[0], MSG(P1CONMSG), MAXSTRLEN, 0);
                playerID = playerCount - 1;
                send(sockList[0], &playerID, sizeof(playerCount), 0);
            }
            else if (2 == playerCount)
            {
                sockList[1] = sockClient;
                send(sockList[1], MSG(P2CONMSG), MAXSTRLEN, 0);
                playerID = playerCount - 1;
                send(sockList[1], &playerID, sizeof(playerCount), 0);

                if (pthread_create(&thread, NULL, game_thread, (void *)sockList))
                {
                    printf("%s\n", MSG(TH_C_ERR));
                    return -1;
                }
                playerCount = 0;
            }
            /*
            else
            {
                printf("%s\n", MSG(NEW_SPEC));
                sockList[playerCount - 1] = sockClient;
            }
            */
        }
    }
    return 0;
}

/*--------------------------------------------------------------------*/

int create_server()
{
    struct sockaddr_in addr;

    int sockConnect;
    int iSetOption;            /* For SO_REUSEADDR, if program quits without freeing socket */

    iSetOption = 1;

    sockConnect = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockConnect)
    {
        printf("%s\n", MSG(OPEN_ERR));
        return -1;
    }
    setsockopt(sockConnect, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = 0;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    if (-1 == bind(sockConnect, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
    {
        printf("%s\n", MSG(BIND_ERR));
        return -1;
    }
    else
    {
        printf("%s%u\n", MSG(BIND_SUC), PORT);
    }

    listen(sockConnect, 3);
    printf("%s%s\n", MSG(SERV_RUN), MSG(WAIT4CON));
    
    return sockConnect;
}

/*--------------------------------------------------------------------*/

void *game_thread(void *param)
{
    printf("%s\n", MSG(TH_C_SUC));

    int *sockList = (int *)param;
    board b;

    b = init_board();

    /* Send initial boardstate */
    send_board(sockList[0], b, 0);
    send_board(sockList[1], b, 0);

    /* Multiplayer game loop */
    while (1)
    {
        //Get list of moves, and apply them
        mseq ms =  receive_move_sequence(sockList[b.playerTurn]); 
        for (int i = 0; i < ms.length; i++)
        {
            make_move(b,b.playerTurn, ms.moves[i]);
        }
    
        //Check if there is a winner 
        int winner = has_winner(b); 
        if (winner != -1)
        {
            send_board(sockList[0], b, -1);     
            send_board(sockList[1], b, -1);     
            break;
        }
        
        //Check if someone has no moves to make
        if (ms.length == 0)
        {
            if (b.playerTurn == 0)
            {
                send_board(sockList[0], b, -2);     
            }
            else if (b.playerTurn == -1)
            {
            
                send_board(sockList[1], b, -3);     
            }
            break;
        }
        

        //Swap whose turn it is
        b.playerTurn = 1 - b.playerTurn;
        
        //sends board to clients
        send_board(sockList[0], b, b.playerTurn);
        send_board(sockList[1], b, b.playerTurn);
           
    }
    puts("Game thread ended.");
    return 0;
}

/*--------------------------------------------------------------------*/

void send_board(int sockPlayer, board b, int playerTurn)
{
    send(sockPlayer, &playerTurn, sizeof(playerTurn), 0);
    for (int i = 0; i < 32; i++)
    {
        send_piece(sockPlayer, b.pieces[i]);
    }
}

/*--------------------------------------------------------------------*/

void send_piece(int sockPlayer, piece p)
{
    send(sockPlayer, &p.state, sizeof(p.state), 0);
    send(sockPlayer, &p.color, sizeof(p.color), 0);
}

/*--------------------------------------------------------------------*/

void send_message(int sockPlayer, char *message, bool isActive)
{
    /* Somehow differentiate them when sending (add flag at beginning/end?) */
    if (isActive)
    {
        send(sockPlayer, message, strlen(message), 0);
    }
    else
    {
        send(sockPlayer, message, strlen(message), 0);
    }
}

/*--------------------------------------------------------------------*/

mseq receive_move_sequence(int sockPlayer)
{
    mseq ms;

    recv(sockPlayer, &ms.length, sizeof(ms.length), 0);
    for (int i = 0; i < ms.length; i++)
    {
        ms.moves[i] = receive_move(sockPlayer);
    }

    return ms;
}

/*--------------------------------------------------------------------*/

moove receive_move(int sockPlayer)
{
    moove m;

    recv(sockPlayer, &m.from, sizeof(m.from), 0);
    recv(sockPlayer, &m.dest, sizeof(m.dest), 0);

    return m;
}

/*--------------------------------------------------------------------*/
