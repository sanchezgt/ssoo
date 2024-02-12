/* 
Este programa en C demuestra el manejo de señales para contar cuántas veces
se recibe la señal SIGINT (usualmente generada con Ctrl+C en la terminal).
El programa registra un manejador de señales personalizado que incrementa un contador
cada vez que se recibe la señal SIGINT. Luego entra en un bucle infinito, imprimiendo
el número de veces que se ha recibido la señal y pausando por un segundo en cada iteración.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o signal_counter signal_counter.c
4. Ejecuta el programa compilado:
   ./signal_counter
5. Para enviar la señal SIGINT al proceso, presiona Ctrl+C en la misma terminal.
6. Para detener el programa, puedes usar Ctrl+Z y luego matar el proceso con 'kill <PID>'.

Nota: El programa no restaura el manejador de señales predeterminado para SIGUSR1 como
se intenta al final del main, ya que nunca se alcanza esa línea debido al bucle infinito.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf
#include <stdlib.h>   // Para EXIT_SUCCESS y EXIT_FAILURE
#include <unistd.h>   // Para sleep
#include <signal.h>   // Para signal y SIGINT

// Declaración de variables globales
int count = 0; // Contador de señales SIGINT recibidas
void (*oldhandler)(int); // Puntero a la función del manejador de señales anterior

// Declaración del manejador de señales
void sighandler(int sig);

int main() {
    // Registra el manejador de señales personalizado para SIGINT
    oldhandler = signal(SIGINT, sighandler);
    if (oldhandler == SIG_ERR) {
        perror("signal:"); // Imprime un mensaje de error si falla
        exit(EXIT_FAILURE);
    }

    // Imprime el PID del proceso para poder enviar señales desde otra terminal
    printf("[%d]\n", getpid());

    // Bucle infinito que imprime el contador y pausa por un segundo
    do {
        printf("%d veces SIGINT recibida\n", count);
        sleep(1);
    } while (1);

    // Código inalcanzable: intenta restaurar el manejador de señales predeterminado para SIGUSR1
    if (signal(SIGUSR1, oldhandler) == SIG_ERR) {
        perror("signal:"); // Imprime un mensaje de error si falla
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente
}

// Implementación del manejador de señales personalizado
void sighandler(int sig) {
    // Incrementa el contador cuando se recibe la señal SIGINT
    count++;
}