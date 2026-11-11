/*
 * s0.c — Capturar SIGINT (Ctrl+C)
 *
 * Por defecto, Ctrl+C envía SIGINT y el proceso termina. Con signal() se
 * instala un manejador: una función que el núcleo ejecuta, interrumpiendo
 * al programa, cada vez que llega la señal.
 *
 * El manejador solo anota que la señal llegó; el mensaje lo imprime el
 * ciclo principal. Dentro de un manejador solo se deben usar funciones
 * seguras ante señales (async-signal-safe), y printf() no lo es.
 *
 * Observe: Ctrl+C ya no termina el programa. Use Ctrl+\ (SIGQUIT) o
 *          kill <pid> desde otra terminal.
 *
 * Norma:    ISO C (signal), POSIX (sleep)
 * Compilar: gcc -Wall -Wextra -o s0 s0.c
 * Ejecutar: ./s0
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile sig_atomic_t senal_recibida = 0;

static void manejador(int sig)
{
    senal_recibida = sig;
}

int main(void)
{
    if (signal(SIGINT, manejador) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    printf("[%d] Pulse Ctrl+C; Ctrl+\\ para salir\n", (int)getpid());
    for (;;) {
        if (senal_recibida) {
            printf("Señal capturada [%d]\n", (int)senal_recibida);
            senal_recibida = 0;
        }
        printf("Hola mundo\n");
        sleep(1);
    }
}
