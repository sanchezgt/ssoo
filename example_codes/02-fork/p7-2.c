/*
 * p7-2.c — El árbol de p7.c visto con pstree
 *
 *          padre
 *            |
 *           h0
 *        /   |   \
 *      n0    n1    n2
 *
 * Misma estructura que p7.c. El padre espera un segundo para que todos
 * los procesos existan y dibuja el árbol con  pstree -cAlp <pid>. Luego
 * cada proceso imprime qué hijo recogió con wait().
 *
 * Observe: el padre recoge solo a h0; los nietos los recoge h0.
 *
 * Norma:    POSIX (fork, wait), ISO C (system)
 * Compilar: gcc -Wall -Wextra -o p7-2 p7-2.c
 * Ejecutar: ./p7-2        (requiere pstree: paquete psmisc)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 3

static void mostrar_arbol(void)
{
    char orden[64];
    snprintf(orden, sizeof orden, "pstree -cAlp %d", (int)getpid());
    if (system(orden) != 0)
        fprintf(stderr, "No se pudo ejecutar pstree\n");
}

int main(void)
{
    pid_t recogido;
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        int i;
        for (i = 0; i < N; i++) {
            pid_t nieto = fork();
            if (nieto == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (nieto == 0)
                break;
        }
        if (i == N) {
            for (int j = 0; j < N; j++) {
                recogido = wait(NULL);
                printf("%d recogió a %d\n", (int)getpid(), (int)recogido);
            }
        } else {
            sleep(2);
        }
    } else {
        sleep(1);
        mostrar_arbol();
        recogido = wait(NULL);
        printf("%d recogió a %d\n", (int)getpid(), (int)recogido);
    }

    return EXIT_SUCCESS;
}
