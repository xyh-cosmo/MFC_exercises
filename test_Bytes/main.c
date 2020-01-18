#include <stdio.h>
#include <stdlib.h>

void my_itoa( int num, char str[], unsigned int radix){

    switch (radix) {
    case 16:
        sprintf(str,"%x",num);
        break;
    case 8:
        sprintf(str,"%o",num);
        break;
    case 2:
        sprintf(str,"%b",num);
        break;
    default:
        printf(" Wrong radix: %d", radix);
        break;
    }
}

int main( int argc, char *argv[] ){

//  ###############################################################################
//    int integerType;
//    float floatType;
//    double doubleType;
//    char charType;

    //  sizeof 操作符用于计算变量的字节大小
//    printf("Size of int:    %ld bytes\n", sizeof(integerType));
//    printf("Size of float:  %ld bytes\n", sizeof(floatType));
//    printf("Size of double: %ld bytes\n", sizeof(doubleType));
//    printf("Size of char:   %ld bytes\n", sizeof(charType));
//  ###############################################################################

//  ###############################################################################
//  输出二进制，需调用char*itoa(int value,char*string,int radix)将数字转换成字符串
//  拓展：不同进制的数如何显式地表示，例如16进制的前缀为“0X”
    int a = 0x7fffffe;
    char s16[128]="";
    char s8[128]="";
    char s2[128]="";

    my_itoa(a,s16,16);
    my_itoa(a,s8,8);
    my_itoa(a,s2,2);

    printf("== 直接输出数值结果 ==\n");
    printf("10进制数值： %d\n", a);
    printf("16进制数值： %x\n", a);

    printf("== 输出转换后的字符 ==\n");
    printf("16进制:   %s\n",s16);
    printf("8进制:    %s\n",s8);
    printf("2进制:    %s\n",s2);
//  ###############################################################################

    return 0;
}
