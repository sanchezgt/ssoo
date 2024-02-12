/* 
Este programa en C demuestra el uso de memoria compartida entre procesos padre e hijo en sistemas Unix.
El proceso padre crea un segmento de memoria compartida, luego crea un proceso hijo que espera a que
el valor en la memoria compartida cambie a 1. Mientras tanto, el proceso padre duerme durante 2 segundos
y luego cambia el valor en la memoria compartida a 1, lo que permite que el proceso hijo continúe y termine.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o shared_memory_demo shared_memory_demo.c
4. Ejecuta el programa compilado:
   ./shared_memory_demo

El programa imprimirá mensajes del proceso padre e hijo, mostrando el uso de la memoria compartida para
sincronizar una acción simple entre ellos.

*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>   // Para fork(), sleep(), usleep()
#include <sys/wait.h> // Para wait()
#include <sys/shm.h>  // Para shmget(), shmctl(), shmat(), shmdt()
#include <sys/stat.h> // Para constantes de modo de acceso

int main() {
    int *ptr = NULL;
    int shm_id, c = 0;
    int shm_size = sizeof(int);

    // Crea un segmento de memoria compartida
    shm_id = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);

    // Crea un proceso hijo
    if (fork() == 0) {
        // Proceso hijo
        printf("[%d]child process started\n", getpid());
        // Adjunta el segmento de memoria compartida al espacio de direcciones del proceso hijo
        ptr = (int *)shmat(shm_id, 0, 0);
        // Espera hasta que el valor en la memoria compartida cambie a 1
        do {
            usleep(2000); // Duerme por 2 milisegundos
            printf(".");
            fflush(stdout); // Fuerza la escritura de los datos en el buffer de stdout
        } while (*ptr != 1);
        printf("[%d]shm_value->%d\n", getpid(), *ptr);

        // Desadjunta el segmento de memoria compartida
        shmdt(ptr);
    } else {
        // Proceso padre
        // Adjunta el segmento de memoria compartida al espacio de direcciones del proceso padre
        ptr = (int *)shmat(shm_id, 0, 0);
        // Inicializa el valor en la memoria compartida a 0
        *ptr = 0;
        printf("[%d]parent process started\n", getpid());
        sleep(2); // Duerme por 2 segundos
        // Cambia el valor en la memoria compartida a 1
        *ptr = 1;
        printf("[%d]shm_value->%d\n", getpid(), *ptr);
        // Espera a que el proceso hijo termine
        wait(NULL);

        // Desadjunta y elimina el segmento de memoria compartida
        shmdt(ptr);
        shmctl(shm_id, IPC_RMID, NULL);
    }
    return EXIT_SUCCESS;
}
