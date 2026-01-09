#include<stdio.h>

int main(){
    unsigned int a = 0x12345678;
    unsigned int b = 0x23456789;
    unsigned int c = 0x34567890;
    unsigned long long x = *((unsigned long long *)&c);
    unsigned long long y = *((unsigned long long *)(&b+1));
    
    // printf("%d\n", a);
    printf("&a = %p\n", &a);
    printf("&b = %p\n", &b);
    printf("&c = %p\n", &c);
    printf("%lld\n", x);
    printf("%lld\n", y);
    return 0;
}