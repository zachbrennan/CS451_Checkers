#---------------------------------------------------------------
# Docker configuration.

# IMAGES - some image name with (optional) tag
# BUILDS - can include 'sudo' if non-root or docker group member
# RUNNER - optionally append '--rm' to destroy intermediates
# MOUNTS - **should match the WORKDIR from Dockerfile**
# SHELLS - which existing shell to use during 'envir' target run

IMAGES := alpine:checkers
BUILDS := sudo docker build
RUNNER := sudo docker run --rm
MOUNTS := /home
SHELLS := "/bin/bash"

#---------------------------------------------------------------
# Local, container-related targets.

image:
	$(Q)$(BUILDS) -f Dockerfile . -t $(IMAGES)

envir:
	$(Q)$(RUNNER) \
	-v $(PROJECT_ABS):$(MOUNTS) \
	-it $(IMAGES) \
	$(SHELLS)

#---------------------------------------------------------------
# In-container operations.

# game.
# In this case, $CC is the default inside container. Can be
# configured within the Dockerfile, if desired.

# FLAG - give linker options (please use static and PIC)
# LIBS - required link line to build the project

FLAG := -static -fPIC -flto -g -Wall
LIBS := -lncurses -lmenu -lc -lcheck -lgcov

game:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(CC) $(FLAG) \
		-o $(MOUNTS)/$(OUTFILE) \
		-I$(SRC) \
		$(SRC)/game/game_manager.c \
		$(SRC)/game/game_logic.c \
		$(SRC)/game/user_interface.c \
		$(SRC)/game/display.c \
		$(SRC)/game/board.c \
		$(SRC)/network/client.c \
		$(SRC)/network/server.c \
		$(SRC)/game/move.h \
		$(SRCFILE) \
		$(LIBS)

gcov:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(CC) $(FLAG) \
			-fprofile-arcs \
			-ftest-coverage \
			-fprofile-dir=$(PROJECT_ABS)/data \
			-fprofile-generate=$(PROJECT_ABS)/data \
		-o $(MOUNTS)/$(OUTFILE) \
		-I$(SRC) \
		$(SRC)/game/game_manager.c \
		$(SRC)/game/game_logic.c \
		$(SRC)/game/user_interface.c \
		$(SRC)/game/display.c \
		$(SRC)/game/board.c \
		$(SRC)/network/client.c \
		$(SRC)/network/server.c \
		$(SRC)/game/move.h \
		$(SRCFILE) \
		$(LIBS)

network:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(CC) $(FLAG) -D TEST \
		-o $(MOUNTS)/$(BIN)/client \
		-I$(SRC) \
		$(SRC)/game/board.c \
		$(SRC)/game/game_logic.c \
		$(SRC)/game/user_interface.c \
		$(SRC)/game/display.c \
		$(SRC)/game/game_manager.c \
		$(NET)/client.c \
		$(LIBS)
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(CC) $(FLAG) -D TEST \
		-o $(MOUNTS)/$(BIN)/server \
		-I$(SRC) \
		$(SRC)/game/game_logic.c \
		$(SRC)/game/user_interface.c \
		$(SRC)/game/board.c \
		$(SRC)/game/display.c \
		$(SRC)/game/game_manager.c \
		$(NET)/server.c \
		$(LIBS)

logic:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(CC) $(FLAG) \
		-o $(MOUNTS)/$(BIN)/logic \
		-I$(SRC) \
		$(SRC)/game/game_logic.c \
		$(LIBS)

display:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(CC) $(FLAG) \
		-o $(MOUNTS)/$(BIN)/display \
		-I$(SRC) \
		$(SRC)/game/board.c \
		$(SRC)/game/display.c \
		$(SRC)/test/TEST_display.c \
		$(LIBS)

user_interface:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(CC) $(FLAG) \
		-o $(MOUNTS)/$(BIN)/user_interface \
		-I$(SRC) \
		$(SRC)/game/board.c \
		$(SRC)/game/display.c \
		$(SRC)/game/user_interface.c \
		$(SRC)/test/TEST_user_interface.c \
		$(LIBS)


# cppcheck.
# Works as intended.

cppcheck:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		cppcheck --std=c89 --enable=all --check-config \
			-I$(SRC) \
			-I/usr/include \
			-I/usr/local/include \
			$(SRC)

unittest:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(CC) $(FLAG) \
		-I$(SRC) \
		-I/usr/include \
		-I/usr/local/include \
		-o $(MOUNTS)/$(BIN)/tests \
		./tests/tests.c \
		$(LIBS)


# valgrind.
# Due to the way Docker allows you to pass arguments / commands,
# there is some funky 'timeout N' foo here because we can't pipe
# /dev/null (even escaping \</dev/null) to make the executable
# quit on launch. Note: this will not be a problem with the main
# (final) executable; it may simply print the '--help' menu.

valgrind:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		timeout 5 valgrind --leak-check=full \
			$(OUTFILE)

# run.
# Simulated 'run-con' from parent Makefile.

run:
	$(Q)$(RUNNER) \
		-v $(PROJECT_ABS):$(MOUNTS) \
		-it $(IMAGES) \
		$(MOUNTS)/$(OUTFILE)
