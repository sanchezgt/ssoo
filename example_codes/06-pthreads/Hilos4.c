/*
 * Hilos4.c — Error común: todos los hilos reciben la misma dirección
 *
 * main() crea N hilos y a todos les pasa la dirección de la MISMA
 * variable, que modifica en cada vuelta del ciclo. Cada hilo lee el valor
 * que tenga la variable cuando le toque ejecutarse, no el que tenía al
 * crearlo. Además, todos escriben la variable global numero sin
 * protección.
 *
 * Observe: quite el usleep() y verá valores repetidos (por ejemplo,
 *          varios hilos con dato=3). La pausa solo oculta el error.
 *          Hilos5.c muestra la forma correcta: un dato propio por hilo.
 *
 * Norma:    POSIX threads (pthread_create, pthread_join), POSIX (usleep)
 * Compilar: gcc -Wall -Wextra -pthread -o Hilos4 Hilos4.c
 * Ejecutar: ./Hilos4
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 4

struct nodo {
    int a;
};

static int numero = 0;

static void *funcion_hilo(void *arg)
{
    const struct nodo *p = arg;
    printf("Hilo [%lu] dato=%d\n", (unsigned long)pthread_self(), p->a);
    numero = 500;
    return NULL;
}

int main(void)
{
    pthread_t tid[N];
    struct nodo dato;

    for (int i = 0; i < N; i++) {
        dato.a = i;
        if (pthread_create(&tid[i], NULL, funcion_hilo, &dato) != 0) {
            fprintf(stderr, "pthread_create falló\n");
            exit(EXIT_FAILURE);
        }
        usleep(100);
    }
    printf("Hilo principal [%lu]\n", (unsigned long)pthread_self());

    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    printf("numero=%d\n", numero);
    return EXIT_SUCCESS;
}
