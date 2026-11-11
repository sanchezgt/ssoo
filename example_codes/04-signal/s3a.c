/*
 * s3a.c — El padre envía una señal al hijo (manejador instalado en el hijo)
 *
 * El hijo instala su manejador de SIGUSR1 y espera con pause(). El padre
 * espera un momento y le envía SIGUSR1 con kill().
 *
 * Observe: hay una carrera. Si la señal llega antes de que el hijo
 *          ejecute signal(), se aplica la acción por defecto de SIGUSR1
 *          y el hijo muere sin imprimir "Terminando" (el padre lo informa).
 *          Reduzca el usleep() del padre para verlo. s3.c evita esta
 *          parte de la carrera instalando el manejador antes de fork().
 *
 * Norma:    ISO C (signal), POSIX (fork, kill, pause, wait)
 * Compilar: gcc -Wall -Wextra -o s3a s3a.c
 * Ejecutar: ./s3a
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t recibida = 0;

static void manejador(int sig)
{
    recibida = sig;
}

int main(void)
{
    pid_t pid = fork();
    int estado;

    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        if (signal(SIGUSR1, manejador) == SIG_ERR) {
            perror("signal");
            exit(EXIT_FAILURE);
        }
        pause();
        printf("[%d] hijo: señal %d capturada. Terminando\n", (int)getpid(), (int)recibida);
        break;
    default:
        usleep(200000);
        kill(pid, SIGUSR1);
        printf("[%d] padre: señal enviada\n", (int)getpid());
        wait(&estado);
        if (WIFSIGNALED(estado))
            printf("[%d] padre: el hijo murió por la señal %d\n", (int)getpid(), WTERMSIG(estado));
        break;
    }

    return EXIT_SUCCESS;
}
