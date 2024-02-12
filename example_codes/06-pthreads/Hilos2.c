/*
Programa: Ejemplo de Hilos en C
Descripción: Este programa crea un hilo secundario y le pasa un valor entero como argumento. 
             Luego, el hilo secundario imprime su identificador y el valor entero recibido.
Instrucciones de compilación: gcc -o ejemplo_hilos ejemplo_hilos.c -lpthread
Instrucciones de ejecución: ./ejemplo_hilos

Nota: Asegúrese de que la opción -lpthread esté incluida en la línea de compilación para vincular la biblioteca de hilos.
*/

#include <stdio.h>      // Incluye funciones estándar de entrada y salida
#include <unistd.h>     // Proporciona acceso a la API POSIX del sistema operativo
#include <stdlib.h>     // Incluye la definición de la macro EXIT_SUCCESS
#include <pthread.h>    // Incluye las funciones y tipos para hilos POSIX

// Prototipo de la función que será ejecutada por el hilo
void* funcion_hilo(void* arg);

int main(){
    pthread_t tid; // Variable para almacenar el identificador del hilo
    int *dato;     // Puntero para almacenar el dato que se pasará al hilo
    
    dato = malloc(sizeof(int)); // Asignación dinámica de memoria para el dato
    *dato = 10; // Asignación del valor al dato

    // Creación del hilo secundario
    pthread_create(&tid, NULL, funcion_hilo, (void*) dato);
    printf("Hilo principal[%lu]\n", pthread_self()); // Imprime el identificador del hilo principal
    pthread_join(tid, NULL); // Espera a que el hilo secundario termine

    free(dato); // Liberación de la memoria asignada dinámicamente
    return EXIT_SUCCESS; // Finaliza el programa con éxito
}

// Función que será ejecutada por el hilo secundario
void* funcion_hilo(void* arg){
    int* parametro = (int *)arg; // Conversión del argumento a un puntero a entero
    printf("Hilo [%lu] dato[%d]\n", pthread_self(), *parametro); // Imprime el identificador del hilo y el valor del dato
    pthread_exit(0); // Termina la ejecución del hilo secundario
}
