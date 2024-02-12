/* 
Este programa en C imprime los identificadores del proceso actual (PID) y del proceso padre (PPID).
Estos identificadores son útiles para el seguimiento y control de procesos en sistemas operativos tipo Unix.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o pid_demo pid_demo.c
4. Ejecuta el programa compilado:
   ./pid_demo

Las librerías incluidas proporcionan las funciones necesarias para obtener los identificadores de proceso
y para la salida estándar.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf.
#include <stdlib.h>   // Para la macro EXIT_SUCCESS.
#include <unistd.h>   // Para las funciones getpid y getppid.

int main(void) {
    // Imprime el PID del proceso actual y el PPID del proceso padre.
    printf("pid:%d ppid:%d\n", getpid(), getppid());
    
    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente.
}