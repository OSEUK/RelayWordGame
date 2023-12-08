#include <stdlib.h>
#include <stdio.h>

// 에러 발생시 에러 메시지를 출력하는 함수
void err_handling(char *error_message){
  perror(error_message);
  exit(1);
}
