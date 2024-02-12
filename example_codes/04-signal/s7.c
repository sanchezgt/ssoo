/* 
Este programa en C crea una cadena de procesos donde cada proceso hijo envía una señal al siguiente.
El proceso padre inicia la cadena enviando una señal al último hijo, y cada hijo pasa la señal hacia
el padre. El usuario especifica cuántas veces se debe repetir este ciclo de señales.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o signal_chain signal_chain.c
4. Ejecuta el programa compilado y sigue las instrucciones en pantalla:
   ./signal_chain

El programa imprimirá el PID del proceso padre y de cada hijo, y mostrará el orden en que las señales
son enviadas y recibidas entre ellos.

*/

#include <stdio.h>    // Para funciones de entrada/salida como printf y scanf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>   // Para fork(), getpid(), getppid(), pause() y usleep()
#include <sys/wait.h> // Para wait()
#include <signal.h>   // Para signal(), SIGUSR1 y kill()

#define N_HIJOS 3     // Número de hijos a crear

// Declaración del manejador de señales
void sighandler(int sig);

int main() {
    pid_t pidhijo[N_HIJOS];
    int i, j, n, k;

    // Solicita al usuario el número de ciclos de señales
    printf("N:?");
    scanf("%d", &n);

    // Registra el manejador de señales para SIGUSR1
    oldhandler = signal(SIGUSR1, sighandler);
    if (oldhandler == SIG_ERR) {
        perror("signal:");
        exit(EXIT_FAILURE);
    }

    int root = getpid(); // Guarda el PID del proceso padre
    // Crea los procesos hijos
    for (i = 0; i < N_HIJOS; i++) {
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
        for (k = 0; k < n; k++) {
            usleep(10000); // Pequeña pausa
            printf("Padre [%d]\n", getpid());
            kill(pidhijo[N_HIJOS - 1], SIGUSR1); // Envía señal al último hijo
            pause(); // Espera recibir señal del primer hijo
            printf("Padre [%d]\n", getpid());
        }
        // Espera a que terminen todos los hijos
        for (j = 0; j < N_HIJOS; j++) wait(NULL);
    } else {
        // Procesos hijos
        for (k = 0; k < n; k++) {
            pause(); // Espera recibir señal
            printf("hijo[%d]\n", getpid());
            if (i != 0) {
                // Envía señal al hijo anterior en la cadena
                kill(pidhijo[i - 1], SIGUSR1);
            } else {
                // El primer hijo envía señal al padre
                kill(getppid(), SIGUSR1);
            }
        }
    }

    // Restaura el manejador de señales predeterminado para SIGUSR1
    signal(SIGUSR1, oldhandler);

    return EXIT_SUCCESS;
}

// Implementación del manejador de señales vacío
void sighandler(int sig) {
    // No se realiza ninguna acción al capturar la señal
}