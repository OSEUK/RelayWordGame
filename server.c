#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define PORTNUM 9000

// 에러 발생시 에러 메시지를 출력하는 함수
void err_handling(char *error_message){
	perror(error_message);
	exit(1);
}

// 미리 정의된 목록에서 무작위 단어를 얻는 함수
char* get_random_word() {
	char *start_words[] = {"유닉스", "자바", "테스트"};
	int num_words = sizeof(start_words) / sizeof(start_words[0]);
	
	srand((unsigned)time(NULL));

	int random_index = rand() % num_words;
	return start_words[random_index];
}

// 첫 번째 단어의 끝과 두번째 단어의 시작을 비교하는 함수
int compare_first_last(const char *first_str, const char *second_str){
	if (strlen(first_str) == 0 || strlen(second_str) == 0) {
		return 0;
	}
	// -1, -2가 아닌 -2, -3을 사용한 이유는 send를 할 때 strlen(str) + 1을 해서 뒤에 \0문자가 포함되기 때문
	return (first_str[strlen(first_str) - 2] == second_str[1] && first_str[strlen(first_str)-3] == second_str[0]);
}

int main(void) {
        char word1[256], word2[256];
	char *Lose = "You Lose", *Win = "You Win", *Wait = "두 번째 클라리언트를 기다리는 중 입니다.";
	char* start_word = get_random_word();
	int check = 0;
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
	sin.sin_port = htons(PORTNUM);
	// 바인딩될 IP 주소를 할당
	sin.sin_addr.s_addr = inet_addr("172.18.208.58");

	// 서버 소켓에 주소 정보를 할당
	if (bind(sv_sock, (struct sockaddr *)&sin, sizeof(sin))) 
		err_handling("bind");

	// 서버 소켓이 최대 2개의 클라이언트의 연결을 기다리도록 설정
	if (listen(sv_sock, 2)) 
		err_handling("listen");

	// 첫 번째 클라이언트의 연결을 기다림
	if ((cli_sock[0] = accept(sv_sock, (struct sockaddr *)&cli, &clientlen)) == -1) 
		err_handling("accept");
	
	printf("Player 1 connected\n");
	// 두 번째 클라이언트의 연결을 기다리라는 메시지를 보냄
	if(send(cli_sock[0], Wait, strlen(Wait) + 1, 0) == -1)
	  err_handling("send");


	// 두 번째 클라리언트의 연결을 기다림
	if ((cli_sock[1] = accept(sv_sock, (struct sockaddr *)&cli, &clientlen)) == -1)
	        err_handling("accept");

	printf("Player 2 connected\n");

	// 첫 번째 클라이언트에게 랜덤 시작단어를 전달함 
	if(send(cli_sock[0], start_word, strlen(start_word) + 1, 0) == -1)
		err_handling("send");	

	// 첫 번째 클라이언트의 단어를 받음
	recv(cli_sock[0], word1, sizeof(word1), 0);
	// 비교함수를 통해서 틀렸는지 확인함
	if (!compare_first_last(start_word, word1)){
	  // 틀렸다면 첫 번째 클라이언트에게 Lose메시지를 두 번째 클라이언트에게 Win 메시지를 보냄
	  send(cli_sock[0], Lose, strlen(Lose) + 1, 0);
	  send(cli_sock[1], Win, strlen(Win) + 1, 0);
	  printf("Player 1 sent %s\n", word1);
	  printf("Player 1 Lose \n");
	}else{
	  // 맞다면 check변수를 1로 만들어서 while문이 돌아갈 수 있도록 함
	  check = 1;
	  printf("Player 1 sent %s\n", word1);
	  // 첫 번째 클라이언트가 보낸 단어를 두 번째 클라이언트에게 전달함
	  send(cli_sock[1], word1, strlen(word1) + 1, 0);
	}

	// 이제부터 돌아가면서 단어를 보냄 	
	while (check) {
	  // 두 번째 클라이언트의 단어를 받음 
	  recv(cli_sock[1], word2, sizeof(word2), 0);
	  // 비교함수를 통해서 틀렸는지 확인함
	  if (!compare_first_last(word1, word2)) {
	    printf("Player 2 sent %s\n", word2);
	    printf("Player 2 Lose \n");
	    // 틀렸다면 두 번째 클라이언트에게 Lose메시지를 첫 번째 클라이언트에게 Win메시지를 보냄
	    send(cli_sock[1], Lose, strlen(Lose) + 1, 0);
	    send(cli_sock[0], Win, strlen(Win) + 1, 0);
	    // 틀렸다면 while문을 종료함
	    break;
	  }
	  printf("Player 2 sent %s\n", word2);

	  // 두 번째 클라이언트가 보낸 단어를 첫 번째 클라이언트에게 전달함
	  send(cli_sock[0], word2, strlen(word2) + 1, 0);

	  // 첫 번째 클라이언트의 단어를 받음
	  recv(cli_sock[0], word1, sizeof(word1), 0);
	  // 비교함수를 통해서 틀렸는지 확인함
	  if (!compare_first_last(word2, word1)) {
	    printf("Player 1 sent %s\n", word1);
	    printf("Player 1 Lose\n");
	    // 틀렸다면 두 번째 클라이언트에게 Lose메시지를 첫 번째 클라이언트에게 Win메시지를 보냄
	    send(cli_sock[0], Lose, strlen(Lose) + 1, 0);
	    send(cli_sock[1], Win, strlen(Win) + 1, 0);
	    // 틀렸다면 while문을 종료함 
            break;
	  }
	  printf("Player 1 sent %s\n", word1);

	  // 첫 번째 클라이언트가 보낸 단어를 두 번째 클라이언트에게 전달함
	  send(cli_sock[1], word1, strlen(word1) + 1, 0);
	}

	// 소켓으 닫음
	close(cli_sock[0]);
	close(cli_sock[1]);
	close(sv_sock);

	return 0;
}



