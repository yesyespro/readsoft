
#include "lib/interface.h"
#include <string.h>
#include <unistd.h>

int ch,y,x,i,tempy,tempx,linhatemp=0,printline=0, temp_printline=0;

#define MAX_LINES (y - 2)

void liberar_scrollbar_text(char *array[], int linhas);

static const char *textos[] = {"Setas pra se mover:: ", "Sim", "Não"};

extern int linhas;


struct info_box{

	int height;
	int width;
	int starty;
	int startx;
	int tempy;
	int tempx;

};

int interface(FILE *open, int arq){
	if(open == NULL){
		fprintf(stderr,"Fudeu...\n");
		exit(EXIT_FAILURE);
	}
	rewind(open);
	
	char * scrollbar_text[linhas];


	for(i = 0; i != linhas; i++){		
		scrollbar_text[i] = malloc(1024*sizeof(char));
				
		if(scrollbar_text[i] == NULL){
			fprintf(stderr,"Alocação de memoria falhou\n");
			for(int j = 0; j < i; j++){
				free(scrollbar_text[i]);
			}
			exit(EXIT_FAILURE);
		}
		

	}
	
	
	char * temp_buffer = (char *) malloc(1024*sizeof(char));
	
	size_t tamanho_max = 1024;	
	size_t acompanha = 0;
		

	while(fgets(temp_buffer, tamanho_max, open) != NULL){		

		if(strlen(temp_buffer) + acompanha >= tamanho_max){			



			tamanho_max *=4;
			acompanha = strlen(temp_buffer);
		
			char *temp  = (char *) realloc(scrollbar_text[linhatemp], tamanho_max*sizeof(char));
			char *temp2 = (char *) realloc(temp_buffer, tamanho_max*sizeof(char));
		
			if(temp != NULL && temp2 != NULL){
				scrollbar_text[linhatemp] = temp;
				temp_buffer = temp2;
 			}else{
				fprintf(stderr,"Teve sorte hoje não em meu mano, foi erro ao realocar a memoria blz?");
				exit(EXIT_FAILURE);				
			}
		}

		strcpy(scrollbar_text[linhatemp], temp_buffer);		
		acompanha = strlen(scrollbar_text[linhatemp]);

		if(linhatemp <= linhas){
			linhatemp++;
		}

	}

	fclose(open);


	initscr();
	noecho();
	cbreak();
	curs_set(0);
		
	keypad(stdscr, TRUE);

	getmaxyx(stdscr, y,x);	

	getyx(stdscr , tempy, tempx);

	if(linhas < MAX_LINES){
		for(i = 0; i < linhas; i++){
			mvprintw(tempy++, 0, scrollbar_text[i]);
		}

		attron(A_REVERSE);
		mvprintw(y-1,0,"Press F1 to exit");
		attroff(A_REVERSE);

		refresh();
		while( (ch = getch()) != KEY_F(1) );
		
		endwin();	

		exit(EXIT_SUCCESS);
		
	}

	for(i = 0; i < MAX_LINES; i++){

		mvprintw(tempy++, 0,scrollbar_text[i]);
		printline++;
	}
	temp_printline = printline;

	refresh();	

	while( (ch = getch()) != EOF ){
		getmaxyx(stdscr, y,x);
		getyx(stdscr, tempy,tempx);
		
		switch(ch){
			case KEY_UP:
				if(printline != temp_printline){
					--printline;
				}
				break;
			case KEY_DOWN:
				if(printline != linhas && printline > 0){
					++printline;
				}	
				break;
		}
				
		printar(tempy, tempx, scrollbar_text,  printline);
	}


	free(temp_buffer);

	
	endwin();
	
	liberar_scrollbar_text(scrollbar_text, linhas);
	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx){
	WINDOW *local_win;
	local_win =  newwin(height, width, starty, startx);

	box(local_win,0,0);
	
	wrefresh(local_win);

	return local_win;
}
WINDOW *create_shadow(int height, int width, int starty, int startx){
	WINDOW *local_win;
	
	local_win = newwin(height, width, starty, startx);

	box(local_win,0,0);

	wclear(local_win);

	wrefresh(local_win);
	
	return local_win;

}
int pergunta_seria(int arq){
	
	int ch,x,y;
	int highlight = 0;

	struct info_box info;



	getmaxyx(stdscr, y,x);

	info.width = 55;
	info.height = 10;

	info.starty = (LINES - info.height) / 2;
	info.startx = (COLS - info.width) / 2;

	//definindo personalizando a janela principal! 
	start_color();
	
	init_pair(1, COLOR_WHITE, 17);	
	init_pair(2, COLOR_BLACK, 244);
	init_pair(3, COLOR_BLACK, COLOR_BLACK);


	bkgd(COLOR_PAIR(1));
	if(arq == 1){
		printw("Você forneceu um arquivo com extensão .c ou .cpp deseja ativar um modo melhor pra visualização? ");
	}else if(arq == 2){
	
		printw("Você forneceu um arquivo com extensão .py deseja ativar um modo melhor pra visualização? ");
	}else if(arq == 3){
		printw("Você forneceu um arquivo com extensão .php deseja ativar um modo melhor pra visualização? ");
	}
	
	attron(A_BOLD);
	mvprintw((LINES / 2) + 8,(COLS - strlen(textos[0])) / 2, textos[0]);
	waddch(stdscr,ACS_LARROW);
	waddch(stdscr,' ');
	waddch(stdscr,ACS_RARROW);	
	attroff(A_BOLD);
	refresh();	


	WINDOW *sombra = create_shadow(info.height+1,info.width+1,info.starty,info.startx);
	
	refresh();

	WINDOW *janela_principal = create_newwin(info.height,info.width,info.starty,info.startx);
	keypad(stdscr, TRUE);	


	wbkgd(janela_principal, COLOR_PAIR(2));


	wrefresh(janela_principal);
	desenhar(janela_principal, highlight);

	while( (ch = getch()) != EOF){
		//atualizar janela
		getmaxyx(stdscr,info.tempy, info.tempx);
		

		if(info.tempy != y){
			
			clear();

			getmaxyx(stdscr, y,x);

			if(arq == 1){
			
				printw("Você forneceu um arquivo com extensão .c ou .cpp deseja ativar um modo melhor pra visualização? ");
			}else if(arq == 2){
				
				printw("Você forneceu um arquivo com extensão .py deseja ativar um modo melhor pra visualização? ");
			}else if(arq == 3){
			
				printw("Você forneceu um arquivo com extensão .php deseja ativar um modo melhor pra visualização? ");
			}

			info.starty = (LINES - info.height) / 2;
			info.startx = (COLS - info.width) / 2;
		
			attron(A_BOLD);
			mvprintw((LINES / 2) + 8,(COLS - strlen(textos[0])) / 2, textos[0]);
			waddch(stdscr,ACS_LARROW);
			waddch(stdscr,' ');
			waddch(stdscr,ACS_RARROW);	
			attroff(A_BOLD);


			
			
			getmaxyx(stdscr, y,x);
			
			refresh();
			
			sombra =  create_shadow(info.height+1,info.width+1,info.starty,info.startx);
			
			werase(sombra);
			
			refresh();
			
			janela_principal = create_newwin(info.height, info.width, info.starty,info.startx);
			
			wbkgd(janela_principal, COLOR_PAIR(2));
			
			wrefresh(sombra);
			wrefresh(janela_principal);
			//final da atualização...
		}

		switch(ch){
			case KEY_LEFT:
				highlight = 1;
				//printw("Hello...");
				break;
			case KEY_RIGHT:
				highlight = 2;
				//printw("Hello 2...");
				break;
		}

		//highlight = 1;

		desenhar(janela_principal, highlight);
	//final do while		
	}

}

void desenhar(WINDOW *win, int highlight){
	int y,x;
	getmaxyx(win, y,x);
	//mvwprintw(win,0,0,"%d ", highlight);
	if(highlight == 1){
			
	//	mvwprintw(win,0,0,"1");
		wattron(win, A_REVERSE);
		mvwprintw(win, (y / 2), ( (x - strlen(textos[2]) ) /2 ) - 22, textos[1]);
		wattroff(win, A_REVERSE);	
		
			
		mvwprintw(win, (y / 2), ( (x - strlen(textos[1]) ) / 2 ) + 22, textos[2]);
		
	}else if(highlight == 2){
	//	mvwprintw(win,0,0,"2");
		wattron(win, A_REVERSE);
		mvwprintw(win, (y / 2), ( (x - strlen(textos[1]) ) / 2 ) + 22, textos[2]);
		wattroff(win, A_REVERSE);
		
			
		mvwprintw(win, (y / 2), ( (x - strlen(textos[2]) ) /2 ) - 22, textos[1]);

	}else{	
		mvwprintw(win, (y / 2), ( (x - strlen(textos[2]) ) /2 ) - 22, textos[1]);
			
		mvwprintw(win, (y / 2), ( (x - strlen(textos[1]) ) / 2 ) + 22, textos[2]);
	}	

	wrefresh(win);
}
void printar(int tempy, int tempx, char **buffer, int printline){
	int  y,x, valor_temp;

	clear();

	getmaxyx(stdscr, y,x);
	valor_temp = printline - MAX_LINES;
	
	for(x = valor_temp; x < linhas; x++){
		
		getyx(stdscr, tempy, tempx);
		if(tempy == MAX_LINES){
			break;
		}
		mvprintw(tempy++, 0,"%s",  buffer[x]);	
	}

	refresh();
}
void liberar_scrollbar_text(char *array[], int linhas) {
    for (int i = 0; i < linhas; i++) {
        if (array[i] != NULL) {
            free(array[i]);
            array[i] = NULL;
        }
    }
}
