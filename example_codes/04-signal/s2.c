/*
 * s2.c — Contar señales
 *
 * El manejador incrementa un contador global cada vez que llega SIGINT.
 * El contador es volatile sig_atomic_t: volatile obliga a leerlo de
 * memoria en cada vuelta del ciclo, y sig_atomic_t garantiza que se lee
 * y escribe de una sola vez aunque una señal interrumpa.
 *
 * Observe: pulse Ctrl+C varias veces, o envíe la señal desde otra
 *          terminal con  kill -INT <pid>. Si pulsa muy rápido, puede
 *          contar menos: las señales pendientes del mismo tipo no se
 *          acumulan. Para salir use Ctrl+\ (SIGQUIT).
 *
 * Norma:    ISO C (signal), POSIX (sleep, kill desde la terminal)
 * Compilar: gcc -Wall -Wextra -o s2 s2.c
 * Ejecutar: ./s2
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile sig_atomic_t contador = 0;

static void manejador(int sig)
{
    (void)sig;
    contador++;
}

int main(void)
{
    if (signal(SIGINT, manejador) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    printf("[%d] Pulse Ctrl+C; Ctrl+\\ para salir\n", (int)getpid());
    for (;;) {
        printf("SIGINT recibida %d veces\n", (int)contador);
        sleep(1);
    }
}
