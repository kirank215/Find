CC = gcc
CFALGS = -Wextra -Wall -Werror -std=c99 -pedantic -g
VPATH = . src
SRC = extras.c myfind1.c
OBJ = extras.o myfind1.o

all: myfind

myfind: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ) myfind 
