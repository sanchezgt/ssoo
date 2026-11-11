/*
 * pipe4.c — Fin de archivo en una tubería
 *
 * El padre lee líneas del teclado y las envía al hijo por la tubería. El
 * hijo lee en un ciclo hasta que read() devuelve 0, lo que ocurre cuando
 * la tubería está vacía y ya nadie la tiene abierta para escribir. Por
 * eso es obligatorio que el hijo cierre su copia de fd[1]: si no lo hace,
 * él mismo mantiene abierta la escritura y nunca ve el fin de archivo.
 *
 * Observe: escriba varias líneas y termine con "salir" o con Ctrl+D.
 *          Si escribe muy rápido, el hijo puede recibir dos mensajes en
 *          un solo read(): una tubería no conserva límites de mensaje.
 *
 * Norma:    POSIX (pipe, fork, read, write, close, wait)
 * Compilar: gcc -Wall -Wextra -o pipe4 pipe4.c
 * Ejecutar: ./pipe4
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 256

int main(void)
{
    int fd[2];
    char buf[MAX];

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
        ssize_t n;
        close(fd[1]);
        while ((n = read(fd[0], buf, sizeof buf - 1)) > 0) {
            buf[n] = '\0';
            printf("[%d] hijo lee    <-- %s\n", (int)getpid(), buf);
        }
        printf("[%d] hijo: fin de archivo, termino\n", (int)getpid());
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    close(fd[0]);
    printf("Escriba mensajes; \"salir\" o Ctrl+D para terminar\n");
    while (fgets(buf, sizeof buf, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';
        if (strcmp(buf, "salir") == 0)
            break;
        if (buf[0] == '\0')
            continue;
        printf("[%d] padre escribe --> %s\n", (int)getpid(), buf);
        if (write(fd[1], buf, strlen(buf)) == -1)
            perror("write");
    }
    close(fd[1]);
    wait(NULL);

    return EXIT_SUCCESS;
}
