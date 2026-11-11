/*
 * p6-2.c — El árbol de p6.c visto con pstree
 *
 *          padre
 *        /   |   \
 *      h0    h1    h2
 *      |           |
 *      n0          n2
 *
 * Misma estructura que p6.c. El padre espera un segundo para que todos
 * los procesos existan y dibuja el árbol con  pstree -cAlp <pid>. Luego
 * cada proceso imprime qué hijo recogió con wait().
 *
 * Observe: en el árbol también aparece "sh" y "pstree": system() crea
 *          dos procesos más para ejecutar la orden. Los hijos duermen 2 s
 *          para seguir vivos mientras se dibuja el árbol.
 *
 * Norma:    POSIX (fork, wait), ISO C (system)
 * Compilar: gcc -Wall -Wextra -o p6-2 p6-2.c
 * Ejecutar: ./p6-2        (requiere pstree: paquete psmisc)
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
    pid_t raiz = getpid(), recogido;

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            if (i == 0 || i == 2)
                fork();
            break;
        }
    }

    if (getpid() == raiz) {
        sleep(1);
        mostrar_arbol();
        for (int i = 0; i < N; i++) {
            recogido = wait(NULL);
            printf("%d recogió a %d\n", (int)getpid(), (int)recogido);
        }
    } else {
        sleep(2);
        recogido = wait(NULL);
        if (recogido != -1)
            printf("%d recogió a %d\n", (int)getpid(), (int)recogido);
    }

    return EXIT_SUCCESS;
}
