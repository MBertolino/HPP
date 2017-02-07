CC = gcc
LD = gcc
CFLAGS = -Wall
LDFLAGS = 
RM = /bin/rm -f
OBJS = triang.o
PASC = triang
all:$(PASC)

$(PASC): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PASC)

triang.o: triang.c
	$(CC) $(CFLAGS) -c triang.c

clean:
	$(RM) $(PASC) $(OBJS)

