/*
Programa: Incremento Concurrente
Descripción: Este programa demuestra el uso de la memoria compartida en C para realizar un incremento concurrente de una variable.
Instrucciones de compilación: gcc -o incremento_concurrente incremento_concurrente.c -lrt
Instrucciones de ejecución: ./incremento_concurrente
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(){
    // Inicialización de variables
    int *ptr;
    int shm_id, i, j, c=0, max_iter=10000000;
    int shm_size = sizeof(int);

    // Creación de la memoria compartida
    shm_id=shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
    ptr = (int *) shmat(shm_id, 0, 0);
    *ptr = 0;

    if(!fork()){
        // Proceso hijo
        printf("[%d] Proceso hijo iniciado\n", getpid() );

        for (i = 0; i < max_iter; ++i)
        {
            *ptr = *ptr+1;
            //usleep(2);
        }
        //printf("[%d]--[%d] iterations *ptr->%d\n", getpid(),i,*ptr);
        shmdt(ptr);
    }
    else{
        // Proceso padre
        printf("[%d] Proceso padre iniciado\n", getpid() );

        for (j = 0; j < max_iter; ++j)
        {
            *ptr = *ptr+1;
            //usleep(1);
        }

        wait(NULL);

        printf("[%d]--[%d] iterations *ptr->%d\n", getpid(), j,*ptr);
        shmdt(ptr);
        shmctl(shm_id, IPC_RMID, 0);
    }
    return EXIT_SUCCESS;
}
