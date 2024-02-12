/*
Este programa muestra cómo usar la función fork() en C para crear un proceso hijo y obtener información 
sobre los identificadores de proceso (PID) y los identificadores de proceso padre (PPID).
El proceso padre crea un proceso hijo utilizando la función fork(). E
l proceso hijo espera 100000 microsegundos (0.1 segundos) utilizando usleep() y luego imprime su PID y PPID. 
El proceso padre también imprime su PID y PPID. Además, el proceso padre espera a que el proceso hijo termine utilizando 
la función wait().
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
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pidchild = fork();

    switch (pidchild) {
        case -1: // Error en la ejecución de fork(), se debe implementar la función manejadora del error
            perror("Error fork");
            exit(EXIT_FAILURE);
        case 0: // Bloque proceso hijo
            usleep(100000); // Espera 100000 microsegundos (0.1 segundos)
            printf("P. hijo: pid=%d y ppid=%d\n", getpid(), getppid());
            break;
        default: // Bloque proceso padre
            printf("P. padre: pid=%d y ppid=%d\n", getpid(), getppid());
            wait(NULL); // Espera a que el proceso hijo termine
    }

    return EXIT_SUCCESS;
}