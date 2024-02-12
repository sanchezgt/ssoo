/* 
Este programa en C demuestra el uso de pipes para la comunicación entre procesos.
El programa crea un pipe, escribe un mensaje en el extremo de escritura y luego 
lee el mismo mensaje desde el extremo de lectura.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o pipe_demo pipe_demo.c
4. Ejecuta el programa compilado:
   ./pipe_demo

Las librerías incluidas proporcionan las funciones necesarias para la entrada/salida estándar,
manejo de strings y la creación y uso de pipes.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf.
#include <stdlib.h>   // Para la macro EXIT_SUCCESS.
#include <unistd.h>   // Para la función pipe y getpid.
#include <string.h>   // Para funciones de manejo de strings como strcpy y strlen.

int main() {
    int fd[2], n; // fd[0] será el extremo de lectura del pipe, fd[1] el de escritura.
    char Buffer_out[1024]; // Buffer para almacenar los datos a escribir en el pipe.
    char Buffer_in[1024];  // Buffer para leer los datos del pipe.

    char data[] = "Hola mundo pipes"; // Mensaje a enviar a través del pipe.
    
    // Crea un pipe y almacena los descriptores de archivo en fd.
    pipe(fd);
    
    // Copia el mensaje en el buffer de salida.
    strcpy(Buffer_out, data);
    // Imprime el mensaje que se va a escribir en el pipe.
    printf("[%d]write:--> %s\n", getpid(), Buffer_out);
    // Escribe el mensaje en el extremo de escritura del pipe.
    write(fd[1], Buffer_out, strlen(Buffer_out)); // EOL = '\0'
    
    // Lee el mensaje desde el extremo de lectura del pipe.
    n = read(fd[0], Buffer_in, sizeof(Buffer_in) - 1);
    // Asegura que el mensaje leído sea una cadena de texto válida.
    Buffer_in[n] = '\0';
    // Imprime el mensaje que se leyó del pipe.
    printf("[%d]read:<-- %s\n", getpid(), Buffer_in);

    return EXIT_SUCCESS; // Indica que el programa terminó exitosamente.
}