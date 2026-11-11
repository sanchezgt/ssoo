/*
 * shm0.c — Memoria compartida sin conflicto: un contador por proceso
 *
 * El padre crea un segmento de memoria compartida con dos enteros y luego
 * crea un hijo. Cada proceso incrementa SU propio entero, así que nunca
 * escriben en la misma posición y el resultado siempre es exacto.
 *
 * Ciclo de vida de un segmento System V:
 *   shmget  crea el segmento (o obtiene uno existente) y devuelve su id
 *   shmat   lo conecta al espacio de direcciones del proceso
 *   shmdt   lo desconecta
 *   shmctl  con IPC_RMID lo marca para borrarlo
 *
 * Observe: siempre imprime 2 x ITER. Compare con shm-concurrency.c, donde
 *          ambos procesos escriben el mismo entero. Con  ipcs -m  puede
 *          ver los segmentos que existen en el sistema.
 *
 * Norma:    System V IPC (shmget, shmat, shmdt, shmctl), POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o shm0 shm0.c
 * Ejecutar: ./shm0
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
    int shm_id = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    /* volatile: cada incremento lee y escribe la memoria de verdad. */
    volatile int *contador = shmat(shm_id, NULL, 0);
    if (contador == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    contador[0] = contador[1] = 0;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        for (int i = 0; i < ITER; i++)
            contador[0] = contador[0] + 1;
        shmdt((const void *)contador);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < ITER; i++)
        contador[1] = contador[1] + 1;
    wait(NULL);

    printf("hijo=%d padre=%d total=%d (esperado %d)\n",
           contador[0], contador[1], contador[0] + contador[1], 2 * ITER);

    shmdt((const void *)contador);
    shmctl(shm_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
