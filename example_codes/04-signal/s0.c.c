/* 
Este programa en C demuestra cómo manejar señales en sistemas operativos tipo Unix.
El programa registra un manejador de señales personalizado para SIGINT (interrupción
desde el teclado, usualmente Ctrl+C) y entra en un bucle infinito que imprime "Hello World!"
cada segundo. Cuando el usuario envía la señal SIGINT, el manejador de señales imprime un
mensaje indicando que la señal fue capturada.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o signal_handler signal_handler.c
4. Ejecuta el programa compilado:
   ./signal_handler
5. Para enviar la señal SIGINT, presiona Ctrl+C mientras el programa está en ejecución.

El programa imprimirá "Hello World!" cada segundo hasta que se reciba la señal SIGINT,
momento en el cual imprimirá el mensaje del manejador de señales y continuará la ejecución.
*/

#include <signal.h> // Para la función signal() y la macro SIGINT
#include <stdio.h>  // Para la función printf()
#include <unistd.h> // Para la función sleep()

// Declaración del manejador de señales
void manejador(int sig);

int main() {
    // Registra el manejador de señales para SIGINT
    signal(SIGINT, manejador);

    // Bucle infinito que imprime "Hello World!" cada segundo
    while (1) {
        printf("Hello World!\n");
        sleep(1); // Pausa la ejecución durante 1 segundo
    }
}

// Implementación del manejador de señales
void manejador(int sig) {
    // Imprime un mensaje indicando que la señal fue capturada
    printf("senal capturada [%d]\n", sig);
}