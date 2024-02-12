/*
Este programa en C imprime la lista completa de variables de entorno disponibles para el proceso actual.
Además, recupera y muestra el valor de la variable de entorno "PATH". Este programa es útil para
diagnosticar el entorno de ejecución de un proceso y verificar las configuraciones del sistema.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o p0_environ p0_environ.c
4. Ejecuta el programa compilado:
   ./p0_environ

Las librerías incluidas proporcionan las funciones necesarias para la entrada/salida estándar y
para la gestión de variables de entorno.
*/

#include <stdio.h>   // Para funciones de entrada/salida como printf.
#include <stdlib.h>  // Para la función getenv.
#include <unistd.h>  // Para la variable environ.

extern char **environ; // Declaración externa para la lista de variables de entorno.

int main(void) {
    // Puntero para recorrer la lista de variables de entorno.
    char **list = environ;
    char *var;

    // Recorre la lista de variables de entorno y muestra su contenido.
    while (*list != NULL) {
        printf("%s\n", *list); // Imprime la variable de entorno actual.
        list++; // Avanza al siguiente elemento de la lista.
    }

    // Obtiene el valor de la variable de entorno "PATH".
    var = getenv("PATH");
    // Imprime el valor de la variable de entorno "PATH".
    printf("PATH = %s\n", var);

    // Retorna EXIT_SUCCESS para indicar que el programa finalizó correctamente.
    return EXIT_SUCCESS;
}