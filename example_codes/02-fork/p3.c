/*
 * p3.c — wait(): el padre espera al hijo
 *
 * Igual que p2.c, pero el padre llama a wait() y se bloquea hasta que el
 * hijo termine. Así el hijo no queda huérfano y el padre recoge su estado
 * de salida (si no lo hiciera, el hijo quedaría como zombi).
 *
 * Observe: ahora el PPID del hijo sí coincide con el PID del padre, y el
 *          prompt aparece después de las dos líneas.
 *
 * Norma:    POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o p3 p3.c
 * Ejecutar: ./p3
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
        wait(NULL);
        break;
    }

    return EXIT_SUCCESS;
}
