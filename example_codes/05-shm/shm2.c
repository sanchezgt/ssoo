/*
 * shm2.c — Mensajes de texto por memoria compartida
 *
 * El padre lee líneas del teclado y las copia en un segmento compartido.
 * El hijo revisa el segmento cada milisegundo y, si el texto cambió, lo
 * muestra. Ambos terminan cuando el texto es "salir".
 *
 * Observe: no hay ninguna sincronización, y se nota:
 *          - si escribe dos veces la misma línea, el hijo no ve cambio;
 *          - si el padre escribe dos líneas seguidas antes de que el hijo
 *            mire, la primera se pierde (pruebe pegando varias líneas);
 *          - el hijo podría leer una cadena a medio copiar.
 *          A diferencia de una tubería, la memoria compartida no guarda
 *          una cola de mensajes: solo el último valor escrito.
 *
 * Norma:    System V IPC (shmget, shmat, shmdt, shmctl), POSIX (fork, wait)
 * Compilar: gcc -Wall -Wextra -o shm2 shm2.c
 * Ejecutar: ./shm2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define TAM 256

int main(void)
{
    int shm_id = shmget(IPC_PRIVATE, TAM, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    char *texto = shmat(shm_id, NULL, 0);
    if (texto == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        char anterior[TAM] = "";
        printf("[%d] hijo leyendo la memoria compartida\n", (int)getpid());
        while (strcmp(texto, "salir") != 0) {
            if (strcmp(texto, anterior) != 0) {
                printf("<-- %s\n", texto);
                strcpy(anterior, texto);
            }
            usleep(1000);
        }
        printf("[%d] hijo termina\n", (int)getpid());
        shmdt(texto);
        exit(EXIT_SUCCESS);
    }

    char linea[TAM];
    printf("[%d] padre: escriba mensajes, \"salir\" para terminar\n", (int)getpid());
    do {
        if (fgets(linea, sizeof linea, stdin) == NULL)
            strcpy(linea, "salir");
        linea[strcspn(linea, "\n")] = '\0';
        strcpy(texto, linea);
    } while (strcmp(linea, "salir") != 0);

    wait(NULL);
    shmdt(texto);
    shmctl(shm_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
