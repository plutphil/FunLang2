#include<stdio.h>
#include"StackLess.h"
int main()
{
    printf("sizeof(int) = %ld\n", sizeof(int));
    printf("sizeof(char*) = %ld\n", sizeof(char*));
    printf("%ld bit System \n", sizeof(char*) * 8);
    runcode("print('hello')  ");
    runcode("print(print('hello'))");
    runcode("print('first',print('second'))");
    runcode("print(print('first'), 'second')");
    runcode("print(print('first','2'),'second') ");
    runcode("print(add(4,3)) ");
    runcode("set('4',3)print(get('4')) ");
    runcode("set('i',10)while(get('i'))set('i',sub(get('i')-1));");



    //print("Hello World!");
    return 0;
}
