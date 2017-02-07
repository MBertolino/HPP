CC = gcc
LD = gcc
CFLAGS = -Wall
LDFLAGS = 
RM = /bin/rm -f
OBJS = temp.o
TEMP = temp
all:$(TEMP)

$(TEMP): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TEMP)

temp.o: temp.c
	$(CC) $(CFLAGS) -c temp.c

clean:
	$(RM) $(TEMP) $(OBJS)

