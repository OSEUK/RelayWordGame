#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORTNUM 9000

void err_handling(char *error_message){
	perror(error_message);
	exit(1);
}

int main() {
	int sv_sock;
	char word[256];
	struct sockaddr_in sin;

	if ((sv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		err_handling("socket");

	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("172.19.230.129");

	if (connect(sv_sock, (struct sockaddr *)&sin, sizeof(sin))) 
		err_handling("connect");
	
	while(1) {
		if(recv(sv_sock, word, sizeof(word), 0) == -1)
			err_handling("recv");
		printf("Received %s\n", word);
		
		printf("Enter a word: ");
		scanf("%s", word);

		if (send(sv_sock, word, strlen(word) + 1, 0) == -1)
			err_handling("send");
	}
	close(sv_sock);
	
	return 0;
}

