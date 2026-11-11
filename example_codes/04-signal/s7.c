/*
 * s7.c — Cadena de señales con N hijos, repetida varias vueltas
 *
 * Generaliza s6.c. El padre crea N_HIJOS hijos y la señal recorre la
 * cadena tantas vueltas como indique el usuario:
 *
 *     padre --> hijo N-1 --> ... --> hijo 1 --> hijo 0 --> padre
 *
 * Observe: en cada vuelta las líneas salen siempre en el mismo orden,
 *          aunque los procesos sean independientes: las señales imponen
 *          el turno. Cambie N_HIJOS y vea que la cadena se alarga.
 *
 * Norma:    ISO C (signal), POSIX (fork, kill, pause, wait)
 * Compilar: gcc -Wall -Wextra -o s7 s7.c
 * Ejecutar: ./s7
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N_HIJOS 3

static void manejador(int sig)
{
    (void)sig;
}

int main(void)
{
    pid_t hijo[N_HIJOS];
    pid_t raiz = getpid();
    int i, vueltas;

    printf("Número de vueltas: ");
    fflush(stdout);
    if (scanf("%d", &vueltas) != 1 || vueltas < 1) {
        fprintf(stderr, "Se esperaba un entero positivo\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGUSR1, manejador) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < N_HIJOS; i++) {
        hijo[i] = fork();
        if (hijo[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (hijo[i] == 0)
            break;
    }

    if (getpid() == raiz) {
        for (int k = 0; k < vueltas; k++) {
            usleep(10000);
            printf("padre  [%d] vuelta %d\n", (int)getpid(), k + 1);
            kill(hijo[N_HIJOS - 1], SIGUSR1);
            pause();
        }
        printf("padre  [%d] termina\n", (int)getpid());
        for (int k = 0; k < N_HIJOS; k++)
            wait(NULL);
    } else {
        for (int k = 0; k < vueltas; k++) {
            pause();
            printf("hijo %d [%d]\n", i, (int)getpid());
            kill(i > 0 ? hijo[i - 1] : getppid(), SIGUSR1);
        }
    }

    return EXIT_SUCCESS;
}
