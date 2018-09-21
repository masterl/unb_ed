/*  Projeto 1
    Comparador de Textos



    Estrutura de DadosTurma1/2010
*/

/*
	============================
	||	Descricao do projeto  ||
	============================
	
    Escreva um programa em linguagem C,
    o qual deverá ler dois (2) arquivos texto fornecidos pelo usuário (doc1.txt e doc2.txt),
    calcular a semelhança entre os documentos pelo método aqui fornecido,
    gerar um arquivo resultado compararDoc.txt com as palavras e frequências calculadas,
    bem como o valor de semelhança entre os dois.
    Na tela uma indicação de quais arquivos foram analisados, o resultado da métrica   de   comparação,
    e uma frase indicando se houve ou não plágio pela semelhança dos documentos (e.g. 50% de semelhança indicaria plágio).
    Dois arquivos de documentos (doc1.txt, doc2.txt) serão fornecidos para teste.
    O código deve ser bem documentado, de forma modular com funções para cada tarefa independente, 
    realizado por dois (2) estudantes do curso usando “pair programming”, e entregue via sistema
    http://aprender.unb.br do curso, no prazo estipulado.

*/

/*
	===================
	||	Observacoes  ||
	===================

	-Nao esquecer de utilizar o comando -lm na hora de compilar, pois este programa utiliza a biblioteca math.h

	-Os arquivos de entrada devem ser nomeados doc1.txt e doc2.txt

	-Os arquivos de entrada devem estar na mesma pasta em que se encontra o executavel

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>   /* float acos(float);  */

#define PI 3.1415
#define teste printf("teste\n");

/*
	=================================================
	||	Estrutura do tipo lista encadeada simples  ||
	=================================================

	Estrutura que armazenara cada palavra do texto e a quantidade de vezes em que ela aparece

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		pal[20]
			-String
			++Contem a palavra lida
		c
			-unsigned int
			++Contem a quantidade de vezes que a palavra contida em "pal" se repete
		*prox
			-Ponteiro para estrutura do tipo "fila"
			++Armazena o endereco do proximo elemento da lista
++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
typedef struct{
	char pal[20];
	unsigned int c;
	struct fila *prox;
}fila;

/*
	=====================================================
	||	Prototipos das funcoes utilizadas no programa  ||
	=====================================================

	Comentarios detalhados sobre cada funcao encontram-se apos a funcao "main", antes da declaracao de cada uma.

*/
void le_txt(FILE *,fila *);						/*	(ARQUIVO,ponteiro_fila) */
void produto_interno(fila *,fila *,int *);		/*  (ponteiro_fila_A,ponteiro_fila_B,ponteiro_produto) */
void calcula_modulo(fila *,float *);			/*  (ponteiro_fila,ponteiro_modulo) */
void acha_ang(fila *, fila *,float *);			/*	(ponteiro_fila_A,ponteiro_fila_B,ponteiro_angulo) */
int verifica_existencia(fila *,char *);			/*	(ponteiro_fila,ponteiro_string) */
void inicializa_valores(fila *);				/*	(ponteiro_fila) */
int checa_quantidade(fila*,char *);				/*	(ponteiro_fila,string) */
float calcula_porcentagem(float);				/*	(arco cosseno) */
void libera_memoria(fila *,fila*);				/*	(ponteiro_fila_A,ponteiro_fila_B) */
void result(float *,fila *,fila *);				/*	(ponteiro_porcentagem_calculada,ponteiro_fila_A,ponteiro_fila_B) */

/*
	============
	||	main  ||
	============

	1-Chama as funcoes na ordem certa a fim de alcancar o objetivo do programa
	2-Armazena os dados importantes ao programa

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*primeiroa e *primeirob
			-ponteiros para estrutura do tipo "fila"
			++Armazenam o endereco do primeiro elemento de cada lista
		textoa e textob
			-estruturas do tipo "fila"
			++Armazenam o primeiro elemento de cada lista
		ang
			-float
			++Armazena o "angulo" entre os textos
		porc
			-float
			++Armazena a porcentagem de semelhanca entre os textos
++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/
int main(void){
	
	system("clear");

	fila *primeiroa;
	fila *primeirob;
	fila textoa;
	fila textob;
	primeiroa=&textoa;
	primeirob=&textob;
	inicializa_valores(primeiroa);
	inicializa_valores(primeirob);
	float ang=0,porc;

	le_txt(fopen("doc1.txt","r"),primeiroa);
	le_txt(fopen("doc2.txt","r"),primeirob);

	acha_ang(primeiroa,primeirob,&ang);
	porc=calcula_porcentagem(ang);
	result(&porc,primeiroa,primeirob);

	libera_memoria(primeiroa,primeirob);

	return 0;
}

/*
	=========================
	||	iniciliza_valores  ||
	=========================

	Atribui valores iniciais para cada elemento novo da lista

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*termo
			-ponteiro para estrutura do tipo "fila"
			++Contem o endereco do termo a ser inicializado
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
*/
void inicializa_valores(fila *termo){
	termo->pal[0]='\0';
	termo->c=0;
	termo->prox=NULL;
}

/*
	===========================
	||	verifica_existencia  ||
	===========================

	1-Verifica a existencia de uma determinada palavra na lista
		1.1-Retorna 1 caso o elemento exista
		1.2-Retorna 0 caso o elemento nao exista

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*dados
			-ponteiro para estrutura do tipo "fila"
			++Contem endereco para checagem
		pal
			-String
			++Contem a palavra que se deseja verificar a existencia
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++


*/
int verifica_existencia(fila *dados,char pal[20]){
	while(dados!=NULL){
		if(!strcmp(dados->pal,pal)){
			(dados->c)++;
			return 1;
		}
		else{
			dados=(fila *)dados->prox;
		}
	}
	return 0;
}

/*
	==============
	||	le_txt  ||
	==============

	Abre o arquivo de texto e analisa frequencia de palavras.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*put
			-ponteiro para estrutura do tipo "fila"
			++Contem endereco do elemento atual
		*ant
			-ponteiro para estrutura do tipo "fila"
			++Contem endereco do elemento anterior
		pal
			-String
			++Armazena os caracteres lidos
		ch
			-char
			++Contem caracter lido do arquivo
		x
			-int
			++Controla posicao do vetor de caracteres "pal" onde sera colocado cada caracter lido
		leu
			-int
			++Flag que indica a leitura de caracter valido
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++


*/
void le_txt(FILE *txt,fila *p){
	
	fila *put;
	fila *ant;
	put=p;
	char pal[20];
	char ch;
	int x, leu=0;
	x=0;
	
	while((ch=fgetc(txt))!=EOF){
		if(isalnum(ch)){
			ch=(char)tolower(ch);
			pal[x]=ch;
			x++;
			leu=1;
		}
		else{
			if(leu==1){
				pal[x]='\0';
				if(!verifica_existencia(p,pal)){
					strcpy(put->pal,pal);
					(put->c)++;
					(put->prox)=(fila *) malloc(sizeof(fila));
					ant=put;
					put=(fila *)put->prox;
					inicializa_valores(put);
				}
			}
			leu=0;
			x=0;
		}
	} /*fim while*/

	ant->prox=NULL;
	free(put);
	fclose(txt);
}

/*
	======================
	||	produto_interno ||
	======================

	Calcula produto interno entre os "vetores" de palavras

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*dadoa e *dadob
			-ponteiro para estrutura do tipo "fila"
			++
				>dadoa armazena endereco atual do elemento da fila A
				>dadob armazena endereco do primeiro termo da fila B
		*prod
			-ponteiro para inteiro
			++Contem endereco da variavel que ira receber o resultado do produto interno entre os vetores
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void produto_interno(fila *dadoa,fila *dadob,int *prod){
	*prod=0;
	while(dadoa!=NULL){
		*prod+=((dadoa->c)*(checa_quantidade(dadob,dadoa->pal)));
		dadoa=(fila *)dadoa->prox;
	}/*fim while*/
}

/*
	======================
	||	calcula_modulo  ||
	======================

	Calcula modulo do vetor de palavras

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*dado
			-ponteiro para estrutura do tipo "fila"
			++Contem endereco atual do termo que sera incluso no calculo do modulo
		*mod
			-ponteiro para float
			++Contem endereco da variavel que ira receber o valor do modulo do vetor
		soma
			-float
			++Armazena soma dos termos ao quadrado
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void calcula_modulo(fila *dado,float *mod){	
	int soma=0;

	while(dado!=NULL){
		soma+=((dado->c)*(dado->c));
		dado=(fila *)dado->prox;
	} /*fim while*/

	*mod=sqrt(soma);
}

/*
	arcos(produto_interno / (modulo_A * modulo_B) )
	================
	||	acha_ang  ||
	================

	Calcula "angulo" entre os textos

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*dadoa e *dadob
			-ponteiros para estrutura do tipo "fila"
			++Contem endereco do primeiro termo das filas A e B respectivamente
		*ang
			-ponteiro para float
			++Armazena endereco da variavel que ira receber o valor do angulo
		moda e modb
			-float
			++Armazenam o valor dos modulos da fila A e B respectivamente
		prod
			-int
			++Armazena o produto interno entre as fila A e B
		y
			-float
			++Armazena o resultado da divisao do produto interno pelo produto dos modulos de cada fila
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void acha_ang(fila *dadoa, fila *dadob,float *ang){
	float moda, modb;
	int prod;
	float y;

	produto_interno(dadoa,dadob,&prod);
	calcula_modulo(dadoa,&moda);
	calcula_modulo(dadob,&modb);
	y=prod/(moda*modb);
	if((int)y==1){
		*ang=acos(1.00);
	}
	else{
		*ang=acos(y);
	}

}

/*
	==============
	||	result  ||
	==============

	Fornece saida do programa.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*porc
			-ponteiro para float
			++Contem endereco da variavel que contem o valor da porcentagem de semelhanca entre os textos
		*dadoa e *dadob
			-ponteiros para estrutura do tipo "fila"
			++Contem endereco do termo atual
		*saida
			-ponteiro para arquivo
			++Usado para colocar cada caracter no arquivo de saida (compararDoc.txt)
		x
			-String
			++Armazena a quantidade de vezes em que a palavra atual se repetiu ao longo do texto
		igualdade
			-String
			++Armazena string de saida
				>Utilizada para auxiliar na saida do programa
		temp
			-String
			++Armazena palavra atual
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++


>>>Lembrete:

	>>>	gerar um arquivo resultado compararDoc.txt com as palavras e frequências calculadas,
	>>>	bem como o valor de semelhança entre os dois.
	>>>	Na tela uma indicação de quais arquivos foram analisados, o resultado da métrica   de   comparação,
	>>>	e uma frase indicando se houve ou não plágio pela semelhança dos documentos (e.g. 50% de semelhança indicaria plágio).

*/

void result(float *porc,fila *dadoa,fila *dadob){
	FILE *saida;
	char x[20],temp[20];
	x[19]='\0';

	saida=fopen("compararDoc.txt","w");

	fputs("doc1.txt:\n\n\tPalavras\t\tFrequencia\n",saida);
	while(dadoa!=NULL){
		fputs("\t",saida);
		strcpy(temp,dadoa->pal);
		fputs(temp,saida);
		fputs("\t",saida);
		if((strlen(temp))<4){
			fputs("\t\t\t",saida);
		}
		else if((strlen(temp))<8){
			fputs("\t\t",saida);
		}
		else{
			fputs("\t",saida);
		}
		fprintf(saida,"%d",dadoa->c);
		fputs("\n",saida);
		dadoa=(fila *)dadoa->prox;
	}/*fim while*/
	fputs("\n\ndoc2.txt:\n\n\tPalavras\t\tFrequencia\n",saida);
	while(dadob!=NULL){
		fputs("\t",saida);
		strcpy(temp,dadob->pal);
		fputs(temp,saida);
		fputs("\t",saida);
		if((strlen(temp))<4){
			fputs("\t\t\t",saida);
		}
		else if((strlen(temp))<8){
			fputs("\t\t",saida);
		}
		else{
			fputs("\t",saida);
		}
		fprintf(saida,"%d",dadob->c);
		fputs("\n",saida);
		dadob=(fila *)dadob->prox;
	}/*fim while*/
	fprintf(saida,"\n%.2f porcento de igualdade.\n",*porc);
	printf("\nArquivos analisados:\tdoc1.txt\tdoc2.txt\n");
	printf("\n\t%.2f porcento de igualdade.\n",*porc);
	printf("\n\tRESULTADO: Documentos ");
	if(*porc>=50.00){
		printf("plagiados.\n\n");
	}
	else{
		printf("NAO plagiados.\n\n");
	}

	fclose(saida);
}

/*
	========================
	||	checa_quantidade  ||
	========================

	1-Checa a existencia de determinada palavra na fila B;
	2-Retorna a quantidade de vezes em que ela aparece ou 0 (zero) caso a palavra nao exista.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*dados
			-ponteiro para estrutura do tipo "fila"
			++Contem endereco atual do termo que se esta analisando
		pal
			-String
			++Contem palavra que se deseja verificar a quantidade de vezes em que ocorreu 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
int checa_quantidade(fila *dados,char pal[20]){
	while(dados!=NULL){
		if(!strcmp(dados->pal,pal)){
			return dados->c;
		}
		dados=(fila *)dados->prox;
	}
	return 0;
}

/*
	===========================
	||	calcula_porcentagem  ||
	===========================

	Calcula porcentagem de semelhanca entre os textos

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		x
			-float
			++Contem o valor do "angulo" entre os textos
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
float calcula_porcentagem(float x){
	return (100-((200*x)/PI));
}

/*
	=====================
	||	libera_memoria ||
	=====================

	Libera memoria utilizada pelas filas

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Variaveis:
		*texto1 e *texto2
			-ponteiros para estrutura do tipo "fila"
			++Contem endereco atual do termo que se deseja liberar memoria de cada fila
		*prox1 e *prox2
			-ponteiros para estrutura do tipo "fila"
			++Contem endereco do proximo termo da lista
		fim
			-int
			++Flag que indica liberacao de todos elementos da lista
		
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void libera_memoria(fila *texto1,fila *texto2){

	fila *prox1;
	fila *prox2;
	int fim=0;
	texto1=(fila *)texto1->prox;
	texto2=(fila *)texto2->prox;
	prox1=(fila *)texto1->prox;
	prox2=(fila *)texto2->prox;

	while(!fim){
		if(texto1!=NULL){
			free(texto1);
			texto1=prox1;
			if(prox1!=NULL){
				prox1=(fila *)prox1->prox;
			}
		}
		if(texto2!=NULL){
			free(texto2);
			texto2=prox2;
			if(prox2!=NULL){
				prox2=(fila *)prox2->prox;
			}
		}
		if((texto1==NULL) && (texto2==NULL)){
			fim=1;
		}
	}

}















