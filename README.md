 
 PRACTICA 2 

El objetivo de esta práctica es aprender a programar con el paradigma OpenMP para sistemas de memoria
compartida. Se deben realizar dos tareas y analizar su comportamiento mediante ejecuciones de prueba.
Los programas a realizar deben implementar las siguientes operaciones en una arquitectura paralela de
memoria compartida.
 
Tarea 1. Filtrado de imagen por mediana y promedio 

Se trabajará con una imagen en formato raw en escala de grises (cada pixel un dato “unsigned char”).
Las imágenes serán de tamaño 4096x4096. Se guardará en fichero la imagen filtrada. La reserva de
memoria para almacenar la imagen es libre.

Filtrado por mediana y promedio: se cogerán un total de 15 pixeles (3 filas x 5 columnas) alrededor
del que se va a filtrar (incluido el que se va a filtrar), se ordenan de menor a mayor y el nuevo pixel
filtrado es el promedio de los 3 elementos centrales. En caso de no estar disponibles los 15 pixeles no
se modifica el pixel. (CON o SIN AUTOMATIZADORES)

Los nombres de los ficheros serán pasados como argumentos (2 de imagen y 1 de salida). En fichero
de salida (txt) se imprimirá el tiempo, el número de pixeles filtrados por cada hilo, el número total de
pixeles que han sido aumentados, que han sido disminuidos y que no se han modificado (sin incluir los
no filtrados).


ENTREGA:
Ficheros fuentes comentados.
Tabla de resultados (Tiempo, speed-up y eficiencia). En la medición de tiempos no se incluye el trabajo
con ficheros de imágenes, ya sea lectura o escritura. El tiempo se mide en los mismos puntos que la
práctica 1.
Ejemplo de fichero de salida (txt) incluyendo sentencia de ejecución. 

 
Tarea 2. Sistema iterativo 

Desarrollar un programa que implemente el siguiente esquema iterativo (el mismo de la práctica 1)
para k=0..m. Teniendo en cuenta que el vector x
0
 será el vector unidad (= 1,1,....1). El valor del número 
de iteraciones y de alpha (valor 0< alpha <1) se especificará como argumento. 

Iteración 1:  
1
x
 = M x
0  
   
Iteración 2:  
2
x
 = M x
1
 ;  (Normalización x
2 
1
y x
2
); x
 = alpha x
2 
 -  (1 – alpha
2
1
) x
   
Iteración 3:  
3
x
 = M x
2
 ;            
Iteración 4:  
4
x
 = M x
3
 ;  (Normalización x
4 
3
y x
4
); x
 = alpha x
4 
 -  (1 – alpha
2
3
) x
   
Iteración 5:  
5
x
 = M x
4
 ;            
…  
