/*
 * Hilos3.c — Pasar una estructura a un hilo
 *
 * Para dar varios datos a un hilo se agrupan en una estructura y se pasa
 * su dirección. Aquí la estructura vive en la pila de main(); es válido
 * porque main() espera al hilo con pthread_join() antes de retornar.
 *
 * Observe: compare con Hilos2.c (un entero en el montículo). La regla es
 *          la misma: el dato debe seguir existiendo mientras el hilo lo use.
 *
 * Norma:    POSIX threads (pthread_create, pthread_join)
 * Compilar: gcc -Wall -Wextra -pthread -o Hilos3 Hilos3.c
 * Ejecutar: ./Hilos3
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct nodo {
    int a;
    float b;
};

static void *funcion_hilo(void *arg)
{
    const struct nodo *p = arg;
    printf("Hilo [%lu] recibe [%d | %.1f]\n", (unsigned long)pthread_self(), p->a, p->b);
    return NULL;
}

int main(void)
{
    pthread_t tid;
    struct nodo dato = { .a = 10, .b = 25.5f };

    if (pthread_create(&tid, NULL, funcion_hilo, &dato) != 0) {
        fprintf(stderr, "pthread_create falló\n");
        exit(EXIT_FAILURE);
    }
    printf("Hilo principal [%lu]\n", (unsigned long)pthread_self());

    pthread_join(tid, NULL);
    return EXIT_SUCCESS;
}
