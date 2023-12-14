#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "error.h"
#include "randword.h"
// 미리 정의된 목록에서 무작위 단어를 얻는 함수
char* get_random_word() {
	char *filename = "words.txt";
	
	// words.txt의 파일을 읽기모드로 연다.
	FILE* file = fopen(filename, "r");
	if (!file)
		err_handling("Could not open file");
	
	int word_count = 0;
	char buffer[256];
	// 파일의 줄 개수를 셈 = 단어 개수 파악
	while (fgets(buffer, sizeof(buffer), file) != NULL)
		word_count++;
	
	// 난수 설정하여 해당 줄에서 buffer에 단어를 저장하도록 함.
	srand((unsigned)time(NULL));	
	fseek(file, 0, SEEK_SET);
	int random_line = rand() % word_count;
	for(int i = 0; i < random_line; i++)
		fgets(buffer, sizeof(buffer), file);
	
	// 개행 문자 자리에 널 문자로 대체
	buffer[strcspn(buffer, "\n")] = '\0';

	fclose(file);
	
	// 동적 할당 메모리에 저장 후 반환
	char *result = strdup(buffer);
	
	return result;
}
