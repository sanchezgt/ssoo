/*
 * shm-ejm.c — Condición de carrera: versión mínima
 *
 * La misma carrera de shm-concurrency.c en el menor código posible: dos
 * procesos incrementan el mismo entero compartido sin sincronizarse.
 *
 * Observe: el valor final es menor que 2 x ITER y cambia en cada corrida.
 *
 * Norma:    System V IPC (shmget, shmat, shmdt, shmctl), POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o shm-ejm shm-ejm.c
 * Ejecutar: ./shm-ejm
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define ITER 5000000

int main(void)
{
    int shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    volatile int *p = shmat(shm_id, NULL, 0);
    if (shm_id == -1 || p == (void *)-1) {
        perror("memoria compartida");
        exit(EXIT_FAILURE);
    }
    *p = 0;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    /* Padre e hijo ejecutan el mismo ciclo sobre el mismo entero. */
    for (int i = 0; i < ITER; i++)
        *p = *p + 1;

    if (pid == 0) {
        shmdt((const void *)p);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    printf("valor = %d (esperado %d)\n", *p, 2 * ITER);
    shmdt((const void *)p);
    shmctl(shm_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
