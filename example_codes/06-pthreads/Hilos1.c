/*
 * Hilos1.c — Crear un hilo
 *
 * pthread_create() inicia un hilo que ejecuta la función indicada dentro
 * del mismo proceso: comparte memoria global, archivos abiertos y PID
 * con el hilo principal.
 *
 * Observe: el hilo principal NO espera al secundario. Cuando main()
 *          retorna, termina el proceso completo y con él todos sus hilos,
 *          así que a veces "Hilo secundario" no alcanza a imprimirse.
 *          Ejecútelo varias veces y luego active pthread_join().
 *
 * Norma:    POSIX threads (pthread_create, pthread_self, pthread_join)
 * Compilar: gcc -Wall -Wextra -pthread -o Hilos1 Hilos1.c
 * Ejecutar: ./Hilos1
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static void *funcion_hilo(void *arg)
{
    (void)arg;
    printf("Hilo secundario [%lu]\n", (unsigned long)pthread_self());
    return NULL;
}

int main(void)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, funcion_hilo, NULL) != 0) {
        fprintf(stderr, "pthread_create falló\n");
        exit(EXIT_FAILURE);
    }
    printf("Hilo principal  [%lu]\n", (unsigned long)pthread_self());

    /* pthread_join(tid, NULL); */

    return EXIT_SUCCESS;
}
