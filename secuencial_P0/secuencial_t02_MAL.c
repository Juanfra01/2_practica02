/*Este código no filtra bien porque el tipo de dato de la matriz debe de ser unsigned char.
 *Con lo cual este código no se adecúa a lo que se pide en la práctica
 * 
 * */

//TAREA 2 SISTEMA ITERATIVO
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define N 1200


//Esta función reserva memoria para la matriz
double **crea_matriz()
{
	int i;
	double **matriz;
	matriz = (double**)malloc(N*sizeof(double*));
	for(i=0;i<N;i++){
		matriz[i] = (double*)malloc(N*sizeof(double));
	}
	return matriz;
}


void inicializa_matriz(double **matriz)
{
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
		//	matriz[i][j] = (double) (rand()%(RAND_MAX-1)+1) / (double)RAND_MAX;
		//	matriz[j][i] = -((double) (rand()%(RAND_MAX-1)+1) / (double)RAND_MAX);
			matriz[i][j] = rand()%11;
			
		}
	}
}

// Esta función inicializa el vector que es pasado por referencia
void inicializa_vector(double *vector)
{
	int i;
	for(i=0;i<N;i++){
		vector[i] = 1.0;
	}
}



//Está función inicializa la matriz para que sean comprobados los resultados
void inicializa_matriz_comprobacion(double **matriz)
{
	int i,j;
	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) {
			if (i==j) {
				matriz[i][j]=0.0;
			} else if (i<j) {
				matriz[i][j]=(double)0.000000001*(i+1)*(j+1);
			} else {
				matriz[i][j]=(double)0.000000001*(i+1)*(j+1);
			}
		}
	}
}

//Esta función imprime la matriz que se pase por referencia, con los valores de filas y columnas
void imprime_matriz(double **matriz, int filas, int columnas)
{
	int i,j;
	for(i=0;i<filas;i++){
		for(j=0;j<columnas;j++){
			printf("%.2f\t",matriz[i][j]);
		}
		printf("\n");
	}
	
}


//Esta función imprime el vector que se pasase por referencia
void imprime_vector(double *vector)
{
	int i;
	for(i=0;i<N;i++){
		printf("%.2f \t",vector[i]);
	}
	printf("\n");
	
}


int main(int argc,char *argv[])
{
	srand(time(NULL));
	double **matriz;
	double *vector,*vaux;
	double norma_euclidea[2];
	double norma_euclidea_parcial[2];
	int m = 5;
	double alpha = 0.98;
	int i,j,iter;
	
	//reservamos memoria
	matriz = crea_matriz();
	vector = (double*)malloc(N*sizeof(double));
	vaux = (double*)malloc(N*sizeof(double));
	//iniclializamos matriz y vectores
	inicializa_matriz_comprobacion(matriz);
	//inicializa_matriz(matriz);
	inicializa_vector(vector);
	
	
//	imprime_matriz(matriz,N,N);
	printf("\n");
	//imprime_vector(vector);
	//printf("\n");
	//imprime_vector(vaux);
	
	for(iter=1;iter<=m;iter++){
		//multiplicación
		norma_euclidea[iter%2] = 0;
		for(i=0;i<N;i++){
			vaux[i]=0;
			for(j=0;j<N;j++){
				vaux[i] += matriz[i][j] * vector[j];
			}
			norma_euclidea[iter%2]+= vaux[i] * vaux[i];
		}
		
		norma_euclidea[iter%2] = sqrt(norma_euclidea[iter%2]);
	
	
		if(iter % 2 == 0){
			for(i=0;i<N;i++){
				vaux[i] /= norma_euclidea[iter%2];
				vector[i] /= norma_euclidea[iter % 2 == 0 ? 1 : 0];
				vaux[i] = alpha * vaux[i] - (1 - alpha*alpha) * vector[i];
			}
		}
	
		//copiamos vaux en vector
		for(i=0;i<N;i++){
			vector[i] = vaux[i];
		}
		//COMPROBACIONES
	//	imprime_vector(vaux);
	//	printf("\n");
		printf("\nNorma euclidea %f \n",norma_euclidea[iter%2]);

	}//FIN iter
	
//	printf("\n");
//	imprime_vector(vector);
	
	for(i=0;i<N;i++){
		free(matriz[i]);
	}
	free(matriz);
	free(vector);
	free(vaux);
	
	return 0;
}













