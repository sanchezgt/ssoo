/*
 * s1.c — Instalar y restaurar un manejador
 *
 * signal() devuelve el manejador que había antes (o SIG_ERR si falla).
 * Guardarlo permite restaurar el comportamiento original cuando el
 * programa ya no necesita su manejador.
 *
 * Observe: raise() envía una señal al propio proceso. La primera vez la
 *          atiende el manejador. Después de restaurar, SIGUSR1 vuelve a
 *          su acción por defecto (terminar el proceso), por eso no se
 *          envía de nuevo.
 *
 * Norma:    ISO C (signal, raise)
 * Compilar: gcc -Wall -Wextra -o s1 s1.c
 * Ejecutar: ./s1
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static volatile sig_atomic_t recibidas = 0;

static void manejador(int sig)
{
    (void)sig;
    recibidas++;
}

int main(void)
{
    void (*anterior)(int) = signal(SIGUSR1, manejador);
    if (anterior == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    printf("Manejador anterior: %s\n", anterior == SIG_DFL ? "SIG_DFL (por defecto)" : "otro");

    raise(SIGUSR1);
    printf("SIGUSR1 recibida %d vez\n", (int)recibidas);

    if (signal(SIGUSR1, anterior) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    printf("Manejador restaurado\n");

    return EXIT_SUCCESS;
}
