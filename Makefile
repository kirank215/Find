CC = gcc
CFLAGS = -Wextra -Wall -Werror -std=c99 -pedantic -g
VPATH = . src
SRC = extras.c operations.c parse.c myfind1.c
OBJ = extras.o operations.o parse.o myfind1.o

all: myfind

myfind: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ) myfind 
