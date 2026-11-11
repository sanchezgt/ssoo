/*
 * p1-2.c — Identificadores de proceso: PID y PPID
 *
 * Todo proceso tiene un identificador (PID) y conoce el de su padre
 * (PPID). Al lanzar el programa desde la terminal, el padre es la shell.
 *
 * Observe: ejecute  echo $$  y compare ese número con el PPID impreso.
 *
 * Norma:    POSIX (getpid, getppid)
 * Compilar: gcc -Wall -Wextra -o p1-2 p1-2.c
 * Ejecutar: ./p1-2
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    printf("pid=%d ppid=%d\n", (int)getpid(), (int)getppid());
    return EXIT_SUCCESS;
}
