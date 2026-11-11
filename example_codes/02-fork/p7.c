/*
 * p7.c — Árbol de procesos: un hijo con tres nietos
 *
 *          padre
 *            |
 *           h0
 *        /   |   \
 *      n0    n1    n2
 *
 * El padre crea un solo hijo, y ese hijo crea tres. Para saber quién es
 * quién se usa el contador del ciclo: en h0 el ciclo termina completo
 * (i == N); en cada nieto termina antes por el break.
 *
 * Observe: compare con p6.c: el mismo número de fork(), otra forma de
 *          árbol. p7-2.c es la misma estructura dibujada con pstree.
 *
 * Norma:    POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o p7 p7.c
 * Ejecutar: ./p7
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 3

int main(void)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        int i;
        for (i = 0; i < N; i++) {
            pid_t nieto = fork();
            if (nieto == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (nieto == 0)
                break;
        }
        if (i == N) {
            for (int j = 0; j < N; j++)
                wait(NULL);
        }
    } else {
        wait(NULL);
    }

    printf("Termina %d (ppid=%d)\n", (int)getpid(), (int)getppid());
    return EXIT_SUCCESS;
}
