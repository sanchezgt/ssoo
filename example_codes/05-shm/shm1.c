/*
 * shm1.c — Una bandera en memoria compartida
 *
 * El hijo espera a que el padre cambie un entero compartido de 0 a 1.
 * Mientras espera, revisa la bandera cada 2 ms e imprime un punto. El
 * padre la cambia al cabo de 2 segundos.
 *
 * Observe: cada punto es una consulta inútil: el hijo gasta CPU
 *          preguntando. Es una espera activa; los semáforos o las señales
 *          (shm3.c) evitan ese gasto. Un segmento nuevo de System V
 *          empieza lleno de ceros, por eso el hijo no lee basura aunque
 *          arranque antes que el padre.
 *
 * Norma:    System V IPC (shmget, shmat, shmdt, shmctl), POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o shm1 shm1.c
 * Ejecutar: ./shm1
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void)
{
    int shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    volatile int *bandera = shmat(shm_id, NULL, 0);
    if (bandera == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("[%d] hijo esperando la bandera\n", (int)getpid());
        while (*bandera != 1) {
            usleep(2000);
            putchar('.');
            fflush(stdout);
        }
        printf("\n[%d] hijo ve bandera=%d\n", (int)getpid(), *bandera);
        shmdt((const void *)bandera);
        exit(EXIT_SUCCESS);
    }

    printf("[%d] padre trabajando 2 s\n", (int)getpid());
    sleep(2);
    *bandera = 1;
    printf("\n[%d] padre pone bandera=1\n", (int)getpid());
    wait(NULL);

    shmdt((const void *)bandera);
    shmctl(shm_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
