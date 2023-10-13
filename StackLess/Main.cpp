#include<stdio.h>
#include"StackLess.h"
int main()
{
    /*printf("sizeof(int) = %ld\n", sizeof(int));
    printf("sizeof(char*) = %ld\n", sizeof(char*));
    printf("%ld bit System \n", sizeof(char*) * 8);
    runcode("print('hello')  ");
    runcode("print(print('hello'))");
    runcode("print('first',print('second'))");
    runcode("print(print('first'), 'second')");
    runcode("print(print('first','2'),'second') ");
    runcode("print(add(4,3)) ");*/
    /*runcode("set('4',3)print(get('4'))");
    runcode("set('4',3)print(sub(get('4'),1))");
    runcode("print(set('4',3))print(sub(get('4'),1))");
    runcode("if(1)print('this should be seen');if(0)print('this not');");*/
    runcode(
        "print(set('i',10))"
        "print(get('i'))"
        "print(sub(get('i'),1))"
        "print(set('i',sub(get('i'),1)))");
    runcode("set('i',10)while(get('i'))print(set('i',sub(get('i'),1)));");



    //print("Hello World!");
    return 0;
}
