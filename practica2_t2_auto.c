//TAREA 2 con automatizadores

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define N 1200

double **crea_matriz()
{
	int i;
	double **matriz = (double**)malloc(N*sizeof(double*));
	for(i=0;i<N;i++){
		matriz[i] = (double*)malloc(N*sizeof(double));
	}
	return matriz;
}


//Esta función da los valores especificadados en la tarea, la diagonal se rellena con ceros, la diagonal superior con valores positivo entre (valor 0< alpha <1) y la diagonal inferior con valores negativos (valor 0< alpha <1) 
void inicializa_matriz(double **matriz)
{
	int i,j;
	for(i=0;i<N;i++){
		matriz[i][i]=0;
		for(j=i+1;j<N;j++){
			matriz[i][j] = (double) (rand()%(RAND_MAX-1)+1) / (double)RAND_MAX;
			matriz[j][i] = -((double) (rand()%(RAND_MAX-1)+1) / (double)RAND_MAX);
			
		}
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


// Esta función inicializa el vector que es pasado por referencia
void inicializa_vector(double *vector)
{
	int i;
	for(i=0;i<N;i++){
		vector[i] = 1.0;
	}
}



//Esta función sirve para hacer un cast de un double para escribirlo en el archivo
void cast_double(char cadena[],double **matriz){
	
	int posicion=0;
	double numero=0;
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			numero = matriz[i][j];
		//	printf(" %.3f ",numero);
			cadena[posicion] = (char)numero;
			posicion++;
		}
	}
}

//Esta fución guarda la matriz en un fichero .bin
void guardar_matriz(char *filename, FILE *archivo,char cadena[])
{
	archivo = fopen("matriz.bin","w+");
	int k;
	for(k=0;k<N*N;k++){
		fputc(cadena[k],archivo);
	}
	fclose(archivo);
}

//Esta función imprime la matriz que se pase por referencia, con los valores de filas y columnas
void imprime_matriz(double **matriz, int filas, int columnas)
{
	int i,j;
	for(i=0;i<filas;i++){
		for(j=0;j<columnas;j++){
			printf("%f\t",matriz[i][j]);
		}
		printf("\n");
	}
	
}


//Esta función imprime el vector que se pasase por referencia
void imprime_vector(double *vector)
{
	int i;
	for(i=0;i<N;i++){
		printf("%.2f\t ",vector[i]);
	}
	printf("\n");
	
}

// Libera la memoria de una matriz
void libera_matriz(double **matriz)
{
	int i;
	for (i=0;i<N;i++) {
		free(matriz[i]);
	}
	free(matriz);
}


int main(int argc,char *argv[])
{
	int i,j,iter,limite_inferior,limite_superior,filas_por_proceso;
	int m = 5;
	double alpha = 0.98;
	double **matriz;
	double *vector, *vaux;
	srand(time(NULL));
	double norma_euclidea[2];
	int nproces = 4;
	filas_por_proceso = N/nproces;
		
	//m = atoi(argv[1]);
	//alpha = atof(argv[2]);
	
	matriz = crea_matriz();
	//inicializa_matriz(matriz);
	inicializa_matriz_comprobacion(matriz);
	vector = (double*)malloc(N*sizeof(double));
	vaux = (double*)malloc(N*sizeof(double));
	inicializa_vector(vector);
	
	//imprime_matriz(matriz,N,N);
	//printf("\n");
	//printf("\nVector: \n");
	//imprime_vector(vector);
	
	for(i=0;i<N;i++){
		vaux[i] = 0;
	}
	
	//EMPIEZA EL BUCLE DE ITERACIONES
	for(iter=1; iter<=m; iter++){
		#pragma omp parallel num_threads(nproces) shared(norma_euclidea,alpha,filas_por_proceso,nproces,vector,vaux,matriz,iter) private(i,j)
		
		#pragma omp single
		norma_euclidea[iter%2] = 0;
		
		#pragma omp for schedule(static,filas_por_proceso) 
		for(i=0;i<N;i++){
			vaux[i] = 0;
			for(j=0;j<N;j++){
				vaux[i] += matriz[i][j] * vector[j];	
			}
			norma_euclidea[iter%2] += vaux[i] * vaux[i];
		}
		
		#pragma omp barrier
		
		#pragma omp single
		norma_euclidea[iter%2] = sqrt(norma_euclidea[iter%2]);
		
				
		if(iter % 2 == 0){
			#pragma omp for schedule(static,filas_por_proceso)
			for(i=0; i<N; i++){
				vaux[i] /= norma_euclidea[iter%2];
				vector[i] /= norma_euclidea[iter % 2 == 0 ? 1 : 0];
				vaux[i] = (alpha * vaux[i]) -((1-alpha*alpha) * vector[i]); 
			}
		}
		
		#pragma omp barrier
		
		for(i=0;i<N;i++){
			vector[i] = vaux[i];
		}
		
		#pragma omp master
		if (iter==m) {
			printf("NORMA FINAL: ");
		} else {
			printf("NORMA: ");
		}
		printf("%f\n",norma_euclidea[iter%2]);
		
	}//FOR iteraciones
	
	
	libera_matriz(matriz);
	free(vector);
	free(vaux);
	return 0;
}















































