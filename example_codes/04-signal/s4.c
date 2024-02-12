/* 
Este programa en C demuestra el uso de señales y procesos en sistemas operativos tipo Unix.
El proceso padre crea un proceso hijo y le envía una señal SIGUSR1. El proceso hijo espera
hasta que recibe la señal y luego imprime un mensaje antes de terminar. El proceso padre
espera a que el proceso hijo termine antes de restaurar el manejador de señales predeterminado.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o signal_demo signal_demo.c
4. Ejecuta el programa compilado:
   ./signal_demo

El programa imprimirá el PID del proceso padre y del hijo, indicará cuando la señal ha sido
enviada y recibida, y cuando el proceso hijo está terminando.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>   // Para fork(), getpid(), pause()
#include <sys/wait.h> // Para wait()
#include <signal.h>   // Para signal(), SIGUSR1 y kill()

// Declaración del manejador de señales
void sighandler(int sig);

int main() {
    pid_t pidhijo;
    void (*oldhandler)(int);

    // Registra el manejador de señales personalizado para SIGUSR1
    oldhandler = signal(SIGUSR1, sighandler);
    if (oldhandler == SIG_ERR) {
        perror("signal:");
        exit(EXIT_FAILURE);
    }

    // Crea un proceso hijo
    pidhijo = fork();
    
    // Manejo de errores y comportamiento del proceso hijo y padre
    switch (pidhijo) {
        case -1: // Error al crear el proceso hijo
            perror("fork");
            exit(EXIT_FAILURE);
        case 0: // Proceso hijo
            pause(); // Espera a recibir una señal
            printf("[%d] Terminando\n", getpid());
            break;
        default: // Proceso padre
            // usleep(100); // Pequeña pausa para asegurar que el hijo esté listo (comentado)
            kill(pidhijo, SIGUSR1); // Envía la señal SIGUSR1 al proceso hijo
            printf("[%d] Señal enviada\n", getpid());
            wait(NULL); // Espera a que el proceso hijo termine
    }

    // Restaura el manejador de señales predeterminado para SIGUSR1
    if (signal(SIGUSR1, oldhandler) == SIG_ERR) {
        perror("signal:");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente
}

// Implementación del manejador de señales personalizado
void sighandler(int sig) {
    printf("[%d] sig %d capturada\n", getpid(), sig);
}