/* 
Este programa en C demuestra la comunicación entre procesos usando señales en un entorno Unix.
El proceso padre crea dos procesos hijos. El primer hijo envía una señal al segundo hijo, y luego
el segundo hijo envía una señal de vuelta al padre. Cada proceso espera recibir una señal antes
de continuar y luego imprime su identificador de proceso (PID) antes de terminar.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o signal_communication signal_communication.c
4. Ejecuta el programa compilado:
   ./signal_communication

El programa imprimirá el PID del proceso padre y de los hijos, y mostrará el orden en que las señales
son enviadas y recibidas entre ellos.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>   // Para fork(), getpid(), getppid(), pause() y usleep()
#include <sys/wait.h> // Para wait()
#include <signal.h>   // Para signal(), SIGUSR1 y kill()

// Declaración del manejador de señales vacío
void sighandler(int sig);

int main() {
    pid_t pidhijo[2], root;
    int i, j;
    
    // Registra el manejador de señales vacío para SIGUSR1
    oldhandler = signal(SIGUSR1, sighandler);
    if (oldhandler == SIG_ERR) {
        perror("signal:");
        exit(EXIT_FAILURE);
    }
    
    root = getpid(); // Guarda el PID del proceso padre
    for (i = 0; i < 2; i++) {
        pidhijo[i] = fork();
        if (pidhijo[i] == -1) {
            perror("fork:");
            exit(EXIT_FAILURE);
        }
        if (pidhijo[i] == 0) { // Si es un proceso hijo, sale del bucle
            break;
        }
    }
    
    if (root == getpid()) {
        // Proceso padre
        printf("Padre[%d]\n", getpid());
        usleep(100);
        kill(pidhijo[1], SIGUSR1); // Envía señal al segundo hijo
        pause(); // Espera recibir señal del primer hijo
        printf("Padre[%d]\n", getpid());
        for (j = 0; j < 2; j++) wait(NULL); // Espera a que terminen ambos hijos
    } else {
        // Procesos hijos
        pause(); // Espera recibir señal
        printf("hijo[%d]\n", getpid());
        if (i == 1) {
            kill(pidhijo[0], SIGUSR1); // Segundo hijo envía señal al primer hijo
        } else {
            kill(getppid(), SIGUSR1); // Primer hijo envía señal al padre
        }
    }

    return EXIT_SUCCESS;
}

// Implementación del manejador de señales vacío
void sighandler(int sig) {
    // No se realiza ninguna acción al capturar la señal
}