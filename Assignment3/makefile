CC = gcc
LD = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -L/opt/X11/lib -lX11 -lm -O3
INCLUDES = -I/opt/X11/include
RM = /bin/rm -f
OBJS = galsim.o file_operations.o functions.o graphics.o
EXECUTABLE = galsim

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) -o $(EXECUTABLE) $(OBJS) $(LDFLAGS)

file_operations.o: file_operations/file_operations.h file_operations/file_operations.c
	$(CC) $(CFLAGS) -c file_operations/file_operations.c

functions.o: functions.h functions.c
	$(CC) $(CFLAGS) -c functions.c -std=c99
	
graphics.o: graphics/graphics.c graphics/graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

galsim.o: galsim.c file_operations/file_operations.h graphics/graphics.h functions.h
	$(CC) $(CFLAGS) $(INCLUDES) -c galsim.c -std=c99

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
