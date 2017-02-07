CC = gcc
LD = gcc
CFLAGS = -Wall -Werror
LDFLAGS = 
RM = /bin/rm -f
OBJS = main.o file_operations.o
EXECUTABLE = galsim

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE)

file_operations.o: file_operations/file_operations.h file_operations/file_operations.c
	$(CC) $(CFLAGS) -c file_operations/file_operations.c

main.o: main.c file_operations/file_operations.h
	$(CC) $(CFLAGS) -c main.c

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
