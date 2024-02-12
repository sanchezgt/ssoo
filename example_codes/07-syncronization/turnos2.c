/*
Programa: Ejecución secuencial de hilos con sincronización en C
Descripción: Este programa crea una cantidad especificada de hilos y los ejecuta secuencialmente.
             Cada hilo espera su turno para ejecutarse, utilizando una variable de condición y un mutex para sincronización.
             El programa mide el tiempo total de ejecución de todos los hilos y lo imprime en segundos.
Instrucciones de compilación: gcc -o ejecucion_secuencial ejecucion_secuencial.c -lpthread
Instrucciones de ejecución: ./ejecucion_secuencial
*/

#include <stdio.h>      // Incluye funciones estándar de entrada y salida
#include <time.h>       // Incluye funciones para medir el tiempo
#include <stdlib.h>     // Incluye funciones de manejo de memoria dinámica y control de procesos
#include <pthread.h>    // Incluye las funciones y tipos para hilos POSIX

// Estructura para pasar datos a los hilos
struct dato {
    int id;
};

// Prototipos de las funciones que serán ejecutadas por los hilos y para manejar errores
void *f_hilo(void *);
void error(char *);

// Variables globales para controlar el turno de ejecución de los hilos y sincronización
int t_global = 0;
clock_t t_ini, t_fin;
pthread_cond_t vcond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    pthread_t *tid = NULL; // Puntero para almacenar los identificadores de los hilos
    struct dato *data = NULL; // Puntero para los datos de los hilos
    int nhilos; // Número de hilos a crear

    // Solicita al usuario el número de hilos a crear
    printf("# hilos?");
    scanf("%d", &nhilos);

    // Reserva memoria para los identificadores de los hilos
    tid = (pthread_t *)calloc(nhilos, sizeof(pthread_t));
    if (!tid) error("No hay suficiente memoria\n");

    // Inicia el contador de tiempo
    t_ini = clock();

    // Creación de hilos
    for (int i = 0; i < nhilos; ++i) {
        data = (struct dato *)malloc(sizeof(struct dato));
        if (!data) error("No hay suficiente memoria\n");
        data->id = i;
        pthread_create(&tid[i], NULL, f_hilo, (void*)data);
    }

    // Espera a que todos los hilos terminen
    for (int i = 0; i < nhilos; ++i) {
        pthread_join(tid[i], NULL);
    }

    // Finaliza el contador de tiempo y calcula la duración
    t_fin = clock();
    printf("%f seg\n", (double)(t_fin - t_ini) / CLOCKS_PER_SEC);

    // Libera la memoria de los identificadores de los hilos
    free(tid);

    // Destruye el mutex y la variable de condición
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&vcond);

    return EXIT_SUCCESS;
}

void *f_hilo(void *id) {
    int mid = ((struct dato*)id)->id; // Obtiene el ID del hilo

    // Bloquea el mutex para acceder a la variable 't_global'
    pthread_mutex_lock(&mutex);

    // Espera hasta que sea su turno
    while (mid != t_global)
        pthread_cond_wait(&vcond, &mutex);

    // Sección crítica: es el turno de este hilo
    printf("soy hilo :%d\n", mid);
    t_global = t_global + 1; // Incrementa el turno para el siguiente hilo

    // Notifica a todos los hilos que 't_global' ha cambiado
    pthread_cond_broadcast(&vcond);

    // Desbloquea el mutex
    pthread_mutex_unlock(&mutex);

    // Libera la memoria asignada para los datos del hilo
    free(id);

    // Termina la ejecución del hilo
    pthread_exit(0);
}

void error(char *msg) {
    perror(msg); // Imprime el mensaje de error
    exit(EXIT_FAILURE); // Termina el programa con un código de error
}