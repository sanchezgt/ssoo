/*
Programa: Ejemplo de sincronización de hilos con mutex en C
Descripción: Este programa crea varios hilos para incrementar una variable compartida.
             Utiliza un mutex para sincronizar el acceso a la variable y evitar condiciones de carrera.
             Cada hilo realiza un número determinado de incrementos en la variable compartida.
             Al final, el programa imprime el valor final de la variable compartida.
Instrucciones de compilación: gcc -o programa_sincronizado programa_sincronizado.c -lpthread
Instrucciones de ejecución: ./programa_sincronizado
*/

#include <stdio.h>      // Incluye funciones estándar de entrada y salida
#include <unistd.h>     // Proporciona acceso a la API POSIX del sistema operativo
#include <stdlib.h>     // Incluye la definición de la macro EXIT_SUCCESS
#include <pthread.h>    // Incluye las funciones y tipos para hilos POSIX

// Prototipo de la función que será ejecutada por los hilos
void* funcion_hilo(void* arg);

// Variable compartida y su valor inicial
int valor = 0;
// Número máximo de iteraciones que cada hilo realizará
int max_iter = 10000000;

// Declaración del mutex para sincronizar el acceso a la variable compartida
pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER;

int main() {
    int maxhilos = 7; // Número máximo de hilos a crear
    pthread_t tid[maxhilos]; // Array para almacenar los identificadores de los hilos

    // Creación de los hilos secundarios
    for (int i = 0; i < maxhilos; i++) {
        pthread_create(&tid[i], NULL, funcion_hilo, NULL);
    }

    printf("Hilo principal[%lu]\n", pthread_self()); // Imprime el identificador del hilo principal

    // Espera a que todos los hilos secundarios terminen
    for (int i = 0; i < maxhilos; i++) {
        pthread_join(tid[i], NULL);
    }

    // Imprime el valor final de la variable compartida
    printf("valor=[%d]\n", valor);

    // Destruye el mutex para liberar recursos
    pthread_mutex_destroy(&mt);

    return EXIT_SUCCESS; // Finaliza el programa con éxito
}

// Función que será ejecutada por los hilos secundarios
void* funcion_hilo(void* arg) {
    printf("Hilo [%lu] hará [%d] iteraciones\n", pthread_self(), max_iter);

    // Incremento de la variable compartida con protección de mutex
    for (int i = 0; i < max_iter; ++i) {
        pthread_mutex_lock(&mt); // Bloquea el mutex antes de acceder a la variable
        valor = valor + 1; // Incrementa la variable compartida
        pthread_mutex_unlock(&mt); // Desbloquea el mutex después de acceder a la variable
    }

    pthread_exit(0); // Termina la ejecución del hilo secundario
}