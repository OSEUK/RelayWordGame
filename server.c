#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define PORTNUM 9000

void err_handling(char *error_message){
	perror(error_message);
	exit(1);
}
char* get_random_word() {
	char *start_words[] = {"hello", "unix", "programming"};
	int num_words = sizeof(start_words) / sizeof(start_words[0]);
	
	srand((unsigned)time(NULL));

	int random_index = rand() % num_words;
	return start_words[random_index];
}
int main(void) {
	char *word;
	char* start_word = get_random_word();
	struct sockaddr_in sin, cli;	
	int sv_sock, cli_sock[2], clientlen = sizeof(cli);
	
	if ((sv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		err_handling("socket");

	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("172.19.230.129");
	
	unlink("172.19.230.129");
	if (bind(sv_sock, (struct sockaddr *)&sin, sizeof(sin))) 
		err_handling("bind");

	if (listen(sv_sock, 2)) 
		err_handling("listen");

	if ((cli_sock[0] = accept(sv_sock, (struct sockaddr *)&cli, &clientlen)) == -1) 
		err_handling("accept");
	printf("Player 1 connected\n");

	if(send(cli_sock[0], start_word, strlen(start_word) + 1, 0) == -1)
		err_handling("send");	
	
	if ((cli_sock[1] = accept(sv_sock, (struct sockaddr *)&cli, &clientlen)) == -1)	
		err_handling("accept");
	printf("Player 2 connected\n");

	
	while (1) {
		recv(cli_sock[0], word, sizeof(word), 0);
		printf("Player 1 sent %s\n", word);

		send(cli_sock[1], word, strlen(word) + 1, 0);
		
		recv(cli_sock[1], word, sizeof(word), 0);
		printf("Player 2 sent %s\n", word);
		
		send(cli_sock[0], word, strlen(word) + 1, 0);
	}

	close(cli_sock[0]);
	close(cli_sock[1]);
	close(sv_sock);

	return 0;
}



