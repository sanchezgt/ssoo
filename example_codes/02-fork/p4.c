/*
 * p4.c — Varios hijos de un mismo padre (abanico)
 *
 * El padre crea N hijos dentro de un ciclo. Cada hijo sale del ciclo con
 * break; si no lo hiciera, también ejecutaría fork() en las vueltas
 * siguientes y se crearían 2^N - 1 procesos en lugar de N.
 *
 * Observe: todos los hijos tienen el mismo PPID. Quite el break y cuente
 *          cuántas líneas aparecen.
 *
 * Norma:    POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o p4 p4.c
 * Ejecutar: ./p4
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 3

int main(void)
{
    pid_t raiz = getpid();

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
            break;
    }

    if (getpid() == raiz) {
        for (int i = 0; i < N; i++)
            wait(NULL);
        printf("Termina el padre %d\n", (int)getpid());
    } else {
        printf("Termina el hijo %d (ppid=%d)\n", (int)getpid(), (int)getppid());
    }

    return EXIT_SUCCESS;
}
