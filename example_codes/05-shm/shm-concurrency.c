/*
 * shm-concurrency.c — Condición de carrera en memoria compartida
 *
 * Padre e hijo incrementan ITER veces el MISMO entero compartido. Cada
 * incremento son tres pasos (leer, sumar, escribir); si los dos procesos
 * los intercalan, uno sobrescribe el resultado del otro y se pierden
 * incrementos.
 *
 * Observe: el total casi nunca es 2 x ITER y cambia en cada ejecución.
 *          Con una sola CPU ( taskset -c 0 ./shm-concurrency ) se pierden
 *          menos, pero se siguen perdiendo. Compare con shm0.c. La
 *          solución es una sección crítica protegida, por ejemplo con
 *          un semáforo.
 *
 * Norma:    System V IPC (shmget, shmat, shmdt, shmctl), POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o shm-concurrency shm-concurrency.c
 * Ejecutar: ./shm-concurrency
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define ITER 10000000

int main(void)
{
    int shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    /* volatile: sin él, el compilador podría sumar ITER de una sola vez
     * y la carrera no se vería. */
    volatile int *valor = shmat(shm_id, NULL, 0);
    if (valor == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    *valor = 0;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("[%d] hijo inicia\n", (int)getpid());
        for (int i = 0; i < ITER; i++)
            *valor = *valor + 1;
        shmdt((const void *)valor);
        exit(EXIT_SUCCESS);
    }

    printf("[%d] padre inicia\n", (int)getpid());
    for (int i = 0; i < ITER; i++)
        *valor = *valor + 1;
    wait(NULL);

    printf("valor=%d (esperado %d, se perdieron %d)\n", *valor, 2 * ITER, 2 * ITER - *valor);

    shmdt((const void *)valor);
    shmctl(shm_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
