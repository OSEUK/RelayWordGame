CC = gcc
CFLAGS = -Wall -Wextra

all: ser cli

ser: server.c error.c randword.c randword.h error.h
	$(CC) $(CFLAGS) -o ser server.c error.c randword.c

cli: client.c error.c error.h
	$(CC) $(CFLAGS) -o cli client.c error.c

clean:
	rm -f ser cli
