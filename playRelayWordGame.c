#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "error.h"
#include "sendBySocket.h"
#include "compare_first_last.h"


#define LOSE "You Lose"
#define WIN "You Win"

void playRelayWordGame(int cli_sock[2], const char *start_word) {
        char word1[256], word2[256];
        int check = 0;
        // 첫 번째 클라이언트에게 랜덤 시작단어를 전달
        sendBySocket(cli_sock[0], start_word);

        // 첫 번째 클라이언트의 단어를 받음
        recv(cli_sock[0], word1, sizeof(word1), 0);
        // 비교함수를 통해서 틀렸는지 확인함
        if (!compare_first_last(start_word, word1)){
          // 틀렸다면 첫 번째 클라이언트에게 Lose메시지를 두 번째 클라이언트에게 Win 메시지를 보냄
          sendBySocket(cli_sock[0], LOSE);
          sendBySocket(cli_sock[1], WIN);
          printf("Player 1 sent %s\n", word1);
          printf("Player 1 Lose \n");
        }
        else{
          // 맞다면 check변수를 1로 만들어서 while문이 돌아갈 수 있도록 함
          check = 1;
          printf("Player 1 sent %s\n", word1);
          // 첫 번째 클라이언트가 보낸 단어를 두 번째 클라이언트에게 전달함
          sendBySocket(cli_sock[1], word1);
        }

         while(check){
          // 두 번째 클라이언트의 단어를 받음
          recv(cli_sock[1], word2, sizeof(word2), 0);
          // 비교함수를 통해서 틀렸는지 확인함
          if (!compare_first_last(word1, word2)) {

            printf("Player 2 sent %s\n", word2);
            printf("Player 2 Lose \n");
            // 틀렸다면 두 번째 클라이언트에게 Lose메시지를 첫 번째 클라이언트에게 Win메시지를 보냄
            sendBySocket(cli_sock[1], LOSE);
            sendBySocket(cli_sock[0], WIN);
            // 틀렸다면 while문을 종료함
                break;
          }
          printf("Player 2 sent %s\n", word2);

          // 두 번째 클라이언트가 보낸 단어를 첫 번째 클라이언트에게 전달함
          sendBySocket(cli_sock[0], word2);

          // 첫 번째 클라이언트의 단어를 받음
          recv(cli_sock[0], word1, sizeof(word1), 0);
          // 비교함수를 통해서 틀렸는지 확인함
          if (!compare_first_last(word2, word1)) {
            printf("Player 1 sent %s\n", word1);
            printf("Player 1 Lose\n");
            // 틀렸다면 두 번째 클라이언트에게 Lose메시지를 첫 번째 클라이언트에게 Win메시지를 보냄
            sendBySocket(cli_sock[0], LOSE);
            sendBySocket(cli_sock[1], WIN);
            // 틀렸다면 while문을 종료함
            break;
          }
          printf("Player 1 sent %s\n", word1);

          // 첫 번째 클라이언트가 보낸 단어를 두 번째 클라이언트에게 전달함
          sendBySocket(cli_sock[1], word1);
        }

        // 소켓을 닫음
        close(cli_sock[0]);
        close(cli_sock[1]);
}
