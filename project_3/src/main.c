/*  Projeto 3
    Jogo da Velha



    Estrutura de Dados	Turma E	1/2010
*/

/*
	============================
	||	Descricao do projeto  ||
	============================

	Programa simulador de um jogo da velha;
	O programa deve implementar a estratégia MiniMax e jogar contra um adversário humano;
	A ordem de quem inicia o jogo deve ser aleatória.


	O código deve ser bem documentado, de forma modular com funções para cada tarefa independente, 
	realizado por dois (2) estudantes do curso usando "pair programming", e entregue via sistema 
	http://aprender.unb.br do curso, no prazo estipulado.


*/

#include <stdio.h>
#include <stdio_ext.h>		/* __fpurge(stdin) !!! http://linux.about.com/library/cmd/blcmdl3___fpurge.htm !!! */
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define erro perror("ERRO!");


/*	ARRUMAR ESTRUTURA */
struct arv{
	char vel[3][3];
	int pont;
	struct arv *filho;
	struct arv *prox;
};

typedef struct arv Arv;

void inicializaVelha(char [3][3]);
void imprimeVelha(char [3][3],int,unsigned int,unsigned int,unsigned int);
void geraArvore(Arv *,int);						/* Gera restante da arvore*/
void jogaAleatorio(Arv *);						/* Joga aleatorio em qualquer posição*/ 
void liberaArvore(Arv *);						/* Libera memoria utilizada pela arvore*/
void pausa(void);
Arv* criaNo(char [3][3]);
Arv* miniMax(Arv *);
Arv* solicitaJogada(Arv *);						/* Entrada jogador humano*/
int verificaFim(Arv *);
int verificaVelha(char [3][3]);
int geraRand(int,int);							/* Gera numero randomico*/
Arv* jogadaEvidente(Arv *,int);
Arv* criaPosGanho(Arv *);


int main(void){
	srand(time(NULL));
	int vez, opcao=1, ganhou;
	unsigned int pontuacao=0,velha=0,jogos=0;
	Arv *geral;
	Arv *atual;


	do{
		vez=geraRand(0,10);
		if(vez<=4){
			vez=0;
		}
		else{
			vez=1;
		}

		/*Gera arvore inteira*/	
		geral=malloc(sizeof(Arv));
		inicializaVelha(geral->vel);		
		geraArvore(geral,vez);
		atual=geral;

		do{
			imprimeVelha(atual->vel,vez,pontuacao,velha,jogos);
			if(vez){
				atual=atual->filho;
				atual=miniMax(atual);
				vez=0;
			}
			else{
				atual=solicitaJogada(atual);
				vez=1;
			}
			ganhou=verificaFim(atual);
		}while(!ganhou);
		switch (ganhou){
			case 1:
				pontuacao++;
				vez=3;
				break;
			case 3:
				velha++;
				vez=4;
				break;
			default:
				vez=2;
		}
		jogos++;
		imprimeVelha(atual->vel,vez,pontuacao,velha,jogos);
		liberaArvore(geral);

		do{
			printf("\n>>Deseja sair?\n");
			printf("\t1- CONTINUAR\t2-SAIR\n");
			scanf("%d",&opcao);
			if((opcao!=1) && (opcao!=2)){
				printf("\t!!! OPCAO INVALIDA !!!\n");
			}
		}while((opcao!=1) && (opcao!=2));
	}while(opcao==1);

	return 0;
}

int geraRand(int min,int max){
	int r;
	r=min+(rand()%(max-min+1));
	return r;
}

void pausa(void){
	printf("\t\t==pressione enter==\n");
	__fpurge(stdin);
	getchar();

}

void inicializaVelha(char velha[3][3]){
	int i,j;

	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			velha[i][j]=' ';
		}
	}
}

void imprimeVelha(char velha[3][3],int vez,unsigned int pont,unsigned int v,unsigned int jogos){
	int i,j;
	system("clear");
	printf("Jogador:  %u\t\tComputador:  %u\t\tJogos:  %u\n",pont,((jogos-v)-pont),jogos);
	if(vez==1){
		printf("\tVez do computador...\n\n");
	}
	else if(vez==0){
		printf("\tSua vez...\n\n");
	}
	else if(vez==2){
		printf("\t!!Jogador perdeu!!\n\n");
	}
	else if (vez==3){
		printf("\t!!Computador perdeu!!\n\n");
	}
	else{
		printf("\t!!Velha!!\n\n");
	}


	printf("    1  2  3\n");
	for(i=0;i<3;i++){
		switch(i){
			case 0:
				printf("a   ");
				break;
			case 1:
				printf("b   ");
				break;
			case 2:
				printf("c   ");
				break;
		}
		for(j=0;j<3;j++){
			printf("%c  ",velha[i][j]);
		}
		printf("\n");
	}
}

/*	vez = 1 -> Computador
	vez = 0 -> Jogador	*/
void geraArvore(Arv *entrada,int vez){
	int i, j;
	Arv *aux, *atual;

	if(entrada!=NULL){
		entrada->filho=criaNo(entrada->vel);
		atual=entrada->filho;
	
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				if((atual->vel[i][j])==' '){
					atual->prox=criaNo(atual->vel);				
					aux=atual;
					if(vez){
						atual->vel[i][j]='X';						
					}
					else{
						atual->vel[i][j]='O';
					}
					if(verificaFim(aux)==0){
						geraArvore(atual,!vez);
					}

					atual=atual->prox;
				}
					
			}/* FIM FOR j*/
		
		}/* FIM FOR i*/
		free(aux->prox);
		aux->prox=NULL;

	}

}

Arv* criaNo(char velha[3][3]){
	int i,j;
	Arv *aux;
	aux=(Arv *)malloc(sizeof(Arv));
	aux->pont=0;
	aux->filho=NULL;
	aux->prox=NULL;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			aux->vel[i][j]=velha[i][j];
		}
	}
	return aux;
}

void liberaArvore(Arv *entrada){

	if(entrada!=NULL){
		liberaArvore(entrada->filho);
		liberaArvore(entrada->prox);
		free(entrada);
	}

}


/* 0-> contiua	1-> jogador	2-> computador 3-> velha*/
int verificaFim(Arv *entrada){
	int i, j,flag=0;

	for(i=0;i<3;i++){
		if(((entrada->vel[i][0])==(entrada->vel[i][1])) && ((entrada->vel[i][1])==(entrada->vel[i][2]))){
			if(entrada->vel[i][0]=='X'){
				return 2;
			}
			else if(entrada->vel[i][0]=='O'){
				return 1;
			}
		}
		if(((entrada->vel[0][i])==(entrada->vel[1][i])) && ((entrada->vel[1][i])==(entrada->vel[2][i]))){
			if(entrada->vel[0][i]=='X'){
				return 2;
			}
			else if(entrada->vel[0][i]=='O'){
				return 1;
			}
		}
		for(j=0;j<3;j++){
			if(entrada->vel[i][j]==' '){
				flag=1;
			}
		}
	}
	if(((entrada->vel[0][0])==(entrada->vel[1][1])) && ((entrada->vel[1][1])==(entrada->vel[2][2]))){
		if(entrada->vel[0][0]=='X'){
			return 2;
		}
		else if(entrada->vel[0][0]=='O'){
			return 1;
		}
	}

	if(((entrada->vel[0][2])==(entrada->vel[1][1])) && ((entrada->vel[1][1])==(entrada->vel[2][0]))){
		if(entrada->vel[0][2]=='X'){
			return 2;
		}
		else if(entrada->vel[0][2]=='O'){
			return 1;
		}
	}

	if(flag){
		return 0;
	}
	else{
		return 3;
	}

}

Arv* miniMax(Arv *atual){
	int x, y;

	Arv *aux;
	aux=jogadaEvidente(atual,1);
	if(aux!=NULL){
		return aux;
	}
	aux=jogadaEvidente(atual,0);
	if(aux!=NULL){
		return aux;
	}
	aux=criaPosGanho(atual);
	if(aux!=NULL){
		return aux;
	}

	x=geraRand(0,2);
	y=geraRand(0,2);
	while((atual->vel[x][y]) != 'X'){
		atual=atual->prox;
	}
	return aux;
}

/* player  0-> jogador  1-> computador*/
Arv* jogadaEvidente(Arv *filho,int player){

	int conth, contv, f=1, valorcomp, i, j;
	char p, o;

	if(player==1){
		p='X';
		o='O';
		valorcomp=15;
	}
	else{
		p='O';
		o='X';
		valorcomp=3;
	}
printf("\nValor=%d\n",valorcomp);
	do{
		for(i=0;i<3;i++){
			conth=0;
			contv=0;
			for(j=0;j<3;j++){
				if(filho->vel[i][j]==p){
					conth+=5;
				}
				else if(filho->vel[i][j]==o){
					conth-=7;
				}

				if(filho->vel[j][i]==p){
					contv+=5;
				}
				else if(filho->vel[j][i]==o){
					contv-=7;
				}
			}

/*	printf("%d\t%d\n",conth,contv);*/

			if((conth==valorcomp) || (contv==valorcomp)){
				return filho;
			}
		}
		
		for(i=0;i<3;i++){
			if(filho->vel[i][i]==p){
				conth+=5;
			}
			else if(filho->vel[i][i]==o){
				conth-=7;
			}
			j=2-i;
			if(filho->vel[j][i]==p){
				contv+=5;
			}
			else if(filho->vel[j][i]==o){
				contv-=7;
			}

			if((conth==valorcomp) || (contv==valorcomp)){
				return filho;
			}
		}
		f++;
		filho=filho->prox;
	}while(filho!=NULL);

	return NULL;
}

Arv* criaPosGanho(Arv *filho){

	int conth=0, contv=0, f=1, valorcomp, i, j;
	char p, o;

	p='X';
	o='O';
	valorcomp=10;

	do{
		for(i=0;i<3;i++){
			conth=0;
			contv=0;
			for(j=0;j<3;j++){
				if(filho->vel[i][j]==p){
					conth+=5;
				}
				else if(filho->vel[i][j]==o){
					conth-=7;
				}

				if(filho->vel[j][i]==p){
					contv+=5;
				}
				else if(filho->vel[j][i]==o){
					contv-=7;
				}
			}
			if((conth==valorcomp) || (contv==valorcomp)){
				return filho;
			}
		}
		
		for(i=0;i<3;i++){
			if(filho->vel[i][i]==p){
				conth+=5;
			}
			else if(filho->vel[i][i]==o){
				conth-=7;
			}
			j=2-i;
			if(filho->vel[j][i]==p){
				contv+=5;
			}
			else if(filho->vel[j][i]==o){
				contv-=7;
			}
			if((conth==valorcomp) || (contv==valorcomp)){
				return filho;
			}
			conth=0;
			contv=0;
		}
		f++;
		filho=filho->prox;
	}while(filho!=NULL);

	return NULL;
}

Arv* solicitaJogada(Arv *atual){
	char c;
	int x, y, flag=1;
	do{
		printf("Informe onde deseja marcar:\t\tFormato de entrada (ex): a 2\n");
		__fpurge(stdin);
		scanf("%c %d",&c,&y);
		if(isalpha(c)){
			c=tolower(c);
			switch (c){
				case 'a':
					x=0;
					flag=0;
					break;
				case 'b':
					x=1;
					flag=0;
					break;
				case 'c':
					x=2;
					flag=0;
					break;
				default:
					printf("\t\t>>>ENTRADA INVALIDA<<<\n");
			}

			if((y>=1) && (y<=3)){
				flag=0;
			}
			else{
				printf("\t\t>>>ENTRADA INVALIDA<<<\n");
			}

			if(!flag){
				y--;
				if(atual->vel[x][y]==' '){
					atual=atual->filho;
					while((atual->vel[x][y])!='O'){
						atual=atual->prox;
					}
				}
				else{
					flag=1;
					printf("\t\t>>>ENTRADA INVALIDA<<<\n");
					printf("\t\t>>>POSICAO MARCADA<<<\n");
				}
			}
		}
	}while(flag);
	
	return atual;
}











