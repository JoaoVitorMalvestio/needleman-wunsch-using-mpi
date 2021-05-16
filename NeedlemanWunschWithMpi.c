// Marcos Vinicius Peres RA: 94594
// Jo�o Vitor Malvestio da Silva RA: 93089
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#define TAM_MAX 20000
#define ROOT_ID 0

void inicializacao(char primeiraSequencia[], char segundaSequencia[]);
void matrizDeScore(char primeiraSequencia[], char segundaSequencia[], int id);
void printMatriz(char primeiraSequencia[], char segundaSequencia[]);
int MAIOR(int a, int b);

int matriz[TAM_MAX][TAM_MAX];
int match = 1;
int missmatch = -1;
int gap = -1; 

int main(){	
	int id, num_processos, i;

	MPI_Init(NULL, NULL);
	MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processos);

	printf("Processo %d entre os %d\n", id, num_processos);

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

	if (id == ROOT_ID) {
		printf("Primeira Sequencia: %s \n", primeiraSequencia);
    	printf("Segunda Sequencia: %s \n", segundaSequencia);
	}	
	
    inicializacao(primeiraSequencia, segundaSequencia);

	MPI_Barrier(MPI_COMM_WORLD);

    matrizDeScore(primeiraSequencia, segundaSequencia, id);

	if (id == ROOT_ID) {
		printMatriz(primeiraSequencia, segundaSequencia);	
	}

	MPI_Finalize();
    
	return 0;
}

void inicializacao(char primeiraSequencia[], char segundaSequencia[]) {
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

void matrizDeScore(char primeiraSequencia[], char segundaSequencia [], int id) {
    int tamanhoPrimeiraSequencia = strlen(primeiraSequencia);
    int tamanhoSegundaSequencia = strlen(segundaSequencia);
	MPI_Status status;

	int num_processos;

	MPI_Comm_size(MPI_COMM_WORLD, &num_processos);
    
    for (int i = id + 1; i < tamanhoPrimeiraSequencia + 1; i += num_processos) {
        for (int j = 1; j < tamanhoSegundaSequencia + 1; j++) {			
			if (id != ROOT_ID) {
				int linhaRecebida[TAM_MAX];
				MPI_Recv(&linhaRecebida, tamanhoPrimeiraSequencia, MPI_INT, id - 1, i - 1, MPI_COMM_WORLD, &status);				
				memcpy(&matriz[i - 1], &linhaRecebida, sizeof(int)*TAM_MAX);
			}
        
            int valorDiagonal = 0;
			
            if (primeiraSequencia[i-1] == segundaSequencia[j-1]) {
			    valorDiagonal = matriz[i - 1][j - 1] + match;
            } else {
			    valorDiagonal = matriz[i - 1][j - 1] + missmatch;
            }
    
            int valorEsquerda = matriz[i][j - 1] + gap;
            int valorCima =  matriz[i - 1][j] + gap;
            int maximoScore = MAIOR(MAIOR(valorDiagonal, valorEsquerda), valorCima);
            
            matriz[i][j] = maximoScore;

			if (id != num_processos - 1) {
				MPI_Send(&matriz[i], tamanhoPrimeiraSequencia, MPI_INT, id + 1, i, MPI_COMM_WORLD);
			}
        }
		if (id == ROOT_ID) {
			for (int k = 1; k < num_processos; k++) {

				if (i + k < tamanhoPrimeiraSequencia) {
					int linhaRecebida[TAM_MAX];
					MPI_Recv(&linhaRecebida, tamanhoPrimeiraSequencia + 1, MPI_INT, id + k, i + k, MPI_COMM_WORLD, &status);					
					memcpy(&matriz[i + k], &linhaRecebida, sizeof(int)*TAM_MAX);					
				}
			}
		} else {
			MPI_Send(&matriz[i], tamanhoPrimeiraSequencia + 1, MPI_INT, ROOT_ID, i, MPI_COMM_WORLD);
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
	printf("\n");
}
