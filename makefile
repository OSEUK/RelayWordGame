CC = gcc
CFLAGS = -Wall -Wextra

all: ser cli

ser: server.o error.o randword.o compare_first_last.o sendBySocket.o playRelayWordGame.o
	$(CC) $(CFLAGS) -o ser server.o error.o randword.o compare_first_last.o sendBySocket.o playRelayWordGame.o

cli: client.o error.o printManual.o
	$(CC) $(CFLAGS) -o cli client.o error.o printManual.o

server.o: server.c 
	$(CC) $(CFLAGS) -c server.c

client.o: client.c 
	$(CC) $(CFLAGS) -c client.c

printManual.o: printManual.c
	$(CC) $(CFLAGS) -c printManual.c

error.o: error.c
	$(CC) $(CFLAGS) -c error.c

randword.o: randword.c
	$(CC) $(CFLAGS) -c randword.c

compare_first_last.o: compare_first_last.c
	$(CC) $(CFLAGS) -c compare_first_last.c

sendBySocket.o: sendBySocket.c
	$(CC) $(CFLAGS) -c sendBySocket.c

playRelayWordGame.o: playRelayWordGame.c
	$(CC) $(CFLAGS) -c playRelayWordGame.c
clean:
	rm -f ser cli *.o
