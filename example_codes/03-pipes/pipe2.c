/*
 * pipe2.c — Padre e hijo comunicados por una tubería
 *
 * La tubería se crea antes de fork(), así que ambos procesos heredan los
 * dos descriptores. Cada uno cierra el extremo que no usa: el padre
 * escribe y el hijo lee.
 *
 * Observe: el hijo imprime "esperando" y queda bloqueado en read() los
 *          5 segundos que el padre tarda en escribir. read() bloquea
 *          mientras la tubería esté vacía y alguien la tenga abierta
 *          para escritura.
 *
 * Norma:    POSIX (pipe, fork, read, write, close, wait)
 * Compilar: gcc -Wall -Wextra -o pipe2 pipe2.c
 * Ejecutar: ./pipe2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
        char buf[64];
        close(fd[1]);
        printf("[%d] hijo esperando datos...\n", (int)getpid());
        ssize_t n = read(fd[0], buf, sizeof buf - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("[%d] leído   <-- %s\n", (int)getpid(), buf);
        }
        close(fd[0]);
    } else {
        const char *mensaje = "Mensaje de texto";
        close(fd[0]);
        sleep(5);
        if (write(fd[1], mensaje, strlen(mensaje)) == -1)
            perror("write");
        printf("[%d] escrito --> %s\n", (int)getpid(), mensaje);
        close(fd[1]);
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
