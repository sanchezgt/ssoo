/*
Este programa muestra cómo usar la función fork() en C para crear un proceso hijo y esperar a 
que termine en el proceso padre.
El proceso padre crea un proceso hijo utilizando la función fork(). El proceso hijo retorna 
un valor de estado utilizando la función exit() o return. El proceso padre espera a que el 
proceso hijo termine utilizando la función wait() y luego imprime el estado de salida del proceso hijo.
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
    int estado;
    pid_t pidchild, pidwait;

    pidchild = fork(); // Crea un proceso hijo

    switch (pidchild) {
        case -1: // Error en la ejecución de fork(), se debe implementar la función manejadora del error
            perror("Error fork");
            exit(EXIT_FAILURE);
        case 0: // Bloque proceso hijo
            //exit(100); // Retorna un valor de estado utilizando la función exit()
            return 125; // Retorna un valor de estado utilizando la declaración return
        default: // Bloque proceso padre
            pidwait = wait(&estado); // Espera a que el proceso hijo termine y obtiene el estado de salida
            if (pidwait != -1) {
                if (WIFEXITED(estado)) {
                    printf("Hijo estado: %d\n", WEXITSTATUS(estado)); // Imprime el estado de salida del proceso hijo
                }
            }
    }

    return EXIT_SUCCESS;
}