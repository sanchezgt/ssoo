/*
Programa: Ejecución secuencial de hilos con espera activa en C
Descripción: Este programa crea una cantidad especificada de hilos y los ejecuta secuencialmente.
             Cada hilo realiza una espera activa hasta que una variable global 't_global' coincide con su ID.
             El programa mide el tiempo total de ejecución de todos los hilos y lo imprime en segundos.
             Nota: Este enfoque de espera activa puede ser ineficiente y no es recomendado para sincronización de hilos.
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

// Prototipo de la función que será ejecutada por los hilos
void *f_hilo(void *);
// Prototipo de la función para manejar errores
void error(char *);

// Variable global para controlar el turno de ejecución de los hilos
int t_global = 0;
// Variables para medir el tiempo de ejecución
clock_t t_ini, t_fin;

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

    return EXIT_SUCCESS;
}

void *f_hilo(void *id) {
    int mid = ((struct dato*)id)->id; // Obtiene el ID del hilo

    // Espera activa hasta que sea su turno
    while (mid != t_global); // No se recomienda este tipo de espera activa

    // Sección crítica: es el turno de este hilo
    printf("soy hilo :%d\n", mid);
    t_global = t_global + 1; // Incrementa el turno para el siguiente hilo

    // Libera la memoria asignada para los datos del hilo
    free(id);

    // Termina la ejecución del hilo
    pthread_exit(0);
}

void error(char *msg) {
    perror(msg); // Imprime el mensaje de error
    exit(EXIT_FAILURE); // Termina el programa con un código de error
}