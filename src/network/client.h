#ifndef _SRC_NETWORK_CLIENT_H_
#define _SRC_NETWORK_CLIENT_H_

board receive_board(int sockServer);
piece receive_piece(int sockServer);
int connect_to_server(char *ipAddress);
int start_client();
void send_move_sequence(int sockServer, mseq ms);
void send_move(int sockServer, moove m);

#endif /* _SRC_NETWORK_CLIENT_H_ */
