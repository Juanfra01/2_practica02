#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>


#define N 512

//Esta función se encarga de reservar memoria para la matriz
int **crea_matriz()
{
	int i;
	int **matriz;
	matriz = (int**)malloc(N*sizeof(int*));
	for(i=0;i<N;i++){
		matriz[i] = (int*)malloc(N*sizeof(int));
	}
	return matriz;
}


// Esta función pasa los valores de un vector a una estructura de matriz
void inicializar_matriz(int **matriz,int vector_aux[])
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


//Esta función pasa los valores enteros de la matriz a char
void cast_entero(int **matriz,char cadena[]){
	int posicion=0;
	int numero=0;
	
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			numero = matriz[i][j];
			cadena[posicion] = (char)numero;
			posicion++;
		}
	}
}

//Esta función guarda la matriz en un fichero .bin
void guardar_matriz(char *fileName,char cadena[], FILE *archivo)
{
	archivo = fopen("foto_filtrada.raw","wb");
	int i;
	for(i=0;i<N*N;i++)
	{
		fputc(cadena[i],archivo);
	}
	fclose(archivo);
}


void filtrado_sin_automatizadores(int F,int C,int nproces,int **matriz,int **matriz2,int ind_fila[],int ind_colum[],int paumentados,int pdisminuidos,int piguales)
{
	int i,j,k,myid,promedio;
	int elementos_por_proceso = F/nproces;
	int vector_ordenar[15];
	
	
	#pragma omp parallel num_threads(nproces) shared(matriz,matriz2,F,C,ind_fila,ind_colum,paumentados,pdisminuidos,piguales) private(i,j,k,myid,promedio,elementos_por_proceso)
	{
		//np = omp_get_num_threads();
		myid = omp_get_thread_num();
		int x = myid*(F/nproces);
		
		
		if(myid == nproces-1){
			elementos_por_proceso = F -((nproces-1)* elementos_por_proceso);
		}
		
		for(i=0;i<elementos_por_proceso;i++){
			for(j=0;j<C;j++){
				
			}
		}
	}//FIN PRAGMA
	
		
}




void liberar_memoria(int **matriz)
{
	int i;
	
	for(i=0;i<N;i++){
		free(matriz[i]);
	}
	free(matriz);
}

int main(int argc,char *argv[])
{
	srand(time(NULL));
	FILE *archivo,*fp;
	int **matriz,**matriz2;
	int vector_aux[N*N];
	char cadena[N*N];
	char caracter;
	int valor,i,j,k;
	int incremento = 0;
	int vector_ordenar[15];
	int ind_fila[15]={-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1};
	int ind_colum[15]={-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2};
	int promedio;
	int paumentados = 0;
	int pdisminuidos = 0;
	int piguales = 0;
	int nproces;
	int prueba;
	
	if(argc!=3){
		printf("Error: falta los argumentos correspondientes: nproces lena512x512.raw\n");
		return 1;
	}
	
	archivo = fopen(argv[2],"rb");
	nproces = atoi(argv[1]);
	
	while((caracter = fgetc(archivo)) !=EOF)
	{
		valor = (int)caracter;
		vector_aux[incremento] = valor;
		//printf(" %d ",valor);	
		incremento++;
	}
	fclose(archivo);
		
	matriz = crea_matriz();
	matriz2 = crea_matriz();
	inicializar_matriz(matriz,vector_aux);
	
	filtrado_sin_automatizadores(N,N,nproces,matriz,matriz2,ind_fila,ind_colum,paumentados,pdisminuidos,piguales);
	scanf("%d",&prueba);
	
	
	archivo = fopen("foto_filtrada_pra.raw","wb");
	cast_entero(matriz2,cadena);
	guardar_matriz("matriz.bin",cadena,archivo);
	
	return 0;
}























