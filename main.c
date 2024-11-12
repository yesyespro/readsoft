#include <stdio.h>
#include "lib/help.h"
#include "lib/interface.h"
#include <stdlib.h>
#include <string.h>

static const char *typefile[] = {".c", ".cpp", ".py", ".php"};
int linhas = 0;


void print_help(){
	help_print("Versão do programa: 0.0\n");
	fprintf(stderr, "-b iniciar leitura em hexadecimal\n-i modo leitura com ncurse tipo um less\n-l Mostrar linha apenas pro modo normal\n\n");
}


int main(int argc, char *argv[]){
	//Program create by yesyespro
	
	//flags 
	int b_flag = 0; // ler em binario (hexadecimal)
	int i_flag = 0;	 // interface em ncurse
	int l_flag = 0; // Modo mostrar quantas linhas apenas pra leitura normal
	//argumentos
	if(argc < 2){
		help_print("E burro mesmo em. Tem colocar argumento filho\n", getenv("USER"));
		return -1;		
	}else if(argc > 2){
		for(int i = 1; i < argc; i++){
			if(strcmp(argv[i], "-h") == 0){
				print_help();
				return 1;
			}else if(strcmp(argv[i], "-b") == 0){
				b_flag = 1;
			}else if(strcmp(argv[i], "-i") == 0){
				i_flag = 1;
			}else if(strcmp(argv[i], "-l") == 0){
				l_flag = 1;
			}
		}
		//Combatendo erros
		if(b_flag != 0 && l_flag != 0){
			help_print("Rapazinhoo... vou ter falar em, -l não e ativado pra quando você quer ler em bytes/hexadecimal. Burrokkkkkkk");
			return 1;
		}
	}

	//abrindo arquivo no modo que foi passado no argumentos
	FILE *open;
	if(b_flag == 1){
		open = fopen(argv[1], "rb");
	}else{
		open = fopen(argv[1], "r");
	}

	if(open == NULL){
		help_print("Rapaz, o arquivo % não existe ou foi removido ou problema e do senhor mesmo\n", argv[1]);
		return -1;
	}
	//Alocando memoria pra leitura dos arquivos!
	size_t tamanho_buffer = 1024;

	char *buffer = (char*)malloc(tamanho_buffer * sizeof(char)); //leitura normal

	char *buffer_bin; //leitura binaria 

	char *buffer_temp = (char*)malloc(tamanho_buffer * sizeof(char)); // Espaço de memoria temporaria 
	
	
//	char *tempmem;		

	if(buffer == NULL){
		help_print("Ihh rapaz, erro na alocação da memoria, Ve se tem memoria disponivel pra mim né filho\n");
		return -1;
	}	

	//Leitura binaria verificando se b_flag == 1
	if(b_flag == 1){
		long filesize;
		
		fseek(open, 0, SEEK_END); //Movendo o cursor pra o final do arquivo
		
		filesize =  ftell(open); //pegando o tamanho do arquivo

		rewind(open); //voltando para o inicio do arquivo


		buffer_bin = (char*) malloc(sizeof(char) * filesize+1); //Alocando buffer_bin pra receber a leitura dos hexadecimal

		if(fread(buffer_bin, sizeof(char), filesize, open) != filesize){ //Lendo o arquivo
			help_print("Error ao ler arquivo em binario, deve ser erro do proprio arquivo\n");
			return 1;
		}

		buffer_bin[filesize+1] = '\0';
		
		
	
	}else if(b_flag == 0){ //lendo normal
		size_t tamanho_temporario1;
		size_t tamanho_temporario2;
		while(fgets(buffer_temp, tamanho_buffer, open) != NULL){

			
			tamanho_temporario1 =  strlen(buffer);
			tamanho_temporario2 = strlen(buffer_temp);
				
			
			if(tamanho_temporario1 + tamanho_temporario2  >= tamanho_buffer){
				tamanho_buffer *= 3;
				buffer = (char*) realloc(buffer, tamanho_buffer);
				if(buffer == NULL){
					help_print("Ihh rapaz erro na alocação da memoria\n");
					fclose(open);
					free(buffer_temp);
					return 1;
				}

			}
			strcat(buffer, buffer_temp);
			
			if(i_flag == 1 || l_flag == 1){

				linhas++;
				/*
				for(int l = 0; l < strlen(buffer_temp); l++){
					if(buffer_temp[l] == '\n'){
						linhas++;
					}			
				}
				*/
			}
			


		}

	}


	free(buffer_temp);

	
	if(i_flag == 1){
		//interface
		int file = 0;
		for(int i = 0; i != 3; i++){
			char *p = strstr(argv[1], typefile[i]);
			if(p){
				switch(i){
					case 0:
						file = 1; // .c
						break;
					case 1:
						file = 1; // .cpp
						break;
					case 2:
						file = 2; // .py
						break;
					case 3:
						file = 3; // .php
						break;
				}
				
			}
			
			if(file != 0){
				break;
			}
			
		}
		if(open !=  NULL){
			interface(open, file);
		}else{
			fprintf(stderr, "Algo ocorreu com arquivo....\n");
		}

	}else if(b_flag == 0 && i_flag == 0){
		printf("%s\n", buffer);
	}else if(b_flag == 1 && i_flag == 0){
		for(char *it = buffer_bin; *it != '\0'; it++){
			printf("%02x", *it);
		}

		printf("\n");

	}


	
	free(buffer);	
	fclose(open);

	if(buffer_bin != NULL){
		free(buffer_bin);
	}
	return 0;
}

