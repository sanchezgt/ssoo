/*
 * p1.c — fork(): un programa, dos procesos
 *
 * fork() duplica el proceso que la llama. Desde ese punto hay dos
 * procesos ejecutando el mismo código; lo único que los distingue es el
 * valor de retorno: 0 en el hijo, el PID del hijo en el padre y -1 si
 * hubo error (en ese caso no se creó ningún hijo).
 *
 * Observe: ejecútelo varias veces. El orden de las dos líneas lo decide
 *          el planificador, no el programa.
 *
 * Norma:    POSIX (fork)
 * Compilar: gcc -Wall -Wextra -o p1 p1.c
 * Ejecutar: ./p1
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        printf("Proceso hijo\n");
        break;
    default:
        printf("Proceso padre\n");
        break;
    }

    return EXIT_SUCCESS;
}
