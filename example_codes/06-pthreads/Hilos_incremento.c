/*
 * Hilos_incremento.c — Exclusión mutua con un mutex
 *
 * N hilos incrementan ITER veces la misma variable global. Cada
 * incremento se hace dentro de una sección crítica protegida por un
 * mutex: solo un hilo a la vez puede estar entre lock y unlock.
 *
 * Observe: el resultado siempre es N x ITER. Comente las dos llamadas al
 *          mutex y verá que se pierden incrementos, igual que en
 *          05-shm/shm-concurrency.c pero entre hilos. Mida con  time  y
 *          compare: la exactitud tiene un costo.
 *
 * Norma:    POSIX threads (pthread_create, pthread_join, pthread_mutex_*)
 * Compilar: gcc -Wall -Wextra -pthread -o Hilos_incremento Hilos_incremento.c
 * Ejecutar: ./Hilos_incremento
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 7
#define ITER 10000000

static int valor = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *funcion_hilo(void *arg)
{
    (void)arg;
    for (int i = 0; i < ITER; i++) {
        pthread_mutex_lock(&mutex);
        valor = valor + 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid[N];

    for (int i = 0; i < N; i++) {
        if (pthread_create(&tid[i], NULL, funcion_hilo, NULL) != 0) {
            fprintf(stderr, "pthread_create falló\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("%d hilos, %d incrementos cada uno\n", N, ITER);

    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    printf("valor=%d (esperado %d)\n", valor, N * ITER);
    pthread_mutex_destroy(&mutex);
    return EXIT_SUCCESS;
}
