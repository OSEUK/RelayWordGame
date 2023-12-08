#include <stdlib.h>
#include <time.h>

// 미리 정의된 목록에서 무작위 단어를 얻는 함수
char* get_random_word() {
  char *start_words[] = {"유닉스", "자바", "테스트"};
  int num_words = sizeof(start_words) / sizeof(start_words[0]);
  srand((unsigned)time(NULL));
  int random_index = rand() % num_words;
  return start_words[random_index];
}
