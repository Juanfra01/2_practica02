//TAREA 2 
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
void guardar_matriz(FILE *archivo,char cadena[])
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


void tarea2_opcion1_con_automatizadores(int nproces,double **matriz,double alpha,double *vector,double *vaux,int m)
{
	FILE *fp;
	int i,j,iter;
	int filas_por_proceso = N/nproces;
	double norma_euclidea[2];
	double tini = 0, tfin = 0;
	
	#pragma omp master
	tini = omp_get_wtime();
	
	
	//EMPIEZA EL BUCLE DE ITERACIONES
	for(iter=1; iter<=m; iter++){
		#pragma omp parallel num_threads(nproces) shared(norma_euclidea,alpha,filas_por_proceso,nproces,vector,vaux,matriz,iter) private(i,j,tini,tfin)
		
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
	
	#pragma omp master
	{
		fp = fopen("tiempo_t2_opc1_auto.txt","w+");
		tfin = omp_get_wtime();
		printf("\nEl tiempo tarea 2 opcion 1 automatizadores es de: %f con %d procesos\n",(tfin-tini),nproces);
		fprintf(fp,"\nEl tiempo tarea 2 opcion 1 automatizadores es de: %f con %d procesos\n",(tfin-tini),nproces);
		fclose(fp);
	}
}


void tarea2_opcion1_sin_automatizadores(int nproces,double alpha,double **matriz,double *vector,double *vaux,int m)
{
	FILE *fp;
	int i,j,iter,myid,limite_inferior,limite_superior;
	double norma_euclidea[2];
	double suma_parcial[2];
	int filas_por_proceso = N/nproces; 
	double tini = 0, tfin = 0;
	
	#pragma omp master
	tini = omp_get_wtime();
	
	
	for(iter=1; iter<=m; iter++){
		#pragma omp parallel num_threads(nproces) shared(norma_euclidea,alpha,filas_por_proceso,nproces,matriz,vector,vaux,iter) private(i,j,myid,limite_inferior,limite_superior,suma_parcial,tini,tfin)
		{
			myid = omp_get_thread_num();
			limite_inferior = myid*filas_por_proceso;
			limite_superior = (myid == nproces-1) ? N : (myid+1)*filas_por_proceso;
			suma_parcial[iter%2] = 0;
			
			//Multiplicacion de la matriz por el vector
			for(i=limite_inferior;i<limite_superior;i++){
				vaux[i] = 0;
				for(j=0;j<N;j++){
					vaux[i] += matriz[i][j] * vector[j];	
				}
				//norma_euclidea[iter%2] += vaux[i] * vaux[i];
				suma_parcial[iter%2] += vaux[i] * vaux[i];
			}
			
			#pragma omp single
			norma_euclidea[iter%2] = 0;
			
			#pragma omp critical
			norma_euclidea[iter%2] += suma_parcial[iter%2];
			
			#pragma omp barrier
			#pragma omp single
			norma_euclidea[iter%2] = sqrt(norma_euclidea[iter%2]);
					
			//Normalizamos
			if(iter % 2 == 0){
				for(i=limite_inferior; i<limite_superior; i++){
					vaux[i] /= norma_euclidea[iter%2];
					vector[i] /= norma_euclidea[iter % 2 == 0 ? 1 : 0];
					vaux[i] = (alpha * vaux[i]) -((1-alpha*alpha) * vector[i]); 
				}
			}
			
				
			#pragma omp barrier
			for(i=0;i<N;i++){
				vector[i] = vaux[i];
			}
			
			#pragma omp barrier
			#pragma omp master
			{
				if (iter==m) {
					printf("NORMA FINAL: ");
				} else {
					printf("NORMA: ");
				}
				printf("%f\n",norma_euclidea[iter%2]);
			}
		}//FIN pragma
	}//Fin for iteraciones
	#pragma omp master
	{
		fp = fopen("tiempo_t2_opc1__sin_auto.txt","w+");
		tfin = omp_get_wtime();
		printf("\nEl tiempo tarea 2 opcion 1 sin automatizadores es de: %f con %d procesos\n",(tfin-tini),nproces);
		fprintf(fp,"\nEl tiempo tarea 2 opcion 1 sin automatizadores es de: %f con %d procesos\n",(tfin-tini),nproces);
		fclose(fp);
	}
}


void tarea2_opcion2_con_automatizadores(int nproces,double **matriz,double alpha,double *vector,double *vaux,int m)
{
	FILE *fp;
	int i,j,iter;
	int filas_por_proceso = N/nproces;
	double norma_euclidea[2];
	double tini = 0, tfin = 0;
	
	#pragma omp master
	tini = omp_get_wtime();
	
	//EMPIEZA EL BUCLE DE ITERACIONES
	for(iter=1; iter<=m; iter++){
		#pragma omp parallel num_threads(nproces) shared(norma_euclidea,alpha,filas_por_proceso,nproces,vector,vaux,matriz,iter) private(i,j,tini,tfin)
		
		#pragma omp single
		norma_euclidea[iter%2] = 0;
		
		#pragma omp for schedule(static,350) 
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
	
	#pragma omp master
	{
		fp = fopen("tiempo_t2_opc2_auto.txt","w+");
		tfin = omp_get_wtime();
		printf("\nEl tiempo tarea 2 opcion 2 automatizadores es de: %f con %d procesos\n",(tfin-tini),nproces);
		fprintf(fp,"\nEl tiempo tarea 2 opcion 2 automatizadores es de: %f con %d procesos\n",(tfin-tini),nproces);
		fclose(fp);
	}
}



int main(int argc,char *argv[])
{
	FILE *archivo;
	char cadena[N*N];
	int i,j,m,nproces,opcion;
	double alpha;
	double **matriz;
	double *vector, *vaux;
	srand(time(NULL));
		
	
	if(argc!=5){
		printf("Error, el formato es nproces m alpha opcion\n");
		printf("\nSistema Iterativo asignando un bloque de filas a cada proceso con automatizadores. Opcion = 1\n");
		printf("\nSistema Iterativo asignando un bloque de filas a cada proceso sin automatizadores. Opcion = 2\n");
		return 1;
	}
		
	nproces = atoi(argv[1]);
	m = atoi(argv[2]);
	alpha = atof(argv[3]);
	opcion = atoi(argv[4]);
	
	matriz = crea_matriz();
	//inicializa_matriz(matriz);
	inicializa_matriz_comprobacion(matriz);
	vector = (double*)malloc(N*sizeof(double));
	vaux = (double*)calloc(N,sizeof(double));
	inicializa_vector(vector);
	
	//imprime_matriz(matriz,N,N);
	//printf("\n");
	//printf("\nVector: \n");
	//imprime_vector(vector);

	
	if(opcion == 1){
		printf("Opcion 1 con automatizadores\n");
		tarea2_opcion1_con_automatizadores(nproces,matriz,alpha,vector,vaux,m);
	}else if(opcion == 2){
		printf("\nOpcion 1 sin automatizadores\n");
		tarea2_opcion1_sin_automatizadores(nproces,alpha,matriz,vector,vaux,m);
	}else if(opcion == 3){
		printf("\nOpcion 2 con automatizadores\n");
		tarea2_opcion2_con_automatizadores(nproces,matriz,alpha,vector,vaux,m);
	}
	
	cast_double(cadena,matriz);
	guardar_matriz(archivo,cadena);
	
	
	libera_matriz(matriz);
	free(vector);
	free(vaux);
	return 0;
}

/*
 * Para m = 5, alpha = 0.98 
Norma: 1.728359e+01
Norma: 9.952852e+00
Norma: 5.415347e-01
Norma: 3.118459e-01
Norma final: 5.415347e-01*/
 








