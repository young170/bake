# Makefile for bake
#
# $@ The name of the EXEC file (the one before the colon)
# $< The name of the first (or only) prerequisite file (the first one after the colon)
# $^ The names of all the prerequisite files (space separated)
# $* The stem (the bit which matches the % wildcard in the rule definition.
#
CC = gcc -std=c99 -Werror -Wall -pedantic
INCDIR = ./include
SRCDIR = ./src

SRC = $(SRCDIR)/rushhour.c
OBJ = $(SRC:.c=.o)
EXE = bake

# rule for link
all: $(EXE)
$(EXE): $(SRCDIR)/%.o
	$(CC) -o $@ $^

# rule for compilation
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< -I$(INCDIR)

.PHONY: clean

clean:
	rm -f $(SRCDIR)/*.o $(EXE)