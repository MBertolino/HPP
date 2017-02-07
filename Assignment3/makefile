CC = gcc
LD = gcc
CFLAGS = -Wall -Werror
LDFLAGS = 
RM = /bin/rm -f
OBJS = galsim.o file_operations.o
EXECUTABLE = galsim

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE)

file_operations.o: file_operations/file_operations.h file_operations/file_operations.c
	$(CC) $(CFLAGS) -c file_operations/file_operations.c

galsim.o: galsim.c file_operations/file_operations.h
	$(CC) $(CFLAGS) -c galsim.c -std=c99

clean:
	$(RM) $(EXECUTABLE) $(OBJS) makefile~ galsim.c~