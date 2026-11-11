/*
 * pragmas.c — Compilación condicional con el preprocesador
 *
 * El preprocesador decide qué código llega al compilador. Con #ifdef se
 * compila una u otra rama según exista o no una macro; la macro se puede
 * definir desde la línea de órdenes con -D, sin tocar el código.
 *
 * Observe: compile las dos versiones y compare la salida. Con  gcc -E
 *          verá el código después del preprocesador.
 *
 * Norma:    ISO C
 * Compilar: gcc -Wall -Wextra -o pragmas pragmas.c
 *           gcc -Wall -Wextra -DUBUNTU -o pragmas pragmas.c
 * Ejecutar: ./pragmas
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
#ifdef UBUNTU
    printf("Compilado para Ubuntu\n");
#else
    printf("Compilado para otro sistema operativo\n");
#endif

#ifdef __linux__
    printf("El compilador definió __linux__ por su cuenta\n");
#endif

    return EXIT_SUCCESS;
}
