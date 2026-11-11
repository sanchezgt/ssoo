/*
 * asserts.c — Aserciones con assert()
 *
 * assert(expr) detiene el programa con un mensaje (archivo, línea y
 * expresión) si expr es falsa. Sirve para verificar supuestos durante el
 * desarrollo; al compilar con -DNDEBUG las aserciones desaparecen.
 *
 * Observe: la segunda aserción falla y el proceso termina con SIGABRT.
 *          Compile con -DNDEBUG y el programa llega al final.
 *
 * Norma:    ISO C
 * Compilar: gcc -Wall -Wextra -o asserts asserts.c
 *           gcc -Wall -Wextra -DNDEBUG -o asserts asserts.c
 * Ejecutar: ./asserts
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int a = 5;

    assert(a == 5);
    printf("Primera aserción superada\n");

    assert(a == 0);
    printf("Segunda aserción superada (solo con -DNDEBUG)\n");

    return EXIT_SUCCESS;
}
