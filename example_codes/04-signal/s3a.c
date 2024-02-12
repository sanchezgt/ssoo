/* 
Este programa en C crea un proceso hijo que espera recibir una señal SIGUSR1 del proceso padre.
Una vez que el proceso hijo recibe la señal, imprime un mensaje indicando que la señal fue capturada
y luego termina. El proceso padre espera a que el proceso hijo termine antes de finalizar.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o signal_handling signal_handling.c
4. Ejecuta el programa compilado:
   ./signal_handling

El programa imprimirá el PID del proceso padre, enviará una señal al proceso hijo,
y el proceso hijo imprimirá un mensaje antes de terminar.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>   // Para fork(), getpid(), usleep() y pause()
#include <sys/wait.h> // Para wait()
#include <signal.h>   // Para signal(), SIGUSR1 y kill()

// Declaración del manejador de señales
void sighandler(int sig);

int main() {
    pid_t pidhijo;
    void (*oldhandler)(int);

    // Crea un proceso hijo
    pidhijo = fork();
    
    // Manejo de errores y comportamiento del proceso hijo y padre
    switch (pidhijo) {
        case -1: // Error al crear el proceso hijo
            perror("fork");
            exit(EXIT_FAILURE);
        case 0: // Proceso hijo
            // Registra el manejador de señales personalizado para SIGUSR1
            oldhandler = signal(SIGUSR1, sighandler);
            if (oldhandler == SIG_ERR) {
                perror("signal:");
                exit(EXIT_FAILURE);
            }
            
            pause(); // Espera a recibir una señal
            
            printf("[%d] Terminando\n", getpid());
            
            // Restaura el manejador de señales predeterminado para SIGUSR1
            if (signal(SIGUSR1, oldhandler) == SIG_ERR) {
                perror("signal:");
                exit(EXIT_FAILURE);
            }
            break;
        default: // Proceso padre
            printf("[%d] Señal enviada\n", getpid());
            usleep(200); // Pequeña pausa para asegurar que el hijo esté listo
            kill(pidhijo, SIGUSR1); // Envía la señal SIGUSR1 al proceso hijo
            wait(NULL); // Espera a que el proceso hijo termine
    }

    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente
}

// Implementación del manejador de señales personalizado
void sighandler(int sig) {
    printf("[%d] sig %d capturada\n", getpid(), sig);
}