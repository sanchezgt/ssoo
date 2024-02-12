/* 
Este programa en C demuestra la comunicación entre procesos utilizando pipes.
Crea dos procesos hijos que escriben un valor flotante en su respectivo pipe.
El proceso padre lee estos valores flotantes de ambos pipes y calcula su suma.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o pipe_summation pipe_summation.c
4. Ejecuta el programa compilado:
   ./pipe_summation
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para la macro EXIT_SUCCESS
#include <unistd.h>   // Para las funciones pipe(), fork(), read(), write()
#include <sys/wait.h> // Para la función wait()

int main() {
    int tubs[2][2]; // Array para almacenar los descriptores de archivo de dos pipes
    int i;          // Contador para bucles
    int root = getpid(); // ID del proceso padre

    // Crear dos pipes
    for (i = 0; i < 2; ++i) {
        pipe(tubs[i]);
    }
    
    // Crear dos procesos hijos
    for (i = 0; i < 2; ++i) {
        if (!fork()) break; // Si es un proceso hijo, sale del bucle
    }

    // Proceso padre
    if (root == getpid()) {
        float datos[2]; // Array para almacenar los datos leídos de los pipes
        // Cierra los extremos de escritura de los pipes que no se usan en el padre
        close(tubs[0][1]);
        close(tubs[1][1]);

        // Leer los datos de ambos hijos
        for (i = 0; i < 2; ++i) {
            read(tubs[i][0], &datos[i], sizeof(float)); // Lee un float de cada pipe
            printf("hijo[%d]<-%f\n", i, datos[i]); // Imprime el valor leído
        }
        printf("suma total = %f\n", datos[0] + datos[1]); // Imprime la suma de los valores

        // Espera a que ambos hijos terminen
        wait(NULL);
        wait(NULL);
    } else { // Procesos hijos
        // Determina el índice del pipe que no se usará para escribir en este hijo
        int tc = i == 1 ? 0 : 1;
        float data; // Variable para almacenar el dato a escribir en el pipe

        // Cierra los extremos de lectura de los pipes y el extremo de escritura no utilizado
        close(tubs[0][0]);
        close(tubs[1][0]);
        close(tubs[tc][1]);

        // Asigna un valor a la variable data dependiendo de si es el primer o segundo hijo
        if (i == 0) {
            data = 10.3;
        } else {
            data = 8.4;
        }

        // Escribe el valor de data en el pipe correspondiente
        write(tubs[i][1], &data, sizeof(data));
    }

    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente
}