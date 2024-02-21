#include <stdio.h>
#include <stdlib.h>

union Dato{
    int a;
    double b;
};

int main(){
union Dato dato;
dato.a = 5;
printf("a=%d\n", dato.a);
printf("b=%d\n", dato.b);
dato.b=12.2;
printf("a=%d\n", dato.a);
printf("b=%d\n", dato.b);

return EXIT_SUCCESS;
}