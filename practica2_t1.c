/* practica 2 tarea 1 con automatizadores*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#define N 512


//Esta función se encarga de reservar memoria para la matriz
unsigned char **crea_matriz()
{
	int i;
	unsigned char **matriz;
	matriz = (unsigned char**)malloc(N*sizeof(unsigned char*));
	for(i=0;i<N;i++){
		matriz[i] = (unsigned char*)malloc(N*sizeof(unsigned char));
	}
	return matriz;
}


// Esta función pasa los valores de un vector a una estructura de matriz
void inicializar_matriz(unsigned char **matriz,unsigned char vector_aux[])
{
	int i,j;
	int incremento=0;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			matriz[i][j] = vector_aux[incremento];
			incremento++;
		}
	}
}

//Forma parte del argoritmo de ordenación Merge Sort
void merge(int vector[],int l,  int m, int r)
{
	int i,j,k;
	int n1 = m - l +1;
	int n2 = r - m;
	
	//creamos vectores temporales
	int L[n1], R[n2];
	
	//copiamos datos en los vectores temporales
	for(i=0;i<n1;i++){
		L[i] = vector[l+i];
	}
	for(j=0;j<n2;j++){
		R[j] = vector[m + 1 + j];
	}
	
	i = 0;
	j = 0;
	k = l;
	while(i < n1 && j < n2){
		if(L[i] <= R[j]){
			vector[k] = L[i];
			i++;
		}else{
			vector[k] = R[j];
			j++;
		}
		k++;
	}
	
	//Copiamos los elementos restantes de L[], si los hubiera
	while(i < n1){
		vector[k] = L[i];
		i++;
		k++;
	}
	//Copiamos los elementos restantes de R[], si los hubiera
	while(j < n2){
		vector[k] = R[j];
		j++;
		k++;
	}
}


//Algoritmo de ordenación Merge Sort
void mergeSort(int vector[],int l, int r)
{
	int m;
	if(l < r){
		m = (l+r)/2;
		mergeSort(vector,l,m);
		mergeSort(vector,m+1,r);
		
		merge(vector,l,m,r);
	}
}


//Esta función guarda la matriz en un fichero .bin
void guardar_matriz(char *fileName,unsigned char **matriz2, FILE *archivo)
{
	archivo = fopen(fileName,"wb");
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			fputc(matriz2[i][j],archivo);
		}
	}
	fclose(archivo);
}


void liberar_memoria(unsigned char **matriz)
{
	int i;
	
	for(i=0;i<N;i++){
		free(matriz[i]);
	}
	free(matriz);
}


void tarea1_con_automatizadores(unsigned char **matriz,unsigned char **matriz2,int ind_fila[],int ind_colum[],int nproces,FILE *archivo)
{
	
	int i,j,k,promedio;
	int paumentados=0;
	int pdisminuidos=0;
	int piguales=0;
	int elementos_por_proceso = N/nproces;
	int vector_ordenar[15];
	
	
	#pragma omp parallel for num_threads(nproces) shared(matriz,matriz2,i,ind_fila,ind_colum) private(k,j,vector_ordenar,promedio) reduction(+:paumentados) reduction(+:pdisminuidos) reduction(+:piguales)
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(i==0 || i==(N-1) || j==0 || j==1 || j==(N-1) || j==(N-2)){
					matriz2[i][j] = matriz[i][j];
			}else{
				for(k=0;k<15;k++){
					vector_ordenar[k] = matriz[i+ind_fila[k]][j+ind_colum[k]];
				}
				
				//función de ordenación ascendente 
				//MEDIANA
				mergeSort(vector_ordenar,0,14);
				
				//PROMEDIO
				promedio = (vector_ordenar[6] + vector_ordenar[7] + vector_ordenar[8])/3;
				
				if(promedio > matriz[i][j]){
					paumentados++;
				}else if(promedio < matriz[i][j]){
					pdisminuidos++;
				}else{
					piguales++;
				}
				
				matriz2[i][j] = promedio;
				
			}//fin else
		}
	}
	
	
	printf("Pixeles aumentados: %d ",paumentados);
	printf("\nPixeles disminuidos: %d ",pdisminuidos);
	printf("\nPixeles iguales: %d \n",piguales);
	
	guardar_matriz("filtro_con_automatizadores.raw",matriz2,archivo);

}


void tarea1_sin_automatizadores(unsigned char **matriz,unsigned char **matriz2,int ind_fila[],int ind_colum[],int nproces,FILE *archivo)
{
	int i,j,k,promedio,paumentados,pdisminuidos,piguales,auxmas,auxmenos,auxigual;
	int limite_inferior,limite_superior,myid;
	int filas_por_proceso = N/nproces;
	int vector_ordenar[15];
	
	
	#pragma omp parallel num_threads(nproces) shared(matriz,matriz2,ind_fila,ind_colum,nproces,filas_por_proceso) private(i,j,k,vector_ordenar,promedio,myid,limite_inferior,limite_superior,auxmas,auxmenos,auxigual)
	{
		myid = omp_get_thread_num();
		limite_inferior = myid*filas_por_proceso;
		limite_superior = (myid == nproces-1) ? N : (myid+1)*filas_por_proceso;
		auxmas = 0;
		auxmenos = 0;
		auxigual = 0;
		
		printf("\nSoy el proceso %d de %d procesos  LI %d   LS %d \n",myid,nproces,limite_inferior,limite_superior);
		
		for(i=limite_inferior;i<limite_superior;i++){
			for(j=0;j<N;j++){
				if(i==0 || i==(N-1) || j==0 || j==1 || j==(N-1) || j==(N-2)){
						matriz2[i][j] = matriz[i][j];
				}else{
					for(k=0;k<15;k++){
						vector_ordenar[k] = matriz[i+ind_fila[k]][j+ind_colum[k]];
					}
					
					//función de ordenación ascendente 
					//MEDIANA
					mergeSort(vector_ordenar,0,14);
					
					//PROMEDIO
					promedio = (vector_ordenar[6] + vector_ordenar[7] + vector_ordenar[8])/3;
					
					if(promedio > matriz[i][j]){
						auxmas++;
					}else if(promedio < matriz[i][j]){
						auxmenos++;
					}else{
						auxigual++;
					}
					
					matriz2[i][j] = promedio;
				}//fin else
			}
			
			#pragma omp single
			{
				paumentados = 0;
				pdisminuidos = 0;
				piguales = 0;
			}
		}

		#pragma omp critical
		{
			paumentados += auxmas;
			pdisminuidos += auxmenos;
			piguales += auxigual;
		}
		
	}//Fin pragma
	
	printf("Pixeles aumentados: %d ",paumentados);
	printf("\nPixeles disminuidos: %d ",pdisminuidos);
	printf("\nPixeles iguales: %d \n",piguales);
	
	guardar_matriz("filtro_sin_automatizadores.raw",matriz2,archivo);
}



int main(int argc,char *argv[])
{
	srand(time(NULL));
	FILE *archivo,*fp;
	unsigned char **matriz,**matriz2;
	unsigned char vector_aux[N*N];
	char caracter;
	unsigned char valor;
	int i,j,k;
	int incremento = 0;
	int ind_fila[15]={-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1};
	int ind_colum[15]={-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2};
	int nproces;
	
	if(argc!=3){
		printf("Error: falta el argumento correspondiente lena512x512.raw nproces\n");
		return 1;
	}
	
	archivo = fopen(argv[1],"rb");
	nproces = atoi(argv[2]);

	
	while((caracter = fgetc(archivo)) !=EOF)
	{
		vector_aux[incremento] = caracter;
		//printf(" %d ",valor);	
		incremento++;
	}
	fclose(archivo);
		
	matriz = crea_matriz();
	matriz2 = crea_matriz();
	inicializar_matriz(matriz,vector_aux);

	
//	tarea1_con_automatizadores(matriz,matriz2,ind_fila,ind_colum,nproces,archivo);
	tarea1_sin_automatizadores(matriz,matriz2,ind_fila,ind_colum,nproces,archivo);
	
	
	liberar_memoria(matriz);
	liberar_memoria(matriz2);
	
	return 0;
}

/*
Pixeles aumentados:90800
Pixeles disminuidos: 117279
Pixeles iguales: 51001
* */



