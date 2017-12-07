G=$(PROJECT_ABS)/src/game/

run:
	$(PROJECT_ABS)/$(OUTFILE)

test_display:
	gcc -o $(PROJECT_ABS)/bin/display -I$(G) TEST_display.c $(G)display.c $(G)board.c -lncurses

test_user_interface:
	gcc -o $(PROJECT_ABS)/bin/user_interface -I$(G) TEST_user_interface.c $(G)user_interface.c $(G)display.c $(G)board.c $(G)move.h -lncurses
