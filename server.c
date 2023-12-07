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
	char *start_words[] = {"유닉스", "자바", "테스트"};
	int num_words = sizeof(start_words) / sizeof(start_words[0]);
	
	srand((unsigned)time(NULL));

	int random_index = rand() % num_words;
	return start_words[random_index];
}

int compare_first_last(const char *first_str, const char *second_str){
	if (strlen(first_str) == 0 || strlen(second_str) == 0) {
		return 0;
	}

	return (first_str[strlen(first_str) - 2] == second_str[1] && first_str[strlen(first_str)-3] == second_str[0]);
}

int main(void) {
        char word1[256], word2[256];
	char *Lose, *Win;
	Lose = "You Lose";
	Win = "You Win";
	char* start_word = get_random_word();
	int check = 0;
	struct sockaddr_in sin, cli;	
	int sv_sock, cli_sock[2], clientlen = sizeof(cli);
	
	if ((sv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		err_handling("socket");

	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("172.19.230.129");
	
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

	recv(cli_sock[0], word1, sizeof(word1), 0);
	if (!compare_first_last(start_word, word1)){
	  send(cli_sock[0], Lose, strlen(Lose) + 1, 0);
	  send(cli_sock[1], Win, strlen(Win) + 1, 0);
	  printf("Player 1 sent %s\n", word1);
	  printf("Player 1 Lose \n");
	}
	else{
	  check = 1;
	  printf("Player 1 sent %s\n", word1);
	  send(cli_sock[1], word1, strlen(word1) + 1, 0);
	}
	
	while (check) {
	        recv(cli_sock[1], word2, sizeof(word2), 0);
		if (!compare_first_last(word1, word2)) {
		  printf("Player 2 sent %s\n", word2);
		  printf("Player 2 Lose \n");
		  send(cli_sock[1], Lose, strlen(Lose) + 1, 0);
		  send(cli_sock[0], Win, strlen(Win) + 1, 0);
		  break;
		}
		printf("Player 2 sent %s\n", word2);
		
		send(cli_sock[0], word2, strlen(word2) + 1, 0);

		recv(cli_sock[0], word1, sizeof(word1), 0);
		if (!compare_first_last(word2, word1)) {
		  printf("Player 1 sent %s\n", word1);
		  printf("Player 1 Lose\n");
		  send(cli_sock[0], Lose, strlen(Lose) + 1, 0);
		  send(cli_sock[1], Win, strlen(Win) + 1, 0);
		  break;
		}
		printf("Player 1 sent %s\n", word1);

		send(cli_sock[1], word1, strlen(word1) + 1, 0);
	}

	close(cli_sock[0]);
	close(cli_sock[1]);
	close(sv_sock);

	return 0;
}



