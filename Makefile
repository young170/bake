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
EXE = bake

# List all source files
SRC = $(wildcard $(SRCDIR)/*.c)

# Create a list of corresponding object files
OBJ = $(SRC:$(SRCDIR)/%.c=$(SRCDIR)/%.o)

# rule for link
$(EXE): $(OBJ)
	$(CC) -o $@ $^

# rule for compilation
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< -I$(INCDIR)

.PHONY: clean

clean:
	rm -f $(SRCDIR)/*.o $(EXE)
