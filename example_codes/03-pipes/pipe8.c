/*
 * pipe8.c — Reparto de trabajo entre hijos con tuberías
 *
 * Variante de pipe5.c organizada como reparto de trabajo: cada hijo
 * calcula su parte (aquí, una función de su índice) y la devuelve al
 * padre por su propia tubería. El padre espera a ambos y suma.
 *
 *     hijo 0 --tub[0]--> padre <--tub[1]-- hijo 1
 *
 * Observe: para más hijos basta con cambiar N. El padre podría enviar a
 *          cada hijo sus datos de entrada por una segunda tubería.
 *
 * Norma:    POSIX (pipe, fork, read, write, close, wait)
 * Compilar: gcc -Wall -Wextra -o pipe8 pipe8.c
 * Ejecutar: ./pipe8
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 2

static float calcular(int i)
{
    static const float resultados[N] = { 25.32f, 14.1f };
    return resultados[i];
}

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
        float dato = calcular(i);
        for (int k = 0; k < N; k++) {
            close(tub[k][0]);
            if (k != i)
                close(tub[k][1]);
        }
        if (write(tub[i][1], &dato, sizeof dato) == -1)
            perror("write");
        printf("[%d] hijo %d --> %.2f\n", (int)getpid(), i, dato);
        close(tub[i][1]);
        exit(EXIT_SUCCESS);
    }

    float datos[N], suma = 0;
    for (int k = 0; k < N; k++)
        close(tub[k][1]);
    for (int k = 0; k < N; k++) {
        if (read(tub[k][0], &datos[k], sizeof datos[k]) == (ssize_t)sizeof datos[k])
            suma += datos[k];
        close(tub[k][0]);
    }
    for (int k = 0; k < N; k++)
        wait(NULL);
    printf("suma total = %.2f\n", suma);

    return EXIT_SUCCESS;
}
