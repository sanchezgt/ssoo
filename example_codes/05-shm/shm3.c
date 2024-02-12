/* 
Este programa en C demuestra la comunicación entre procesos utilizando memoria compartida y señales.
El proceso padre crea un segmento de memoria compartida, escribe un vector de enteros en él y luego
envía una señal al proceso hijo. El proceso hijo, después de recibir la señal, lee y muestra los valores
del vector desde la memoria compartida.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o shared_memory_signal shared_memory_signal.c
4. Ejecuta el programa compilado:
   ./shared_memory_signal

El programa imprimirá los valores escritos por el proceso padre y luego leídos por el proceso hijo.

*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>   // Para fork(), getpid(), usleep(), pause(), kill()
#include <signal.h>   // Para signal(), SIGUSR1
#include <sys/wait.h> // Para wait()
#include <sys/shm.h>  // Para shmget(), shmctl(), shmat(), shmdt()
#include <sys/stat.h> // Para constantes de modo de acceso

// Manejador de señales vacío para capturar SIGUSR1
void sig_handler(int s) {}

int main() {
    pid_t child;
    int *ptr = NULL;
    int shm_id;
    int shm_size = sizeof(int) * 10; // Tamaño del segmento de memoria compartida

    // Crea un segmento de memoria compartida
    shm_id = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
    // Configura el manejador de señales para SIGUSR1
    signal(SIGUSR1, sig_handler);

    // Crea un proceso hijo
    child = fork();
    if (!child) {
        // Proceso hijo
        printf("[%d]child process started\n", getpid());
        // Adjunta el segmento de memoria compartida al espacio de direcciones del proceso hijo
        ptr = (int *)shmat(shm_id, 0, 0);
        // Espera a recibir la señal del proceso padre
        pause();
        printf("[%d]reading shm_values:\n", getpid());
        // Lee y muestra los valores del vector desde la memoria compartida
        for (int i = 0; i < 10; i++)
            printf("%d ", ptr[i]);
        printf("\n");
        // Desadjunta el segmento de memoria compartida
        shmdt(ptr);
    } else {
        // Proceso padre
        printf("[%d]parent process started\n", getpid());
        // Vector de enteros a escribir en la memoria compartida
        int vect[10] = {10, 20, 5, 0, 2, 14, 1, 8, 9, 0};
        // Adjunta el segmento de memoria compartida al espacio de direcciones del proceso padre
        ptr = (int *)shmat(shm_id, 0, 0);

        usleep(3000); // Duerme por 3 milisegundos para dar tiempo al hijo de iniciar
        // Escribe el vector en la memoria compartida
        for (int i = 0; i < 10; i++)
            ptr[i] = vect[i];
        printf("[%d]written shm_values.\n", getpid());
        // Envía la señal SIGUSR1 al proceso hijo
        kill(child, SIGUSR1);
        // Espera a que el proceso hijo termine
        wait(NULL);

        // Desadjunta y elimina el segmento de memoria compartida
        shmdt(ptr);
        shmctl(shm_id, IPC_RMID, 0);
    }
    return EXIT_SUCCESS;
}