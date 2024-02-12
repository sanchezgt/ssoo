/*
Programa: Ejemplo de creación de múltiples hilos con asignación dinámica en C
Descripción: Este programa crea un número determinado de hilos secundarios y les pasa una estructura
             con asignación dinámica de memoria como argumento. Cada hilo imprime su identificador
             y el valor entero contenido en la estructura. El programa ilustra cómo pasar datos
             dinámicamente asignados a hilos y la importancia de liberar dicha memoria adecuadamente.
Instrucciones de compilación: gcc -o ejemplo_hilos_dinamicos ejemplo_hilos_dinamicos.c -lpthread
Instrucciones de ejecución: ./ejemplo_hilos_dinamicos
*/

#include <stdio.h>      // Incluye funciones estándar de entrada y salida
#include <unistd.h>     // Proporciona acceso a la API POSIX del sistema operativo
#include <stdlib.h>     // Incluye funciones de manejo de memoria dinámica y la macro EXIT_SUCCESS
#include <pthread.h>    // Incluye las funciones y tipos para hilos POSIX

// Prototipo de la función que será ejecutada por los hilos
void* funcion_hilo(void* arg);

// Definición de la estructura que se pasará a los hilos
struct nodo {
    int a;	
};

int main() {
    int maxhilos = 4; // Número máximo de hilos a crear
    pthread_t tid[maxhilos]; // Array para almacenar los identificadores de los hilos
    struct nodo *dato; // Puntero a la estructura nodo

    // Creación de los hilos secundarios con asignación dinámica de la estructura
    for (int i = 0; i < maxhilos; i++) {
        dato = (struct nodo*)malloc(sizeof(struct nodo)); // Asignación dinámica de memoria para la estructura
        if (dato == NULL) {
            perror("Error en malloc");
            exit(EXIT_FAILURE);
        }
        dato->a = i; // Asignación del valor a la estructura
        pthread_create(&tid[i], NULL, funcion_hilo, (void*)dato);
    }

    printf("Hilo principal[%lu]\n", pthread_self()); // Imprime el identificador del hilo principal

    // Espera a que todos los hilos secundarios terminen
    for (int i = 0; i < maxhilos; i++) {
        pthread_join(tid[i], NULL);
    }

    return EXIT_SUCCESS; // Finaliza el programa con éxito
}

// Función que será ejecutada por los hilos secundarios
void* funcion_hilo(void* arg) {
    struct nodo* p = (struct nodo *)arg; // Conversión del argumento a un puntero a la estructura nodo
    printf("Hilo [%lu] dato[%d]\n", pthread_self(), p->a); // Imprime el identificador del hilo y el valor de la estructura
    free(p); // Libera la memoria asignada dinámicamente para la estructura
    pthread_exit(0); // Termina la ejecución del hilo secundario
}