#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


void pontilhado() {
        printf("\n-----------------------------------\n");
}

int calcDimensao(char *nome) { // Esta função calcula a quantidade de variáveis contando os espaços em branco da primeira linha da matriz
	int i, tam = 0, tam_linha;
	char linha_matriz[101];
	FILE *fq = fopen(nome, "r");
	
	fgets(linha_matriz,101,fq); // lê a primeira linha da matriz
	tam_linha = strlen(linha_matriz); // calcula o tamanho da linha
	
	for(i = 0; i < tam_linha; i++) {
		if(linha_matriz[i] == ' ') // conta os espaços em branco e acrescenta em tam
			tam++;
	}
 	
        fclose(fq);
        return(tam);
}
 	
double **ler(char *nome_arqu_matriz, int *dim) {
        int i, j;
        double **M, **aux, num;
        FILE *fp = fopen(nome_arqu_matriz, "r");
        
        *dim = calcDimensao(nome_arqu_matriz); // calcula a dimensão
        
        M = malloc(*dim*sizeof(double*)); // aloca memoria para as linhas da matriz
        aux = M;       
       
        for(i = 0; i < *dim; i++) {
        	*aux = malloc((*dim+1)*sizeof(double)); // aloca memoria para as colunas da linha i da matriz
        	
        	for(j = 0; j <= *dim; j++)
        		fscanf(fp,"%lf", &M[i][j]);
        	        	
        	aux++;
        }	
        
       fclose(fp);
       return(M);
}                               

void imprime(double **M, int dim) {
        int i, j;
        
        pontilhado(); // função que torna mais apresentável os resultados na tela
        for(i = 0; i < dim; i++) {
                for(j = 0; j < (dim+1); j++) {
                        printf("%.2lf\t", M[i][j]);
                }
                printf("\n");
        }
        pontilhado();
}

void pivoteamento(double **M, int k, int dim) {
	int i;
	double maior = M[k][k], *aux_linha; // a variável maior guarda o elemento da diagonal principal que a principio utilizaríamos
	// para zerar os elementos da mesma coluna nas linhas inferiores, a linha auxiliar será utilizada na hora de trocar as linhas da
	// matriz, caso o maior seja superado por outro elemento abaixo de sua coluna
	
	for(i = k; i < dim; i++) {
		if(fabs(maior) < fabs(M[i][k])) {
			maior = M[i][k];
			
			aux_linha = M[i];
			M[i] = M[k];
			M[k] = aux_linha;
		}
	}
}
  			
void triangsup(double **M, int dim) {
        int i, j, k=0;
        double lambda;
        
        for(k = 0; k < (dim-1); k++) // k é a linha que nos estamos trabalhando
        	for(i = (k+1); i < dim; i++) { // i são as linhas abaixo de k onde iremos operar
        		pivoteamento(M,k, dim);
        		
        		(M[k][k] != 0)? (lambda = (-M[i][k]/M[k][k])) : (lambda = 0); // lambda é a constante que iremos
        		// multiplicar os elementos da linha k e somar com os respectivos elementos da linha i para o escalonamento
        	
        		for(j = 0; j <= dim; j++) // aqui faz a operação na linha i
        			M[i][j] += lambda*M[k][j];  
              	} 
}


void subsreversa(double **M,double *raizes, int dim) {
	int i, j;
	double somatorio=0;
	
	for(i = (dim-1); i >= 0; i--) {
		for(j = (i+1); j < dim; j++)
			somatorio += M[i][j]*raizes[j];
		raizes[i] = (M[i][dim] - somatorio)/M[i][i];

		somatorio = 0;
	}
}


int main(int argc, char **argv) {
	double **M; 
	double *raizes;
	int i, dim = 0;
	
	M = ler(argv[1], &dim);
	
	
	imprime(M,dim);
	triangsup(M,dim);
	imprime(M,dim);
	
	raizes = malloc(dim*sizeof(double));
	subsreversa(M,raizes,dim);
	
	
	for(i = 0; i < dim; i++)
		printf("x%1d = %5.3lf\n", i, raizes[i]);
	
	return(0);
}

