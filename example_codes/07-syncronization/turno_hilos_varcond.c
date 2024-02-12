/*
Programa: Ejecución secuencial de hilos usando condiciones en C
Descripción: Este programa crea una cantidad especificada de hilos y los ejecuta secuencialmente.
             Cada hilo espera su turno para ejecutarse, garantizado por variables de condición y un mutex.
             El programa mide el tiempo total de ejecución de todos los hilos.
Instrucciones de compilación: gcc -o ejecucion_secuencial ejecucion_secuencial.c -lpthread
Instrucciones de ejecución: ./ejecucion_secuencial
*/

#include <stdio.h>      // Incluye funciones estándar de entrada y salida
#include <stdlib.h>     // Incluye funciones de manejo de memoria dinámica y control de procesos
#include <pthread.h>    // Incluye las funciones y tipos para hilos POSIX
#include <time.h>       // Incluye funciones para medir el tiempo

// Estructura para pasar datos a los hilos
struct DatoHilo {
    int dato;
};

// Prototipo de la función que será ejecutada por los hilos
void *funhilos(void *);

// Variables globales para controlar el turno y sincronización de hilos
int turno = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

int main() {
    clock_t t_ini, t_fin; // Variables para medir el tiempo de ejecución
    double secs; // Variable para almacenar el tiempo en segundos
    int nhilos = 0, i = 0; // Contador de hilos y variable de iteración
    pthread_t *pidhilos = NULL; // Puntero para almacenar los identificadores de los hilos
    struct DatoHilo *datohilo; // Puntero para los datos de los hilos

    // Solicita al usuario el número de hilos a crear
    printf("Numero de hilos\n");
    scanf("%d", &nhilos);

    // Creación de hilos
    pidhilos = (pthread_t *)calloc(nhilos, sizeof(pthread_t));
    for (i = 0; i < nhilos; i++) {
        datohilo = (struct DatoHilo*)malloc(sizeof(struct DatoHilo));
        datohilo->dato = i;
        pthread_create(&pidhilos[i], NULL, funhilos, (void*)datohilo);
    }

    // Inicia el contador de tiempo
    t_ini = clock();

    // Espera a que todos los hilos terminen
    for (i = 0; i < nhilos; i++)
        pthread_join(pidhilos[i], NULL);

    // Finaliza el contador de tiempo y calcula la duración
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("%.16g milisegundos\n", secs * 1000.0);

    // Libera la memoria de los identificadores de los hilos
    free(pidhilos);

    // Destruye el mutex y la variable de condición
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

void *funhilos(void *arg) {
    int myturno = ((struct DatoHilo *)arg)->dato; // Obtiene el turno del hilo

    // Bloquea el mutex para acceder a la variable 'turno'
    pthread_mutex_lock(&mutex);

    // Espera hasta que sea su turno
    while (turno != myturno)
        pthread_cond_wait(&cond, &mutex);

    // Sección crítica: es el turno de este hilo
    printf("Hilo turno %d\t[%u]\n", myturno, (unsigned int)pthread_self());
    turno++;

    // Notifica a todos los hilos que 'turno' ha cambiado
    pthread_cond_broadcast(&cond);

    // Desbloquea el mutex
    pthread_mutex_unlock(&mutex);

    // Libera la memoria asignada para los datos del hilo
    free(arg);

    // Termina la ejecución del hilo
    pthread_exit(0);
}