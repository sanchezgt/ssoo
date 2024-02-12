/* 
Este programa en C demuestra la comunicación entre procesos utilizando pipes.
Crea un proceso hijo (h0) y un proceso nieto (h1) que se comunican a través de pipes.
El nieto escribe un valor entero en un pipe que es leído por el hijo, y luego el hijo
escribe este valor en otro pipe que es leído por el proceso padre.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o pipe_communication pipe_communication.c
4. Ejecuta el programa compilado:
   ./pipe_communication
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf y read/write
#include <stdlib.h>   // Para la macro EXIT_SUCCESS
#include <unistd.h>   // Para las funciones pipe(), fork(), close() y getpid()
#include <sys/wait.h> // Para la función wait()

int main() {
    int tub[2][2]; // Array para almacenar los descriptores de archivo de dos pipes

    // Crear dos pipes
    pipe(tub[0]);
    pipe(tub[1]);

    if (!fork()) { // Proceso hijo h0
        int data;

        if (!fork()) { // Proceso nieto h1
            data = 50; // Valor a escribir en el pipe
            // Cierra los descriptores de archivo no necesarios en el nieto
            close(tub[0][0]);
            close(tub[0][1]);
            close(tub[1][0]);
            // Escribe el valor en el pipe
            printf("nieto escrito %d \n", data);
            write(tub[1][1], &data, sizeof(int));
        } else { // Vuelve al proceso hijo h0
            // Cierra los descriptores de archivo no necesarios en el hijo
            close(tub[0][0]);
            close(tub[1][1]);
            // Lee el valor del pipe
            read(tub[1][0], &data, sizeof(int));
            // Escribe el valor leído en el otro pipe
            write(tub[0][1], &data, sizeof(int));
            printf("leido %d \n", data);
            // Espera a que el nieto termine
            wait(NULL);
        }
    } else { // Proceso padre
        int data;
        // Cierra los descriptores de archivo no necesarios en el padre
        close(tub[1][0]);
        close(tub[1][1]);
        close(tub[0][1]);
        // Lee el valor del pipe
        read(tub[0][0], &data, sizeof(int));
        printf("data=%d\n", data);
        // Espera a que el hijo termine
        wait(NULL);
    }

    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente
}