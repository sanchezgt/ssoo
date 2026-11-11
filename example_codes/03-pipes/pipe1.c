/*
 * pipe1.c — Una tubería dentro de un solo proceso
 *
 * pipe() crea un canal de bytes en el núcleo y devuelve dos descriptores:
 * fd[0] para leer y fd[1] para escribir. Lo que se escribe en fd[1] sale,
 * en el mismo orden, por fd[0].
 *
 * Observe: el mismo PID escribe y lee. Una tubería sola no comunica
 *          procesos; sirve para eso cuando se hereda con fork() (pipe2.c).
 *          write() no envía el '\0' final: el lector debe agregarlo.
 *
 * Norma:    POSIX (pipe, read, write)
 * Compilar: gcc -Wall -Wextra -o pipe1 pipe1.c
 * Ejecutar: ./pipe1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    int fd[2];
    const char *mensaje = "Hola mundo pipes";
    char buf[128];

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    printf("[%d] escribe --> %s\n", (int)getpid(), mensaje);
    if (write(fd[1], mensaje, strlen(mensaje)) == -1)
        perror("write");

    ssize_t n = read(fd[0], buf, sizeof buf - 1);
    if (n == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buf[n] = '\0';
    printf("[%d] lee    <-- %s (%zd bytes)\n", (int)getpid(), buf, n);

    close(fd[0]);
    close(fd[1]);
    return EXIT_SUCCESS;
}
