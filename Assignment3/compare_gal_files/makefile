CC = gcc
LD = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -lm
RM = /bin/rm -f
OBJS = compare_gal_files.o file_operations.o
EXECUTABLE = compare

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

file_operations.o: ../file_operations/file_operations.h ../file_operations/file_operations.c
	$(CC) $(CFLAGS) -c ../file_operations/file_operations.c

compare_gal_files.o: compare_gal_files.c ../file_operations/file_operations.h
	$(CC) $(CFLAGS) -c compare_gal_files.c -std=c99

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
