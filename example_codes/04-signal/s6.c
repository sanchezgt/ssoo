/*
 * s6.c — Anillo de señales entre tres procesos
 *
 * El padre crea dos hijos. Una señal SIGUSR1 recorre el anillo:
 *
 *     padre --> hijo 1 --> hijo 0 --> padre
 *
 * Cada proceso espera con pause(), imprime su PID y pasa la señal al
 * siguiente. El manejador está vacío: solo sirve para que la señal
 * despierte a pause() en lugar de terminar el proceso.
 *
 * Observe: el hijo 1 conoce el PID del hijo 0 porque nació después y
 *          heredó el arreglo ya lleno; el hijo 0 no conoce el del hijo 1.
 *          La espera inicial del padre da tiempo a que los hijos entren
 *          en pause() (ver la carrera en s4.c).
 *
 * Norma:    ISO C (signal), POSIX (fork, kill, pause, wait)
 * Compilar: gcc -Wall -Wextra -o s6 s6.c
 * Ejecutar: ./s6
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 2

static void manejador(int sig)
{
    (void)sig;
}

int main(void)
{
    pid_t hijo[N];
    pid_t raiz = getpid();
    int i;

    if (signal(SIGUSR1, manejador) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < N; i++) {
        hijo[i] = fork();
        if (hijo[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (hijo[i] == 0)
            break;
    }

    if (getpid() == raiz) {
        printf("padre  [%d] inicia\n", (int)getpid());
        usleep(100000);
        kill(hijo[N - 1], SIGUSR1);
        pause();
        printf("padre  [%d] recibe y termina\n", (int)getpid());
        for (int k = 0; k < N; k++)
            wait(NULL);
    } else {
        pause();
        printf("hijo %d [%d]\n", i, (int)getpid());
        kill(i > 0 ? hijo[i - 1] : getppid(), SIGUSR1);
    }

    return EXIT_SUCCESS;
}
