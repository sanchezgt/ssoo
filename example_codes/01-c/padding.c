/* 
Este programa en C muestra el tamaño en bytes de varios tipos de datos primitivos y estructuras.
Además, ilustra cómo el orden de los miembros en una estructura puede afectar su tamaño total debido al padding.
El padding es un espacio adicional que el compilador agrega para alinear los datos en memoria.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o padding padding.c
4. Ejecuta el programa compilado:
   ./padding

Las librerías incluidas proporcionan las funciones necesarias para la entrada/salida estándar y
para la gestión de la finalización del programa.
*/

#include <stdio.h>  // Para funciones de entrada/salida como printf.
#include <stdlib.h> // Para la macro EXIT_SUCCESS.

// Definición de estructuras para demostrar el efecto del padding.
struct data1 {
    char a;    // 1 byte
    char b;    // 1 byte
    char c;    // 1 byte
    int d;     // 4 bytes (típicamente)
    double e;  // 8 bytes (típicamente)
};

struct data2 {
    char a;    // 1 byte
    char b;    // 1 byte
    int d;     // 4 bytes (típicamente)
    char c;    // 1 byte
    double e;  // 8 bytes (típicamente)
};

struct data3 {
    char a;    // 1 byte
    int d;     // 4 bytes (típicamente)
    char b;    // 1 byte
    double e;  // 8 bytes (típicamente)
    char c;    // 1 byte
};

// Función que imprime el tamaño de varios tipos de datos y estructuras.
void sizes_by_type() {
    // Imprime el tamaño de varios tipos de datos primitivos.
    printf("Tamaño de char: %zu byte(s)\n", sizeof(char));
    printf("Tamaño de int: %zu byte(s)\n", sizeof(int));
    printf("Tamaño de short: %zu byte(s)\n", sizeof(short));
    printf("Tamaño de long: %zu byte(s)\n", sizeof(long));
    printf("Tamaño de long long: %zu byte(s)\n", sizeof(long long));
    printf("Tamaño de float: %zu byte(s)\n", sizeof(float));
    printf("Tamaño de double: %zu byte(s)\n", sizeof(double));
    printf("Tamaño de long double: %zu byte(s)\n", sizeof(long double));
    
    // Imprime el tamaño de las estructuras definidas anteriormente.
    printf("Tamaño de struct data1: %zu byte(s)\n", sizeof(struct data1));
    printf("Tamaño de struct data2: %zu byte(s)\n", sizeof(struct data2));
    printf("Tamaño de struct data3: %zu byte(s)\n", sizeof(struct data3));
}

// Función principal que llama a sizes_by_type para mostrar los tamaños.
int main() {    
    sizes_by_type();
    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente.
}