/*
Este programa en C crea un hilo secundario y muestra los identificadores del hilo principal y del hilo secundario.
Para compilar este programa, use el siguiente comando:
gcc -o nombre_del_programa archivo_fuente.c -lpthread

Para ejecutar el programa compilado, use el siguiente comando:
./nombre_del_programa

Instrucciones:
1. Compile el programa con el comando proporcionado.
2. Ejecute el programa compilado.
3. Observe la salida que muestra los identificadores de los hilos.
*/

#include <stdio.h>      // Librería estándar de entrada y salida
#include <unistd.h>     // Proporciona acceso a la API POSIX del sistema operativo
#include <stdlib.h>     // Para usar la macro EXIT_SUCCESS
#include <pthread.h>    // Para usar funciones de hilos POSIX

// Declaración de funciones para los hilos
void* funcion_hilo1(void*);

// Declaración del identificador del hilo
pthread_t tid1;

int main(){
    // Creación del hilo secundario
    pthread_create(&tid1, NULL, funcion_hilo1, NULL); // Crea un hilo que ejecuta funcion_hilo1
    printf("Hilo principal[%lu]\n", pthread_self()); // Imprime el identificador del hilo principal

    // El hilo principal podría esperar al hilo secundario si se descomenta la siguiente línea
    // pthread_join(tid1, NULL); // Espera a que el hilo secundario termine

    return EXIT_SUCCESS; // Finaliza el programa con éxito
}

// Función que ejecuta el hilo secundario
void* funcion_hilo1(void* arg){
    printf("Hilo1 [%lu]\n", pthread_self()); // Imprime el identificador del hilo secundario
    pthread_exit(0); // Termina la ejecución del hilo secundario
}
