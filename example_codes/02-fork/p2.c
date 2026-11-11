/*
 * p2.c — Proceso huérfano
 *
 * El hijo duerme 0,1 s antes de imprimir. Mientras tanto el padre imprime
 * y termina sin esperarlo, así que el hijo queda huérfano y el sistema lo
 * reasigna a otro proceso (init o un "subreaper").
 *
 * Observe: el PPID que imprime el hijo ya no es el PID del padre. La shell
 *          muestra el prompt antes de que el hijo escriba. Compare con p3.c.
 *
 * Norma:    POSIX (fork, getpid, getppid, usleep)
 * Compilar: gcc -Wall -Wextra -o p2 p2.c
 * Ejecutar: ./p2
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        usleep(100000);
        printf("Hijo:  pid=%d ppid=%d\n", (int)getpid(), (int)getppid());
        break;
    default:
        printf("Padre: pid=%d ppid=%d\n", (int)getpid(), (int)getppid());
        break;
    }

    return EXIT_SUCCESS;
}
