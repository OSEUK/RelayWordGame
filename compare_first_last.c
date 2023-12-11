#include <stdio.h>
#include <string.h>
#include "compare_first_last.h"

int compare_first_last(const char *first_str, const char *second_str){
        if (strlen(first_str) == 0 || strlen(second_str) == 0) {
                return 0;
        }
        // -1, -2가 아닌 -2, -3을 사용한 이유는 send를 할 때 strlen(str) + 1을 해서 뒤에 \0문자가 포함되기 때문
        return (first_str[strlen(first_str) - 2] == second_str[1] && first_str[strlen(first_str)-3] == second_str[0]);
}
