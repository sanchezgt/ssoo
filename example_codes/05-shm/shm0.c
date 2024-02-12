/* 
Este programa en C demuestra el uso de memoria compartida entre procesos en sistemas Unix.
El proceso padre crea un segmento de memoria compartida y luego crea un proceso hijo.
Ambos procesos incrementan un contador en la memoria compartida un número determinado de veces.
Al final, el proceso padre espera a que el hijo termine y luego imprime el valor acumulado
de los contadores.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o shared_memory_example shared_memory_example.c -lrt
4. Ejecuta el programa compilado:
   ./shared_memory_example

El programa imprimirá el valor acumulado de los contadores después de que ambos procesos
hayan terminado de incrementar sus respectivos contadores.

*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>   // Para fork(), shmat(), shmdt()
#include <sys/wait.h> // Para wait()
#include <sys/shm.h>  // Para shmget(), shmctl(), shmat(), shmdt()
#include <sys/stat.h> // Para constantes de modo de acceso

int main() {
    int shmid, max_iter = 10000000;
    int *pointer = NULL;

    // Crea un segmento de memoria compartida
    shmid = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    // Adjunta el segmento de memoria compartida al espacio de direcciones del proceso
    pointer = (int *)shmat(shmid, 0, 0);
    // Inicializa los contadores en la memoria compartida
    pointer[0] = pointer[1] = 0;

    // Crea un proceso hijo
    if (!fork()) {
        // Proceso hijo incrementa el contador 0
        for (int i = 0; i < max_iter; ++i) {
            pointer[0] = pointer[0] + 1;
        }
        // Desadjunta el segmento de memoria compartida
        shmdt(pointer);
    } else {
        // Proceso padre incrementa el contador 1
        for (int i = 0; i < max_iter; ++i) {
            pointer[1] = pointer[1] + 1;
        }
        // Espera a que el proceso hijo termine
        wait(NULL);
        // Imprime el valor acumulado de los contadores
        printf("the value of shm = [%d] \n", pointer[0] + pointer[1]);

        // Desadjunta y elimina el segmento de memoria compartida
        shmdt(pointer);
        shmctl(shmid, IPC_RMID, 0);
    }

    return EXIT_SUCCESS;
}