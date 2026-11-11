/*
 * pipe3.c — Enviar una estructura por una tubería
 *
 * Una tubería transporta bytes sin interpretarlos. Se puede enviar una
 * estructura completa escribiendo sizeof(struct) bytes desde su dirección,
 * siempre que el lector use la misma definición de la estructura.
 *
 * Observe: sizeof(struct dato) incluye el relleno que agrega el
 *          compilador (ver 01-c/padding.c). Esto solo funciona entre
 *          procesos de la misma máquina y del mismo programa.
 *
 * Norma:    POSIX (pipe, fork, read, write, wait)
 * Compilar: gcc -Wall -Wextra -o pipe3 pipe3.c
 * Ejecutar: ./pipe3
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct dato {
    int a;
    float b;
};

int main(void)
{
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        struct dato recibido;
        close(fd[1]);
        if (read(fd[0], &recibido, sizeof recibido) == (ssize_t)sizeof recibido)
            printf("[%d] lee    <-- [%d | %.2f]\n", (int)getpid(), recibido.a, recibido.b);
        close(fd[0]);
    } else {
        struct dato enviado = { .a = 10, .b = 2.3f };
        close(fd[0]);
        printf("[%d] escribe --> [%d | %.2f] (%zu bytes)\n",
               (int)getpid(), enviado.a, enviado.b, sizeof enviado);
        if (write(fd[1], &enviado, sizeof enviado) == -1)
            perror("write");
        close(fd[1]);
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
