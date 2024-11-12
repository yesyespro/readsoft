#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define RED "\e[31m"
#define GREEN "\e[32m"
#define RESET "\e[0m"


void help_print(const char *msg, ...);

#ifndef HELP_H
#define HELP_H


void help_print(const char *msg, ...){
	int quantas = 0;
	char buffer[150];
	va_list args;
	va_start(args, msg);
	

	printf("%s(Help)%s %s", RED, RESET, GREEN);
	for(const char *p = msg; *p != '\0'; p++){
		if(*p == '%'){
			printf("%s", va_arg(args, const char*));
			quantas++;
		}else{
			putchar(*p);
		}
	}
	va_end(args);		
	printf("%s", RESET);
}



#endif
