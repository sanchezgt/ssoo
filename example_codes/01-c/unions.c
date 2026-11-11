/*
 * unions.c — Uniones: varios miembros en la misma memoria
 *
 * En una unión todos los miembros empiezan en la misma dirección. La
 * unión ocupa lo que ocupa su miembro más grande, y escribir un miembro
 * sobrescribe los bytes de los demás.
 *
 * Observe: después de escribir b, el valor de a cambia: se están leyendo
 *          como int los primeros 4 bytes del double.
 *
 * Norma:    ISO C
 * Compilar: gcc -Wall -Wextra -o unions unions.c
 * Ejecutar: ./unions
 */
#include <stdio.h>
#include <stdlib.h>

union dato {
    int a;
    double b;
};

int main(void)
{
    union dato d;

    printf("sizeof(int)=%zu  sizeof(double)=%zu  sizeof(union dato)=%zu\n",
           sizeof(int), sizeof(double), sizeof(union dato));
    printf("&d.a=%p  &d.b=%p\n\n", (void *)&d.a, (void *)&d.b);

    d.a = 5;
    printf("después de d.a = 5:     a=%d\n", d.a);

    d.b = 12.2;
    printf("después de d.b = 12.2:  b=%f  a=%d\n", d.b, d.a);

    return EXIT_SUCCESS;
}
