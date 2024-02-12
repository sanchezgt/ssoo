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
    int tub[2][2]; // Array para almacenar los descriptores de archivo de dos pipes
    int i;         // Contador para bucles
    pid_t root = getpid(); // ID del proceso padre

    // Crear dos pipes
    for (i = 0; i < 2; ++i) {
        pipe(tub[i]);
    }

    // Crear dos procesos hijos
    for (i = 0; i < 2; ++i) {
        if (!fork()) break; // Si es un proceso hijo, sale del bucle
    }

    // Proceso padre
    if (root == getpid()) {
        float datos[2]; // Array para almacenar los datos leídos de los pipes

        // Cierra los extremos de escritura de los pipes que no se usan en el padre
        for (i = 0; i < 2; ++i) {
            close(tub[i][1]);
        }

        // Leer los datos de ambos hijos
        for (int j = 0; j < 2; ++j) {
            read(tub[j][0], &datos[j], sizeof(float)); // Lee un float de cada pipe
        }

        // Espera a que ambos hijos terminen
        wait(NULL);
        wait(NULL);

        // Imprime la suma de los valores
        printf("suma total=%f\n", datos[0] + datos[1]);

    } else { // Procesos hijos
        float dato; // Variable para almacenar el dato a escribir en el pipe

        // Cierra los extremos de lectura de los pipes y el extremo de escritura no utilizado
        close(tub[0][0]);
        close(tub[1][0]);
        close(tub[i == 0 ? 1 : 0][1]);

        // Asigna un valor a la variable dato dependiendo de si es el primer o segundo hijo
        if (i == 0) {
            dato = 25.32; // f(datos_del_padre)
        } else {
            dato = 14.1;  // f(datos_del_padre)
        }

        // Escribe el valor de dato en el pipe correspondiente
        write(tub[i][1], &dato, sizeof(float));
        printf("[%d] -> %f\n", getpid(), dato);
    }

    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente
}