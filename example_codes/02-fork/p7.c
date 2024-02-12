/*
Este programa en C crea una estructura de procesos en forma de árbol. El proceso principal (padre)
crea un proceso hijo, que a su vez crea tres procesos hijos. El proceso hijo original espera a que
sus tres hijos terminen. El proceso padre, después de un breve retraso, muestra el árbol de procesos
utilizando una función 'showtree' no definida en el fragmento de código proporcionado.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o proceso proceso.c
4. Ejecuta el programa compilado:
   ./proceso

Nota: Este código no compilará sin la función 'showtree' definida. Asegúrate de definir esta función
o eliminar la llamada a 'showtree' antes de compilar.

Las librerías incluidas proporcionan las funciones necesarias para la creación y manejo de procesos,
así como para la entrada/salida estándar.
*/

#include <stdio.h>      // Para funciones de entrada/salida como printf.
#include <stdlib.h>     // Para funciones de utilidad general como exit.
#include <sys/types.h>  // Para tipos de datos como pid_t.
#include <sys/wait.h>   // Para la función wait.
#include <unistd.h>     // Para la función fork y sleep.

// Función 'showtree' no definida. Debe ser proporcionada para que el código funcione.
// void showtree();

int main(void) {
    int i;
    pid_t root = getpid(); // Almacena el PID del proceso padre original.

    if (!fork()) { // El proceso padre crea un proceso hijo.
        for (i = 0; i < 3; i++) {
            if (!fork()) { // El proceso hijo crea tres procesos hijos.
                break; // Los procesos hijos no continúan el bucle.
            }
        }

        if (i == 3) { // Si es el proceso hijo original...
            for (int j = 0; j < 3; j++) {
                wait(NULL); // Espera a que terminen sus tres hijos.
            }
        }
    } else {
        sleep(1); // El proceso padre espera un segundo.
        // showtree(); // Muestra el árbol de procesos. Esta función debe ser definida.
        wait(NULL); // Espera a que termine el proceso hijo original.
    }

    return EXIT_SUCCESS; // Termina el proceso con éxito.
}