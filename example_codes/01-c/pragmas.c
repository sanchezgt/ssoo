#include <stdio.h>
#include <stdlib.h>

int main(){
    #ifdef UBUNTU
    printf("Compilación para ubuntu\n");
    #else
    printf("Compilacion para otro OS");
    #endif

    return EXIT_SUCCESS;
}