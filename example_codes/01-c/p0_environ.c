/*
 * p0_environ.c — Variables de entorno de un proceso
 *
 * Cada proceso recibe del padre una copia de su entorno: una lista de
 * cadenas "NOMBRE=valor" terminada en NULL, accesible con la variable
 * global environ. getenv() busca un nombre concreto en esa lista.
 *
 * Observe: ejecute  MI_VAR=hola ./p0_environ | grep MI_VAR  y verá que
 *          la variable existe solo para ese proceso.
 *
 * Norma:    POSIX (environ), ISO C (getenv)
 * Compilar: gcc -Wall -Wextra -o p0_environ p0_environ.c
 * Ejecutar: ./p0_environ
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main(void)
{
    for (char **var = environ; *var != NULL; var++)
        printf("%s\n", *var);

    const char *path = getenv("PATH");
    printf("\nPATH = %s\n", path != NULL ? path : "(no definida)");

    return EXIT_SUCCESS;
}
