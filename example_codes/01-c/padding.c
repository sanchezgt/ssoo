/*
 * padding.c — Tamaño de los tipos y relleno (padding) en estructuras
 *
 * El compilador alinea cada miembro de una estructura a una dirección
 * múltiplo de su tamaño. Para lograrlo inserta bytes de relleno, así que
 * el orden de los miembros cambia el tamaño total de la estructura.
 *
 * Observe: las tres estructuras tienen los mismos miembros, pero no el
 *          mismo tamaño. offsetof() muestra dónde quedó cada miembro.
 *          Los tamaños dependen de la arquitectura (x86-64 en el aula).
 *
 * Norma:    ISO C
 * Compilar: gcc -Wall -Wextra -o padding padding.c
 * Ejecutar: ./padding
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct data1 { char a; char b; char c; int d; double e; };
struct data2 { char a; char b; int d; char c; double e; };
struct data3 { char a; int d; char b; double e; char c; };

#define MOSTRAR(S) \
    printf("%-13s %2zu bytes   a@%zu b@%zu c@%zu d@%zu e@%zu\n", #S, sizeof(S), \
           offsetof(S, a), offsetof(S, b), offsetof(S, c), offsetof(S, d), offsetof(S, e))

int main(void)
{
    printf("char        %zu\n", sizeof(char));
    printf("short       %zu\n", sizeof(short));
    printf("int         %zu\n", sizeof(int));
    printf("long        %zu\n", sizeof(long));
    printf("long long   %zu\n", sizeof(long long));
    printf("float       %zu\n", sizeof(float));
    printf("double      %zu\n", sizeof(double));
    printf("long double %zu\n", sizeof(long double));
    printf("void *      %zu\n\n", sizeof(void *));

    /* Los datos útiles suman 3 + 4 + 8 = 15 bytes en las tres. */
    MOSTRAR(struct data1);
    MOSTRAR(struct data2);
    MOSTRAR(struct data3);

    return EXIT_SUCCESS;
}
