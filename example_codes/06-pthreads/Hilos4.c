/*
Programa: Ejemplo de creación de múltiples hilos en C y acceso a una variable global
Descripción: Este programa crea varios hilos secundarios y les pasa una estructura como argumento.
             Cada hilo imprime su identificador y el valor entero contenido en la estructura.
             Además, cada hilo modifica una variable global llamada 'numero'.
             El programa ilustra el uso de variables globales y cómo pueden ser modificadas por hilos.
             Existe un problema de condición de carrera ya que varios hilos intentan modificar 'numero' simultáneamente.
Instrucciones de compilación: gcc -o ejemplo_hilos_multiples ejemplo_hilos_multiples.c -lpthread
Instrucciones de ejecución: ./ejemplo_hilos_multiples
*/

#include <stdio.h>      // Incluye funciones estándar de entrada y salida
#include <unistd.h>     // Proporciona acceso a la API POSIX del sistema operativo
#include <stdlib.h>     // Incluye la definición de la macro EXIT_SUCCESS
#include <pthread.h>    // Incluye las funciones y tipos para hilos POSIX

// Prototipo de la función que será ejecutada por los hilos
void* funcion_hilo(void* arg);

// Definición de la estructura que se pasará a los hilos
struct nodo {
    int a;	
};

// Declaración de la variable global que será modificada por los hilos
int numero = 0;

int main() {
    int maxhilos = 4; // Número máximo de hilos a crear
    pthread_t tid[maxhilos]; // Array para almacenar los identificadores de los hilos
    struct nodo dato; // Variable de la estructura nodo

    // Creación de los hilos secundarios
    for (int i = 0; i < maxhilos; i++) {
        dato.a = i; // Asignación del valor a la estructura
        pthread_create(&tid[i], NULL, funcion_hilo, (void*)&dato);
        usleep(100); // Pequeña pausa para reducir la condición de carrera
    }

    printf("Hilo principal[%lu]\n", pthread_self()); // Imprime el identificador del hilo principal

    // Espera a que todos los hilos secundarios terminen
    for (int i = 0; i < maxhilos; i++) {
        pthread_join(tid[i], NULL);
    }

    // Imprime el valor de la variable global después de la ejecución de los hilos
    printf("%d\n", numero);

    return EXIT_SUCCESS; // Finaliza el programa con éxito
}

// Función que será ejecutada por los hilos secundarios
void* funcion_hilo(void* arg) {
    struct nodo* p = (struct nodo *)arg; // Conversión del argumento a un puntero a la estructura nodo
    printf("Hilo [%lu] dato[%d]\n", pthread_self(), p->a); // Imprime el identificador del hilo y el valor de la estructura
    numero = 500; // Modifica la variable global 'numero'
    pthread_exit(0); // Termina la ejecución del hilo secundario
}
