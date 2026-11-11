/*
 * p5.c — Estado de salida del hijo
 *
 * El valor que el hijo pasa a exit() (o devuelve desde main) llega al
 * padre a través de wait(). El entero que entrega wait() codifica cómo
 * terminó el hijo; se decodifica con las macros WIFEXITED/WEXITSTATUS
 * (terminación normal) y WIFSIGNALED/WTERMSIG (terminado por una señal).
 *
 * Observe: solo los 8 bits bajos del estado llegan al padre. Pruebe con
 *          exit(300) y verá 44 (300 mod 256).
 *
 * Norma:    POSIX (fork, wait, macros de <sys/wait.h>)
 * Compilar: gcc -Wall -Wextra -o p5 p5.c
 * Ejecutar: ./p5
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int estado;
    pid_t pid = fork();

    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        exit(125);
    default:
        if (wait(&estado) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(estado))
            printf("El hijo %d terminó con estado %d\n", (int)pid, WEXITSTATUS(estado));
        else if (WIFSIGNALED(estado))
            printf("El hijo %d terminó por la señal %d\n", (int)pid, WTERMSIG(estado));
        break;
    }

    return EXIT_SUCCESS;
}
