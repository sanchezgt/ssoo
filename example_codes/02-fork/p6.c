/*
 * p6.c — Árbol de procesos: tres hijos, dos de ellos con un nieto
 *
 *          padre
 *        /   |   \
 *      h0    h1    h2
 *      |           |
 *      n0          n2
 *
 * El padre crea tres hijos. El primero y el tercero crean a su vez un
 * hijo propio. Cada proceso espera a sus hijos directos antes de terminar.
 *
 * Observe: wait() solo espera a los hijos directos. En los nietos y en h1
 *          wait() devuelve -1 de inmediato porque no tienen hijos.
 *          p6-2.c es la misma estructura, pero dibuja el árbol con pstree.
 *
 * Norma:    POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o p6 p6.c
 * Ejecutar: ./p6
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
        if (pid == 0) {
            if (i == 0 || i == 2)
                fork();
            break;
        }
    }

    if (getpid() == raiz) {
        for (int i = 0; i < N; i++)
            wait(NULL);
    } else {
        wait(NULL);
    }

    printf("Termina %d (ppid=%d)\n", (int)getpid(), (int)getppid());
    return EXIT_SUCCESS;
}
