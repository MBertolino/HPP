CC = gcc
LD = gcc
CFLAGS = -Wall -Werror -Ofast
LDFLAGS = -L/opt/X11/lib -lX11 -lm
INCLUDES = -I/opt/X11/include
RM = /bin/rm -f
OBJS = galsim.o file_operations.o graphics.o
EXECUTABLE = galsim

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) -o $(EXECUTABLE) $(OBJS) $(LDFLAGS)

file_operations.o: file_operations/file_operations.h file_operations/file_operations.c
	$(CC) $(CFLAGS) -c file_operations/file_operations.c
	
graphics.o: graphics/graphics.c graphics/graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

galsim.o: galsim.c file_operations/file_operations.h graphics/graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c galsim.c -std=c99

clean:
	$(RM) $(EXECUTABLE) *.o
