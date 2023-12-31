#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include "error.h"
#include "randword.h"
#include "compare_first_last.h"
#include "sendBySocket.h"

#define LOSE "You Lose"
#define WIN "You Win"

void playRelayWordGame(int cli_sock[2], const char *start_word);

int main(int argc, char* argv[]) 
{	 
	// 포트 넘버와 IP 주소를 입력하여 게임을 진행해야 함. 
	// 입력하지 않을 시 사용 방법 제공
	if( argc != 3){
		printf("게임 사용 방법: ./ser [Port Number] [My IP Address]\n");
	       	exit(1);
	}
	// 게임 시작 시 제시어
	char* start_word = get_random_word();
	
	// 사용할 소켓 변수 정의	
	struct sockaddr_in sin, cli;	
	int sv_sock, cli_sock[2], clientlen = sizeof(cli);

	// 소켓 생성
	if ((sv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		err_handling("socket");

	// 소켓 구조체 초기화
	memset((char *)&sin, '\0', sizeof(sin));
	// 소켓 구조체에서 소켓 패밀리를 AF_INET로 지정
	sin.sin_family = AF_INET;
	// 소켓이 사용할 포트 번호를 할당
	sin.sin_port = htons(atoi(argv[1]));
	// 바인딩될 IP 주소를 할당
	sin.sin_addr.s_addr = inet_addr(argv[2]);

	// 서버 소켓에 주소 정보를 할당
	if (bind(sv_sock, (struct sockaddr *)&sin, sizeof(sin))) 
		err_handling("bind");

	// 서버 소켓이 최대 2개의 클라이언트의 연결을 기다리도록 설정
	if (listen(sv_sock, 2)) 
		err_handling("listen");
	
	// 클라이언트 두 명의 connect를 받음
	for (int i = 0; i < 2; i++){
		if ((cli_sock[i] = accept(sv_sock, (struct sockaddr *)&cli,(socklen_t *)&clientlen)) == -1)
				err_handling("accept");
		
		printf("Player %d connected\n", i + 1);
	}

	pid_t pid;
	
	// fork 사용, 자식 프로세스에서 게임을 돌린다.	
	if ((pid = fork()) == -1)
		err_handling("fork");
	else if (pid == 0){
		// 자식 프로세스
		playRelayWordGame(cli_sock, start_word);
	}
	else{	
		// 부모 프로세스
		close(cli_sock[0]);
		close(cli_sock[1]);
		wait(NULL);
	}

	close(sv_sock);
	
	return 0;
}
