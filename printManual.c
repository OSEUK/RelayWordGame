#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "printManual.h"

void printManual() {
	FILE* fp = fopen("manual.txt","r");
 	if(fp == NULL)
        	err_handling("fopen");
  	char c;
  	while((c = fgetc(fp)) != EOF) {
        	printf("%c",c);
  	}
}
