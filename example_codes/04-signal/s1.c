/* 
Este programa en C demuestra el manejo de señales utilizando la función signal.
Registra un manejador de señales personalizado para la señal SIGUSR1 y luego
restaura el manejador de señales predeterminado antes de finalizar el programa.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o signal_handling signal_handling.c
4. Ejecuta el programa compilado:
   ./signal_handling
5. En otra terminal, encuentra el PID del proceso ejecutado y envía la señal SIGUSR1:
   kill -SIGUSR1 <PID>
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <signal.h>   // Para signal y SIGUSR1

// Declaración del manejador de señales
void sighandler(int sig);

int main() {
    // Guarda el manejador de señales anterior
    void (*oldhandler)(int);

    // Registra el manejador de señales personalizado para SIGUSR1
    oldhandler = signal(SIGUSR1, sighandler);
    if (oldhandler == SIG_ERR) {
        perror("signal:"); // Imprime un mensaje de error si falla
        exit(EXIT_FAILURE);
    }

    // ...código que puede generar o esperar la señal SIGUSR1...

    // Restaura el manejador de señales predeterminado para SIGUSR1
    if (signal(SIGUSR1, oldhandler) == SIG_ERR) {
        perror("signal:"); // Imprime un mensaje de error si falla
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente
}

// Implementación del manejador de señales personalizado
void sighandler(int sig) {
    printf("Señal recibida %d\n", sig);
}