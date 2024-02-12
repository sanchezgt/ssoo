/*
Este programa muestra cómo usar la función fork() en C para crear un proceso hijo.
El proceso padre crea un proceso hijo utilizando la función fork(). El proceso hijo 
imprime "Proceso hijo" y el proceso padre imprime "Proceso padre".
Para compilar y ejecutar este programa, sigue los siguientes pasos:
1. Abre una terminal.
2. Navega hasta el directorio donde se encuentra este archivo.
3. Compila el programa usando el siguiente comando:
   gcc programa.c -o programa
4. Ejecuta el programa usando el siguiente comando:
   ./programa
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    pid_t pidchild = fork();

    switch (pidchild) {
        case -1: // Error en la ejecución de fork(), se debe implementar la función manejadora del error
            perror("Error fork");
            exit(EXIT_FAILURE);
        case 0: // Bloque proceso hijo
            printf("Proceso hijo\n");
            break;
        default: // Bloque proceso padre
            printf("Proceso padre\n");
    }

    return 0;
}