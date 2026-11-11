/*
 * Hilos5.c — Un dato propio para cada hilo
 *
 * Corrige Hilos4.c: en cada vuelta del ciclo se reserva una estructura
 * nueva con malloc(), de modo que cada hilo recibe su propia copia. Cada
 * hilo libera su estructura cuando termina de usarla.
 *
 * Observe: todos los valores 0..N-1 aparecen exactamente una vez, aunque
 *          en cualquier orden.
 *
 * Norma:    POSIX threads (pthread_create, pthread_join)
 * Compilar: gcc -Wall -Wextra -pthread -o Hilos5 Hilos5.c
 * Ejecutar: ./Hilos5
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4

struct nodo {
    int a;
};

static void *funcion_hilo(void *arg)
{
    struct nodo *p = arg;
    printf("Hilo [%lu] dato=%d\n", (unsigned long)pthread_self(), p->a);
    free(p);
    return NULL;
}

int main(void)
{
    pthread_t tid[N];

    for (int i = 0; i < N; i++) {
        struct nodo *dato = malloc(sizeof *dato);
        if (dato == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        dato->a = i;
        if (pthread_create(&tid[i], NULL, funcion_hilo, dato) != 0) {
            fprintf(stderr, "pthread_create falló\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("Hilo principal [%lu]\n", (unsigned long)pthread_self());

    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    return EXIT_SUCCESS;
}
