/* 
Este programa en C demuestra la comunicación entre procesos usando pipes. 
El proceso padre escribe un mensaje en el pipe y el proceso hijo lo lee.
El padre espera 5 segundos antes de escribir para simular un trabajo que tarda en completarse.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o pipe_communication pipe_communication.c
4. Ejecuta el programa compilado:
   ./pipe_communication

Las librerías incluidas proporcionan las funciones necesarias para la entrada/salida estándar,
la creación y uso de pipes, la creación de procesos y la espera de su terminación.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf.
#include <stdlib.h>   // Para la macro EXIT_SUCCESS.
#include <unistd.h>   // Para las funciones pipe, fork, getpid, sleep, write y read.
#include <string.h>   // Para funciones de manejo de strings como strlen.
#include <sys/wait.h> // Para la función wait.

int main() {
    int fd[2], n; // Descriptores de archivo para el pipe.
    char buff[50]; // Buffer para almacenar el mensaje leído del pipe.
    
    // Crea un pipe.
    pipe(fd);
    
    // Crea un proceso hijo.
    if (fork()) { // Proceso padre
        char msg[] = "Mensaje de texto"; // Mensaje a enviar al hijo.
        
        // Cierra el extremo de lectura del pipe en el padre.
        close(fd[0]);
        // El padre espera 5 segundos antes de escribir en el pipe.
        sleep(5);
        // Escribe el mensaje en el extremo de escritura del pipe.
        write(fd[1], msg, strlen(msg));
        // Imprime el mensaje que se ha escrito en el pipe.
        printf("[%d]escrito:--> %s\n", getpid(), msg);
        // Cierra el extremo de escritura del pipe en el padre.
        close(fd[1]);
        // Espera a que el hijo termine.
        wait(NULL);
    } else { // Proceso hijo
        // Cierra el extremo de escritura del pipe en el hijo.
        close(fd[1]);
        // Imprime un separador para indicar que el hijo está listo para leer.
        printf("---->\n");
        // Lee el mensaje del extremo de lectura del pipe.
        n = read(fd[0], buff, sizeof(buff) - 1);
        // Asegura que el mensaje leído sea una cadena de texto válida.
        buff[n] = '\0';
        // Imprime el mensaje que se ha leído del pipe.
        printf("[%d] leido:<-- %s\n", getpid(), buff);
        // Cierra el extremo de lectura del pipe en el hijo.
        close(fd[0]);
    }
    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente.
}