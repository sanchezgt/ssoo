/*
Programa: Ejemplo de paso de estructura a un hilo en C
Descripción: Este programa crea un hilo secundario y le pasa una estructura como argumento.
             El hilo secundario imprime su identificador y los valores contenidos en la estructura.
             El programa ilustra cómo pasar una estructura a un hilo y cómo acceder a sus miembros.
Instrucciones de compilación: gcc -o ejemplo_hilos_estructura ejemplo_hilos_estructura.c -lpthread
Instrucciones de ejecución: ./ejemplo_hilos_estructura

Nota: Asegúrese de que la opción -lpthread esté incluida en la línea de compilación para vincular la biblioteca de hilos.
*/

#include <stdio.h>      // Incluye funciones estándar de entrada y salida
#include <unistd.h>     // Proporciona acceso a la API POSIX del sistema operativo
#include <stdlib.h>     // Incluye la definición de la macro EXIT_SUCCESS
#include <pthread.h>    // Incluye las funciones y tipos para hilos POSIX

// Prototipo de la función que será ejecutada por el hilo
void* funcion_hilo(void* arg);

// Definición de la estructura que se pasará al hilo
struct nodo {
    int a;
    float b;
};

int main() {
    pthread_t tid; // Variable para almacenar el identificador del hilo
    struct nodo dato; // Variable de la estructura nodo

    // Inicialización de los miembros de la estructura
    dato.a = 10;
    dato.b = 25.5;

    // Creación del hilo secundario
    pthread_create(&tid, NULL, funcion_hilo, (void*)&dato);
    printf("Hilo principal[%lu]\n", pthread_self()); // Imprime el identificador del hilo principal
    pthread_join(tid, NULL); // Espera a que el hilo secundario termine

    return EXIT_SUCCESS; // Finaliza el programa con éxito
}

// Función que será ejecutada por el hilo secundario
void* funcion_hilo(void* arg) {
    struct nodo* p = (struct nodo *)arg; // Conversión del argumento a un puntero a la estructura nodo
    printf("Hilo [%lu] dato[%d | %f]\n", pthread_self(), p->a, p->b); // Imprime el identificador del hilo y los valores de la estructura
    pthread_exit(0); // Termina la ejecución del hilo secundario
}
