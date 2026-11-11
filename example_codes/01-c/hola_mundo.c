/*
 * hola_mundo.c — El programa mínimo en C
 *
 * main() es el punto de entrada. Su valor de retorno llega al proceso padre
 * (normalmente la shell) como estado de salida.
 *
 * Observe: después de ejecutarlo, escriba  echo $?  para ver el estado
 *          de salida (0 = EXIT_SUCCESS).
 *
 * Norma:    ISO C
 * Compilar: gcc -Wall -Wextra -o hola_mundo hola_mundo.c
 * Ejecutar: ./hola_mundo
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("Hola mundo\n");
    return EXIT_SUCCESS;
}
