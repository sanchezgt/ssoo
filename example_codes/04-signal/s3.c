/*
 * s3.c — El padre envía una señal al hijo (manejador heredado)
 *
 * El padre instala el manejador antes de fork(); el hijo lo hereda y ya
 * está protegido desde que nace. El hijo espera con pause() y el padre,
 * tras una pausa corta, le envía SIGUSR1.
 *
 * Observe: todavía queda una carrera. Si la señal llega cuando el hijo
 *          aún no ha entrado en pause(), el manejador se ejecuta antes,
 *          pause() espera una señal que ya pasó y el programa se queda
 *          bloqueado. El usleep() del padre lo hace improbable, no
 *          imposible. s4.c quita esa pausa para provocar el problema.
 *
 * Norma:    ISO C (signal), POSIX (fork, kill, pause, wait)
 * Compilar: gcc -Wall -Wextra -o s3 s3.c
 * Ejecutar: ./s3
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
    void (*anterior)(int) = signal(SIGUSR1, manejador);
    if (anterior == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        pause();
        printf("[%d] hijo: señal %d capturada. Terminando\n", (int)getpid(), (int)recibida);
        exit(EXIT_SUCCESS);
    default:
        usleep(100000);
        kill(pid, SIGUSR1);
        printf("[%d] padre: señal enviada\n", (int)getpid());
        wait(NULL);
        break;
    }

    signal(SIGUSR1, anterior);
    return EXIT_SUCCESS;
}
