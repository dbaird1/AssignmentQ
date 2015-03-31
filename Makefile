PROJECT = AssignmentQ
TARGET = AssignmentQ


OBJ_FILES = \
  main.o

CC = gcc
CFLAGS = -g -w -std=gnu99

$(TARGET):  	$(OBJ_FILES)
		$(CC) $(OBJ_FILES) -o $@ -lpthread -lm -lrt



main.o:		main.c
