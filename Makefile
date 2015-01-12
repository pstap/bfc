CC = gcc
DEBUG = -ggdb
CFLAGS = -Wall
EXENAME = bfc

bfc : main.c
	$(CC) $(CFLAGS) $(DEBUG) -o $(EXENAME) main.c
