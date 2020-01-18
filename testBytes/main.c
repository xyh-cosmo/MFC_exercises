#include <stdio.h>

int main( int argc, char *argv[] ){

    int integerType;
    float floatType;
    double doubleType;
    char charType;

    //  sizeof 操作符用于计算变量的字节大小
    printf("Size of int:    %ud bytes\n", sizeof(integerType));
    printf("Size of float:  %ud bytes\n", sizeof(floatType));
    printf("Size of double: %ud bytes\n", sizeof(doubleType));
    printf("Size of char:   %ud bytes\n", sizeof(charType));

    return 0;
}
