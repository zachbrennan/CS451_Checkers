################################################################
#------------------------ DOCUMENTATION -----------------------#
################################################################

#---------------------------------------------------------------
# File information.

# Filename : Makefile
# Purpose  : Multiplayer, networked Checkers game for CS451.
# Authors  : Corwin Belser <cmb539@drexel.edu>
#            Zach Brennan  < zab37@drexel.edu>
#            Kris Horsey   < kth37@drexel.edu>
#            Zach van Rijn < zwv23@drexel.edu>
# License  : MIT/X (excl. ext. libs; see respective licenses).
# Revision : 20170811

################################################################
#------------------------ CONFIGURATION -----------------------#
################################################################

DEBUG := 0

#---------------------------------------------------------------
# Build directories.

# Used by scripts to establish base directory.
export PROJECT_ABS := $(shell pwd)
export PROJECT_REL := .

################################################################
#------------------------- ENVIRONMENT ------------------------#
################################################################

export SRC := $(PROJECT_REL)/src
export TST := $(SRC)/test
export NET := $(SRC)/network
export BIN := $(PROJECT_REL)/bin
export DOC := $(PROJECT_REL)/docs
export DKR := $(PROJECT_REL)/docker

export MAIN := $(shell find $(SRC) -maxdepth 1 -type f -name "*.c")
#export MAIN := fonts.c

export SRCFILE := $(MAIN)
export OUTFILE := $(BIN)/game

################################################################
#------------------------ BUILD TARGETS -----------------------#
################################################################

# These are all .PHONY targets because we will be switching to a
# CMake-based build system and for now, this makes it easier to
# test things quickly.

.PHONY: usage \
	image \
	envir \
	clean \
	check \
	game \
	run-loc \
	run-con

# When we are in debug mode, every statement within each recipe
# is printed. Otherwise, we will suppress all commands executed
# (though if the command produces output, it will be printed).

ifeq ($(DEBUG), 1)
Q := 
else
Q := @
endif

#---------------------------------------------------------------
# usage.

# Prints usage text. Please keep this up-to-date if new targets
# are added. Using multiple 'printf' statements to simplify the
# editing process (not worry about '\').

usage:
	$(Q)printf "Usage:\n\n  make <target>\n\n"
	$(Q)printf "Targets:\n\n"
	$(Q)printf "  usage     \tprints this text then exits  \n"
	$(Q)printf "  image     \tbuilds the devel. Docker img.\n"
	$(Q)printf "  envir     \tspawns/enters devel. envir.  \n"
	$(Q)printf "  clean     \tremove intermediate files    \n"
	$(Q)printf "  check     \truns   various tests on code \n"
	$(Q)printf "  game      \tbuilds the Checkers(tm) game \n"
	$(Q)printf "  run-loc   \truns   the game on this mach.\n"
	$(Q)printf "  run-con   \truns   the game in Container \n"
	$(Q)printf "\n"

#---------------------------------------------------------------
# LOCAL targets -- these run on your local machine.

# image.
# Builds the 'alpine:checkers' Docker image.

image: $(DKR)/Makefile
	$(Q)test ! -e $(PROJECT_ABS)/.have_$@
	$(Q)make -C $(DKR) image
	$(Q)touch $(PROJECT_ABS)/.have_$@

# envir.
# Drops into 'alpine:checkers' shell (for testing/debugging).

envir: $(DKR)/Makefile
	$(Q)make -C $(DKR) envir

# clean.
# Removes intermediate executable files. Useful for making sure
# that the executables you test are current.

clean:
	$(Q)rm -fr $(BIN)
	$(Q)rm -fr ./data ./*.txt
	$(Q)rm -f  $(shell find . -type f \( -iname \*.aux -o -iname \*.hst -o -iname \*.log -o -iname \*.synctex.gz -o -iname \*.toc -o -iname \*.ver \))
	$(Q)rm -f  $(shell find . -type f -wholename "*.have_*")
	$(Q)rm -f  $(shell find . -type f -wholename "*.gcno")
	
# run-loc.
# Executes the checkers game on the local machine. Note: make
# sure the architecture matches!

run-loc: $(SRC)/Makefile
	$(Q)$(OUTFILE)

#---------------------------------------------------------------
# CONTAINER targets -- these run inside the Docker container.

# check.
# Runs various testing utilities against the source code and the
# compiled executable(s).

check: game
	$(Q)mkdir -p $(BIN)
	$(Q)make -C $(DKR) cppcheck
	$(Q)make -C $(DKR) unittest
	$(Q)make -C $(DKR) valgrind

cover:
	$(Q)mkdir -p $(BIN)
	$(Q)make -C $(DKR) gcov
	$(Q)$(OUTFILE)
#	$(Q)touch $(shell find . -type f -name "*.gcno" | sed 's@.gcno@.gcda@')

# game.
# Builds the checkers game from source.

game: $(SRC)/Makefile
	$(Q)mkdir -p $(BIN)
	$(Q)make -C $(DKR) game

# run-con.
# Executes the checkers game inside the container. Note: worst-
# case scenario, we require the user to run the game inside of
# Docker containers. Shouldn't be the case, though, since we are
# building static executable(s).

run-con: $(SRC)/Makefile
	$(Q)make -C $(DKR) run

#---------------------------------------------------------------
# Testing.

test_logic: $(DKR)/Makefile
	$(Q)make -C $(DKR) logic

test_display: $(TST)/Makefile
	$(Q)make -C $(DKR) display

test_user_interface: $(TST)/Makefile
	$(Q)make -C $(DKR) user_interface

test_network: $(NET)/Makefile
	$(Q)make -C $(DKR) network
