/* 
Este programa en C demuestra la comunicación entre procesos padre e hijo utilizando pipes.
El proceso padre envía una estructura de datos que contiene un entero y un flotante al proceso hijo.
El hijo lee la estructura de datos del pipe y la imprime.

Instrucciones para compilar y ejecutar el programa:
1. Abre una terminal en tu sistema.
2. Navega al directorio donde guardaste este archivo.
3. Compila el programa con el siguiente comando:
   gcc -o struct_pipe_communication struct_pipe_communication.c
4. Ejecuta el programa compilado:
   ./struct_pipe_communication

Las librerías incluidas proporcionan las funciones necesarias para la entrada/salida estándar,
la creación y uso de pipes, la creación de procesos, la espera de su terminación y la gestión de memoria.
*/

#include <stdio.h>    // Para funciones de entrada/salida como printf.
#include <stdlib.h>   // Para la función malloc, free y la macro EXIT_SUCCESS.
#include <unistd.h>   // Para las funciones pipe, fork, getpid y read/write.
#include <string.h>   // Para funciones de manejo de strings.
#include <sys/wait.h> // Para la función wait.

#define MAX_BUFF 1024 // Tamaño máximo del buffer.

// Definición de la estructura de datos a enviar a través del pipe.
struct data {
   int a;
   float b; // sizeof(struct) > (sizeof(int) + sizeof(float))
};

int main() {
   int fd[2]; // Descriptores de archivo para el pipe.
   pipe(fd);  // Crea un pipe.

   if (fork() == 0) { // Proceso hijo
      struct data get; // Variable para almacenar los datos recibidos.
      close(fd[1]); // Cierra el extremo de escritura del pipe en el hijo.

      // Lee la estructura de datos del extremo de lectura del pipe.
      read(fd[0], &get, sizeof(struct data));
      
      // Imprime los datos leídos del pipe.
      printf("[%d]read:<--\t[%d|%.2f]\n", getpid(), get.a, get.b);
      close(fd[0]); // Cierra el extremo de lectura del pipe en el hijo.
   } else { // Proceso padre
      struct data *post = malloc(sizeof(struct data)); // Asigna memoria para la estructura.
      close(fd[0]); // Cierra el extremo de lectura del pipe en el padre.
      
      // Asigna valores a la estructura.
      post->a = 10;
      post->b = 2.3;
      
      // Imprime los datos que se van a escribir en el pipe.
      printf("[%d]write:-->\t[%d|%.2f]\n", getpid(), post->a, post->b);
      
      // Escribe la estructura de datos en el extremo de escritura del pipe.
      write(fd[1], post, sizeof(struct data));
      free(post); // Libera la memoria asignada para la estructura.
      
      // Espera a que el hijo termine.
      wait(NULL);
      close(fd[1]); // Cierra el extremo de escritura del pipe en el padre.
   }
   return EXIT_SUCCESS; // Indica que el programa terminó exitosamente.
}