/* 
Este programa imprime "Hola Mundo" en la consola.
Utiliza los argumentos de la función main() y retorna EXIT_SUCCESS al finalizar.

Para compilar un archivo de código fuente en C usando el compilador gcc, utiliza el siguiente comando:
gcc hola_mundo.c -o hola_mundo
Para ejecutar el programa compilado, utiliza el siguiente comando:
./hola_mundo

Las librerías incluidas proporcionan las funciones necesarias para la entrada/salida estándar y
para la gestión de la finalización del programa.
*/

#include <stdio.h>  // Incluye la función printf para la salida estándar.
#include <stdlib.h> // Incluye la macro EXIT_SUCCESS para la finalización exitosa del programa.

int main(int argc, char *argv[]) {
    // Imprime "Hola Mundo" en la consola.
    printf("Hola Mundo\n");
    
    // Retorna EXIT_SUCCESS para indicar que el programa finalizó correctamente.
    return EXIT_SUCCESS;
}