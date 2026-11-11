/*
 * shm3.c — Memoria compartida + señales: datos y aviso
 *
 * El padre escribe un vector en memoria compartida y avisa al hijo con
 * SIGUSR1. El hijo espera el aviso con pause() y solo entonces lee. La
 * memoria compartida transporta los datos; la señal indica cuándo están
 * listos.
 *
 * Observe: a diferencia de shm1.c, el hijo no gasta CPU esperando. El
 *          manejador se instala antes de fork() para que el hijo lo
 *          herede, y la pausa del padre le da tiempo de entrar en pause()
 *          (ver la carrera en 04-signal/s4.c).
 *
 * Norma:    System V IPC (shmget, shmat, shmdt, shmctl),
 *           ISO C (signal), POSIX (fork, kill, pause, wait)
 * Compilar: gcc -Wall -Wextra -o shm3 shm3.c
 * Ejecutar: ./shm3
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define N 10

static void manejador(int sig)
{
    (void)sig;
}

int main(void)
{
    int shm_id = shmget(IPC_PRIVATE, N * sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGUSR1, manejador) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    int *vector = shmat(shm_id, NULL, 0);
    if (vector == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("[%d] hijo esperando el aviso\n", (int)getpid());
        pause();
        printf("[%d] hijo lee:", (int)getpid());
        for (int i = 0; i < N; i++)
            printf(" %d", vector[i]);
        printf("\n");
        shmdt(vector);
        exit(EXIT_SUCCESS);
    }

    const int datos[N] = { 10, 20, 5, 0, 2, 14, 1, 8, 9, 0 };
    usleep(100000);
    for (int i = 0; i < N; i++)
        vector[i] = datos[i];
    printf("[%d] padre escribió el vector y avisa\n", (int)getpid());
    kill(pid, SIGUSR1);
    wait(NULL);

    shmdt(vector);
    shmctl(shm_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
