G=$(PROJECT_ABS)/src/game/
C=../game/

run:
	@gcc -pthread -I$(C) $(C)board.c server.c -o server 
	@gcc -pthread -I$(C) $(C)board.c client.c -o client

compile_client:
	gcc -pthread -I$(G) -I$(SRC) $(G)board.c client.c -o client

compile_server:
	gcc -pthread -I$(G) -I$(SRC) $(G)board.c server.c -o server 


