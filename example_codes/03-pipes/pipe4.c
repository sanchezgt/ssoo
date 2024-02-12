/* 
Este programa en C crea un proceso hijo que lee desde un pipe mientras el proceso padre escribe en él.
El usuario puede escribir mensajes en la entrada estándar, que el proceso padre envía al hijo a través del pipe.
El proceso hijo lee y muestra estos mensajes. El programa termina cuando el usuario escribe "salir".

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o pipe_messaging pipe_messaging.c
4. Ejecuta el programa compilado:
   ./pipe_messaging

Las librerías incluidas proporcionan las funciones necesarias para la entrada/salida estándar,
la creación y uso de pipes, la creación de procesos, la espera de su terminación y el manejo de cadenas de texto.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf y fgets.
#include <stdlib.h>   // Para la macro EXIT_SUCCESS.
#include <unistd.h>   // Para las funciones pipe, fork, getpid y read/write.
#include <string.h>   // Para funciones de manejo de cadenas como strlen y strcmp.
#include <sys/wait.h> // Para la función wait.

#define MAX_READ 256 // Tamaño máximo del buffer de lectura.

int main() {
    int fd[2]; // Descriptores de archivo para el pipe.
    char buff[MAX_READ]; // Buffer para almacenar los mensajes.
    int n; // Variable para almacenar el número de bytes leídos.

    // Crea un pipe.
    pipe(fd);

    // Instrucción al usuario.
    printf("write 'salir' to exit\n\n");

    if (fork() != 0) { // Proceso padre
        // Cierra el extremo de lectura del pipe en el padre.
        close(fd[0]);

        do {
            // Lee una línea de la entrada estándar.
            fgets(buff, MAX_READ, stdin);
            // Reemplaza el salto de línea por un carácter nulo para terminar la cadena.
            if (strlen(buff) > 1) {
                buff[strlen(buff) - 1] = EOL;
                // Imprime el mensaje que se va a escribir en el pipe.
                printf("[%d]write-->:%s\n", getpid(), buff);
                // Escribe el mensaje en el pipe.
                write(fd[1], buff, strlen(buff));
            }
        } while (strcmp(buff, "salir") != 0); // Continúa hasta que el usuario escriba "salir".

        // Cierra el extremo de escritura del pipe en el padre.
        close(fd[1]);
        // Espera a que el hijo termine.
        wait(NULL);
    } else { // Proceso hijo
        // Cierra el extremo de escritura del pipe en el hijo.
        close(fd[1]);

        // Lee mensajes del pipe hasta que no haya más datos.
        while ((n = read(fd[0], buff, MAX_READ)) > 0) {
            // Asegura que el mensaje leído sea una cadena de texto válida.
            buff[n] = EOL;
            // Imprime el mensaje que se ha leído del pipe.
            printf("[%d]read<--:%s\n", getpid(), buff);
        }
        // Cierra el extremo de lectura del pipe en el hijo.
        close(fd[0]);
    }
    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente.
}
