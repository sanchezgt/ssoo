/*
 * turno_hilos.c — Turnos con espera activa
 *
 * Se crean N hilos que deben trabajar en orden: el hilo i solo avanza
 * cuando la variable global turno vale i, trabaja 10 ms y pasa el turno.
 * Mientras no es su turno, cada hilo da vueltas en un ciclo vacío
 * preguntando por la variable.
 *
 * Observe: el orden es correcto, pero cada hilo que espera ocupa una CPU
 *          sin hacer nada útil. El tiempo de CPU sale mucho mayor que el
 *          tiempo real, y con más hilos que núcleos se vuelve muy lento.
 *          Compare con turno_hilos_varcond.c, que espera con una variable de condición.
 *          volatile solo impide que el compilador saque la lectura del
 *          ciclo; no es un mecanismo de sincronización.
 *
 * Norma:    POSIX threads (pthread_create, pthread_join),
 *           POSIX (clock_gettime), ISO C (clock)
 * Compilar: gcc -Wall -Wextra -pthread -o turno_hilos turno_hilos.c
 * Ejecutar: ./turno_hilos
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TRABAJO_US 10000                /* 10 ms de trabajo en cada turno */

static volatile int turno = 0;

static void *funcion_hilo(void *arg)
{
    int mi_turno = *(int *)arg;

    while (turno != mi_turno)
        ;                                   /* espera activa */

    printf("hilo %d\n", mi_turno);
    usleep(TRABAJO_US);
    turno = turno + 1;
    return NULL;
}

static double segundos_reales(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

int main(void)
{
    int n;

    printf("Número de hilos: ");
    fflush(stdout);
    if (scanf("%d", &n) != 1 || n < 1) {
        fprintf(stderr, "Se esperaba un entero positivo\n");
        exit(EXIT_FAILURE);
    }

    pthread_t *tid = calloc((size_t)n, sizeof *tid);
    int *id = calloc((size_t)n, sizeof *id);
    if (tid == NULL || id == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    double real0 = segundos_reales();
    clock_t cpu0 = clock();

    for (int i = 0; i < n; i++) {
        id[i] = i;
        if (pthread_create(&tid[i], NULL, funcion_hilo, &id[i]) != 0) {
            fprintf(stderr, "pthread_create falló\n");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < n; i++)
        pthread_join(tid[i], NULL);

    printf("tiempo real: %.1f ms   tiempo de CPU: %.1f ms\n",
           (segundos_reales() - real0) * 1000.0,
           (double)(clock() - cpu0) * 1000.0 / CLOCKS_PER_SEC);

    free(tid);
    free(id);
    return EXIT_SUCCESS;
}
