/*
 * pipe5.c — Dos hijos, dos tuberías: el padre suma los resultados
 *
 * Cada hijo tiene su propia tubería hacia el padre y escribe en ella un
 * valor float. El padre lee de ambas tuberías y suma.
 *
 *     hijo 0 --tub[0]--> padre <--tub[1]-- hijo 1
 *
 * Observe: todos los procesos heredan los cuatro descriptores; cada uno
 *          cierra los que no usa. El padre lee primero tub[0]: si el
 *          hijo 0 tarda, el padre espera aunque tub[1] ya tenga datos.
 *          pipe8.c es la misma idea con el cálculo en una función.
 *
 * Norma:    POSIX (pipe, fork, read, write, close, wait)
 * Compilar: gcc -Wall -Wextra -o pipe5 pipe5.c
 * Ejecutar: ./pipe5
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 2

int main(void)
{
    int tub[N][2];
    int i;

    for (i = 0; i < N; i++) {
        if (pipe(tub[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
            break;
    }

    if (i < N) {
        /* Hijo i: solo escribe en tub[i]. */
        float valor = (i == 0) ? 10.3f : 8.4f;
        for (int k = 0; k < N; k++) {
            close(tub[k][0]);
            if (k != i)
                close(tub[k][1]);
        }
        if (write(tub[i][1], &valor, sizeof valor) == -1)
            perror("write");
        close(tub[i][1]);
        exit(EXIT_SUCCESS);
    }

    /* Padre: solo lee. */
    float datos[N], suma = 0;
    for (int k = 0; k < N; k++)
        close(tub[k][1]);
    for (int k = 0; k < N; k++) {
        if (read(tub[k][0], &datos[k], sizeof datos[k]) != (ssize_t)sizeof datos[k]) {
            fprintf(stderr, "hijo %d no envió datos\n", k);
            exit(EXIT_FAILURE);
        }
        printf("hijo %d <-- %.2f\n", k, datos[k]);
        suma += datos[k];
        close(tub[k][0]);
    }
    for (int k = 0; k < N; k++)
        wait(NULL);
    printf("suma total = %.2f\n", suma);

    return EXIT_SUCCESS;
}
