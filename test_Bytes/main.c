#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ){

//  ###############################################################################
//    int integerType;
//    float floatType;
//    double doubleType;
//    char charType;

    //  sizeof ���������ڼ���������ֽڴ�С
//    printf("Size of int:    %ld bytes\n", sizeof(integerType));
//    printf("Size of float:  %ld bytes\n", sizeof(floatType));
//    printf("Size of double: %ld bytes\n", sizeof(doubleType));
//    printf("Size of char:   %ld bytes\n", sizeof(charType));
//  ###############################################################################

//  ###############################################################################
//  ��������ƣ������char*itoa(int value,char*string,int radix)������ת�����ַ���
    int a = 0x7fffffe;
    char s16[128]="";
    char s8[128]="";
    char s2[128]="";

    itoa(a,s16,16);
    itoa(a,s8,8);
    itoa(a,s2,2);

    printf("== ֱ�������ֵ��� ==\n");
    printf("10������ֵ�� %d\n", a);
    printf("16������ֵ�� %x\n", a);

    printf("== ���ת������ַ� ==\n");
    printf("16����:   %s\n",s16);
    printf("8����:    %s\n",s8);
    printf("2����:    %s\n",s2);
//  ###############################################################################

    return 0;
}
