/*
 * barrier.c — Barrera: esperar a que todos lleguen
 *
 * Cada hilo trabaja un tiempo distinto (aleatorio, 1 a 6 s) y luego llama
 * a pthread_barrier_wait(). Ningún hilo pasa de la barrera hasta que los
 * N hilos hayan llegado a ella; el último en llegar libera a todos.
 *
 * Observe: los mensajes "hecho" salen escalonados, pero todos los
 *          "continúa" salen juntos cuando termina el hilo más lento.
 *          Cada hilo usa su propia semilla con rand_r(), porque rand() no
 *          es seguro entre hilos.
 *
 * Norma:    POSIX threads (pthread_barrier_*, pthread_create, pthread_join)
 * Compilar: gcc -Wall -Wextra -pthread -o barrier barrier.c
 * Ejecutar: ./barrier
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 6

static pthread_barrier_t barrera;

struct param {
    int id;
    unsigned int semilla;
};

static void *funcion_hilo(void *arg)
{
    struct param *p = arg;
    int espera = 1 + rand_r(&p->semilla) % 6;

    printf("hilo %d: trabajando %d s\n", p->id, espera);
    sleep((unsigned)espera);
    printf("hilo %d: hecho, espera en la barrera\n", p->id);

    pthread_barrier_wait(&barrera);

    printf("hilo %d: continúa\n", p->id);
    return NULL;
}

int main(void)
{
    pthread_t tid[N];
    struct param param[N];
    unsigned int base = (unsigned int)time(NULL);

    pthread_barrier_init(&barrera, NULL, N);

    for (int i = 0; i < N; i++) {
        param[i].id = i;
        param[i].semilla = base + (unsigned int)i;
        if (pthread_create(&tid[i], NULL, funcion_hilo, &param[i]) != 0) {
            fprintf(stderr, "pthread_create falló\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    pthread_barrier_destroy(&barrera);
    return EXIT_SUCCESS;
}
