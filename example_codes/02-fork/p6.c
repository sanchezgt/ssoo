/*
Este programa en C demuestra la creación de procesos utilizando la función fork(). 
El proceso principal (padre) crea tres procesos hijos. El primer y el tercer hijo a su vez 
crean su propio proceso hijo. Al final, el proceso principal espera a que sus tres hijos 
terminen, mientras que cada hijo espera a su propio hijo, si lo tiene.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o proceso proceso.c
4. Ejecuta el programa compilado:
   ./proceso

Las librerías incluidas proporcionan las funciones necesarias para la creación y manejo de procesos.
*/

#include <stdio.h>      // Incluye las funciones estándar de entrada y salida.
#include <stdlib.h>     // Incluye las definiciones de macros, constantes y las funciones de manejo de memoria y procesos.
#include <sys/types.h>  // Incluye definiciones de tipos de datos usados en llamadas al sistema.
#include <sys/wait.h>   // Incluye las declaraciones para esperar.
#include <unistd.h>     // Incluye las constantes y declaraciones para llamadas al sistema POSIX.

int main(void) {
    pid_t root = getpid(); // Almacena el PID del proceso padre original.

    // Crea tres procesos hijos.
    for (int i = 0; i < 3; i++) {
        if (!fork()) { // Si es el proceso hijo...
            if (i == 0 || i == 2) { // El primer y tercer hijo crean su propio hijo.
                fork();
            }
            break; // El hijo no continúa el bucle.
        }
    }

    // El proceso padre espera a que terminen sus tres hijos directos.
    if (root == getpid()) {
        for (int j = 0; j < 3; j++) {
            wait(NULL); // Espera a que un proceso hijo termine.
        }
    } else {
        wait(NULL); // Cada hijo espera a su propio hijo, si lo tiene.
    }

    return EXIT_SUCCESS; // Termina el proceso con éxito.
}