/*
 * s4.c — La señal perdida: una carrera con pause()
 *
 * Igual que s3.c, pero el padre envía la señal sin esperar. Lo más
 * probable es que SIGUSR1 llegue antes de que el hijo entre en pause():
 * el manejador se ejecuta, pause() se queda esperando una señal que ya
 * llegó y el padre se bloquea en wait(). El programa no termina.
 *
 * Observe: si se queda bloqueado, pulse Ctrl+C. Ejecútelo varias veces:
 *          el resultado cambia según cómo el planificador reparta la CPU.
 *          La solución correcta es bloquear la señal con sigprocmask()
 *          antes de fork() y esperarla con sigsuspend() (POSIX).
 *
 * Norma:    ISO C (signal), POSIX (fork, kill, pause, wait)
 * Compilar: gcc -Wall -Wextra -o s4 s4.c
 * Ejecutar: ./s4
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
        if (recibida)
            printf("[%d] hijo: la señal llegó antes de pause()\n", (int)getpid());
        pause();
        printf("[%d] hijo: señal %d capturada. Terminando\n", (int)getpid(), (int)recibida);
        exit(EXIT_SUCCESS);
    default:
        kill(pid, SIGUSR1);
        printf("[%d] padre: señal enviada\n", (int)getpid());
        wait(NULL);
        break;
    }

    signal(SIGUSR1, anterior);
    return EXIT_SUCCESS;
}
