#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORTNUM 9000

void err_handling(char *error_message) {
  perror(error_message);
  exit(1);
}

int main() {
  int sv_sock;
  char word[256];
  int state = 1;
  struct sockaddr_in sin;

  // 소켓을 생성
  if ((sv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    err_handling("socket");

  // 소켓 구조체를 초기화
  memset((char *)&sin, '\0', sizeof(sin));
  // 소켓 구조체에게 소켓 패밀리를 AF_INET으로 지정
  sin.sin_family = AF_INET;
  // 소켓이 사용할 포트 번호를 할당
  sin.sin_port = htons(PORTNUM);
  // 바인딩될 IP 주소를 할당
  sin.sin_addr.s_addr = inet_addr("172.18.208.58");

  printf("게임 시작을 기다리는 중 입니다.");

  // 소켓 주소 구조체에 지정한 서버로 connect() 함수를 사용해 연결을 요청
  if (connect(sv_sock, (struct sockaddr *)&sin, sizeof(sin)))
    err_handling("connect");

  // 끝말잇기 시작
  while(state) {
    if(recv(sv_sock, word, sizeof(word), 0) == -1)
      err_handling("recv");

    // 상대방이 보낸 단어 출력 
    printf("Received : %s\n", word);
    // 승패 결과가 오면 state를 0으로 바꿔 while문 탈출 
    if(strcmp(word, "You Lose\0") == 0 || strcmp(word, "You Win\0") == 0)
      state =0;
    else{
      // 아니라면 다음 단어를 입력
      printf("Enter a word: ");
      scanf("%s", word);

      // 서버에게 단어를 전달 
      if (send(sv_sock, word, strlen(word) + 1, 0) == -1)
	err_handling("send");
    }
  }

  // 끝말잇기 종료 후 소켓 닫음
  close(sv_sock);
  return 0;
}
