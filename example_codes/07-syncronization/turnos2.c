/*
 * turnos2.c — Turnos con mutex y variable de condición
 *
 * Mismo problema que turnos.c: el hilo i debe imprimir cuando turno vale i.
 * Aquí cada hilo que no tiene el turno se duerme en pthread_cond_wait(),
 * que libera el mutex mientras espera. Al terminar su turno, el hilo
 * incrementa turno y despierta a todos con pthread_cond_broadcast(); cada
 * uno revisa de nuevo la condición en el while.
 *
 * Observe: el tiempo de CPU es pequeño aunque haya muchos hilos: los
 *          que esperan duermen y no consumen CPU. El trabajo del turno
 *          (usleep) se hace fuera del mutex. Se usa broadcast y no signal
 *          porque solo uno de los dormidos es el siguiente, y signal
 *          podría despertar a otro.
 *
 * Norma:    POSIX threads (pthread_create, pthread_join, pthread_mutex_*,
 *           pthread_cond_*), POSIX (clock_gettime), ISO C (clock)
 * Compilar: gcc -Wall -Wextra -pthread -o turnos2 turnos2.c
 * Ejecutar: ./turnos2
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TRABAJO_US 10000                /* 10 ms de trabajo en cada turno */

static int turno = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cambio = PTHREAD_COND_INITIALIZER;

static void *funcion_hilo(void *arg)
{
    int mi_turno = *(int *)arg;

    pthread_mutex_lock(&mutex);
    while (turno != mi_turno)
        pthread_cond_wait(&cambio, &mutex);
    pthread_mutex_unlock(&mutex);

    printf("hilo %d\n", mi_turno);
    usleep(TRABAJO_US);

    pthread_mutex_lock(&mutex);
    turno = turno + 1;
    pthread_cond_broadcast(&cambio);
    pthread_mutex_unlock(&mutex);
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
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cambio);

    free(tid);
    free(id);
    return EXIT_SUCCESS;
}
