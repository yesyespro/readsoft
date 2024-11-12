#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

//Hahahah so pra definir

#ifndef INTERFACE_H
#define INTERFACE_H


int interface(FILE * buffer, int arq);
void printar(int tempy, int tempx, char **buffer, int printline);
WINDOW *create_newwin(int starty, int startx, int width, int height);
WINDOW *create_shadow(int starty, int startx, int width, int height);
int pergunta_seria(int arq);
void desenhar(WINDOW *win, int highlight);
void leitura_normal();

#endif
