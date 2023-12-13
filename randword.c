#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "error.h"
#include "randword.h"
// 미리 정의된 목록에서 무작위 단어를 얻는 함수
char* get_random_word() {
	char *filename = "words.txt";
	
	FILE* file = fopen(filename, "r");
	if (!file)
		err_handling("Could not open file");

	int word_count = 0;
	char buffer[256];
	while (fgets(buffer, sizeof(buffer), file) != NULL)
		word_count++;

	srand((unsigned)time(NULL));	
	fseek(file, 0, SEEK_SET);
	int random_line = rand() % word_count;
	for(int i = 0; i < random_line; i++)
		fgets(buffer, sizeof(buffer), file);

	buffer[strcspn(buffer, "\n")] = '\0';

	fclose(file);

	char *result = strdup(buffer);
	
	return result;
}
