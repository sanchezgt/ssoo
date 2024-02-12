/*
Este programa muestra cómo usar la función fork() en C para crear múltiples procesos hijos y 
esperar a que terminen en el proceso padre.
El proceso padre crea tres procesos hijos utilizando la función fork(). Cada proceso hijo 
imprime su PID y el PID de su proceso padre (PPID). El proceso padre espera a que los tres procesos 
hijos terminen utilizando la función wait(). Finalmente, el proceso padre imprime un mensaje de finalización.
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
    pid_t root = getpid(); // Obtiene el PID del proceso padre

    for (int i = 0; i < 3; i++) {
        if (!fork()) { // Crea un proceso hijo y verifica si es el proceso hijo
            break; // Si es el proceso hijo, sale del bucle
        }
    }

    if (root == getpid()) { // Si es el proceso padre
        for (int j = 0; j < 3; j++) {
            wait(NULL); // Espera a que los procesos hijos terminen
        }
        printf("Finalizando padre %d\n", getpid()); // Imprime un mensaje de finalización del proceso padre
    } else { // Si es el proceso hijo
        printf("Finalizando hijo %d ppid:%d\n", getpid(), getppid()); // Imprime el PID del proceso hijo y el PID del proceso padre
    }

    return EXIT_SUCCESS;
}