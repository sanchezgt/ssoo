/*
 * Hilos2.c — Pasar un argumento a un hilo
 *
 * El hilo recibe un solo argumento de tipo void *. Para pasarle un entero
 * se le da la dirección de una variable y el hilo la convierte de nuevo
 * a int *. El hilo principal espera con pthread_join() antes de liberar
 * esa memoria.
 *
 * Observe: si se libera el dato antes de pthread_join(), el hilo podría
 *          leer memoria ya liberada.
 *
 * Norma:    POSIX threads (pthread_create, pthread_join)
 * Compilar: gcc -Wall -Wextra -pthread -o Hilos2 Hilos2.c
 * Ejecutar: ./Hilos2
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static void *funcion_hilo(void *arg)
{
    int *dato = arg;
    printf("Hilo [%lu] recibe dato=%d\n", (unsigned long)pthread_self(), *dato);
    return NULL;
}

int main(void)
{
    pthread_t tid;
    int *dato = malloc(sizeof *dato);
    if (dato == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    *dato = 10;

    if (pthread_create(&tid, NULL, funcion_hilo, dato) != 0) {
        fprintf(stderr, "pthread_create falló\n");
        exit(EXIT_FAILURE);
    }
    printf("Hilo principal [%lu]\n", (unsigned long)pthread_self());

    pthread_join(tid, NULL);
    free(dato);
    return EXIT_SUCCESS;
}
