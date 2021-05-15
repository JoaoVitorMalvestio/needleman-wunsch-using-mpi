// Marcos Vinicius Peres RA: 94594
// Jo�o Vitor Malvestio da Silva RA: 93089
#include <stdio.h>
#include <string.h>
#define TAM_MAX 20000

void inicializacao(char primeiraSequencia[], char segundaSequencia[]);
void matrizDeScore(char primeiraSequencia[], char segundaSequencia[]);
void printMatriz(char primeiraSequencia[], char segundaSequencia[]);
int MAIOR(int a, int b);

int matriz[TAM_MAX][TAM_MAX];
int match = 1;
int missmatch = -1;
int gap = -1; 

int main(){	
	FILE *arq;
	char primeiraSequencia[TAM_MAX]; 
    char segundaSequencia[TAM_MAX];
    
	arq = fopen("./input1.txt", "rt");
	if (arq == NULL) {
		printf("Problemas na abertura do arquivo\n");
	 	return 0;
	}
	
	while (!feof(arq)){	  
  		fgets(primeiraSequencia, TAM_MAX, arq);
  		primeiraSequencia[strcspn(primeiraSequencia, "\n")] = 0;	  
	}
	
	fclose(arq);
	
	arq = fopen("./input2.txt", "rt");
	if (arq == NULL) {
		printf("Problemas na abertura do arquivo\n");
	 	return 0;
	}
	
	while (!feof(arq)){	  
  		fgets(segundaSequencia, TAM_MAX, arq);
	  	segundaSequencia[strcspn(segundaSequencia, "\n")] = 0;
	}
	
	fclose(arq);	
	
	printf("Primeira Sequencia: %s \n", primeiraSequencia);    
    printf("Segunda Sequencia: %s \n", segundaSequencia);
    	
    inicializacao(primeiraSequencia, segundaSequencia);
    matrizDeScore(primeiraSequencia, segundaSequencia);
    //printMatriz(primeiraSequencia, segundaSequencia);
    
	return 0;
}

void inicializacao(char primeiraSequencia[], char segundaSequencia[]){
    int tamanhoPrimeiraSequencia = strlen(primeiraSequencia);
    int tamanhoSegundaSequencia = strlen(segundaSequencia);
    
	matriz[0][0] = 0;
    
    for (int i = 0; i < tamanhoPrimeiraSequencia +  1; i++) {    	
        matriz[i][0] = i == 0 ? 0 : matriz[i-1][0] + (gap); 
    }

    for (int j = 0; j < tamanhoSegundaSequencia + 1; j++) {    	
        matriz[0][j] = j == 0 ? 0 : matriz[0][j-1] + (gap);
    }
}

void matrizDeScore(char primeiraSequencia[], char segundaSequencia []){
    int tamanhoPrimeiraSequencia = strlen(primeiraSequencia);
    int tamanhoSegundaSequencia = strlen(segundaSequencia);

    
    for (int i = 1; i < tamanhoPrimeiraSequencia + 1; i++) {
        for (int j = 1; j < tamanhoSegundaSequencia + 1; j++) {
        
            int valorDiagonal = 0;
			
            if (primeiraSequencia[i-1] == segundaSequencia[j-1]) {
			    valorDiagonal = matriz[i - 1][j - 1] + match;
            }else{
			    valorDiagonal = matriz[i - 1][j - 1] + missmatch;
            }
    
            int valorEsquerda = matriz[i][j - 1] + gap;
            int valorCima =  matriz[i - 1][j] + gap;
            int maximoScore = MAIOR(MAIOR(valorDiagonal, valorEsquerda), valorCima);
            
            matriz[i][j] = maximoScore;
        }
    }
}

int MAIOR (int a, int b) {
	if (a > b) {
		return a;
	}
	
	return b;
}

void printMatriz(char primeiraSequencia[], char segundaSequencia []){
    int tamanhoPrimeiraSequencia = strlen(primeiraSequencia);
    int tamanhoSegundaSequencia = strlen(segundaSequencia);
    
    printf("\t\t");
    for (int i = 0; i < tamanhoSegundaSequencia; i++) {		    
		printf("%c\t", segundaSequencia[i]);
	}
    
    for (int i = 0; i < tamanhoPrimeiraSequencia + 1; i++){
        printf("\n");
        
        if (i > 0) {
        	printf("%c\t", primeiraSequencia[i - 1]);	
		} else {
			printf("\t");
		}
		
        for (int j = 0; j < tamanhoSegundaSequencia + 1; j++){
        	if (matriz[i][j] < 0) {
        		printf("%d\t",matriz[i][j]);	
			} else {
				printf(" %d\t",matriz[i][j]);
			}
            
        }

    }

}
