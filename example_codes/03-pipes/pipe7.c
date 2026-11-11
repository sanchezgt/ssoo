/*
 * pipe7.c — Cadena de procesos: nieto -> hijo -> padre
 *
 * Un dato viaja por dos tuberías a través de tres generaciones:
 *
 *     nieto --tub[1]--> hijo --tub[0]--> padre
 *
 * El nieto produce el valor, el hijo lo reenvía y el padre lo muestra.
 * Ningún proceso necesita sincronización adicional: cada read() bloquea
 * hasta que llega el dato.
 *
 * Observe: los descriptores se heredan en cascada. Cada proceso cierra
 *          los que no usa para que los fines de archivo funcionen.
 *
 * Norma:    POSIX (pipe, fork, read, write, close, wait)
 * Compilar: gcc -Wall -Wextra -o pipe7 pipe7.c
 * Ejecutar: ./pipe7
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static pid_t crear_proceso(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

int main(void)
{
    int tub[2][2];
    int dato;

    if (pipe(tub[0]) == -1 || pipe(tub[1]) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (crear_proceso() == 0) {
        if (crear_proceso() == 0) {
            /* Nieto: escribe en tub[1]. */
            close(tub[0][0]);
            close(tub[0][1]);
            close(tub[1][0]);
            dato = 50;
            printf("[%d] nieto escribe %d\n", (int)getpid(), dato);
            if (write(tub[1][1], &dato, sizeof dato) == -1)
                perror("write");
            close(tub[1][1]);
            exit(EXIT_SUCCESS);
        }
        /* Hijo: lee de tub[1] y reenvía por tub[0]. */
        close(tub[0][0]);
        close(tub[1][1]);
        if (read(tub[1][0], &dato, sizeof dato) == (ssize_t)sizeof dato) {
            printf("[%d] hijo reenvía %d\n", (int)getpid(), dato);
            if (write(tub[0][1], &dato, sizeof dato) == -1)
                perror("write");
        }
        close(tub[1][0]);
        close(tub[0][1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    /* Padre: lee de tub[0]. */
    close(tub[1][0]);
    close(tub[1][1]);
    close(tub[0][1]);
    if (read(tub[0][0], &dato, sizeof dato) == (ssize_t)sizeof dato)
        printf("[%d] padre recibe %d\n", (int)getpid(), dato);
    close(tub[0][0]);
    wait(NULL);

    return EXIT_SUCCESS;
}
