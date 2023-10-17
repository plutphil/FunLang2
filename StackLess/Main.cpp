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
    runcode("print(set('4',3))print(sub(get('4'),1))");*/
    runcode("if(1)print('this should be seen');if(0)print('this not');print('this however should')");
    runcode(
        "print(set('i',10))"
        "print(get('i'))"
        "print(sub(get('i'),1))"
        "print(set('i',sub(get('i'),1)))");
    runcode("set('i',10)while(get('i'))print(set('i',sub(get('i'),1)));print('done');");
    runcode(
        "set('num',10)"
        "set('x',0)"
        "set('y',1)"
        "set('z',0)"
        "set('i',0)"
        "while(gt(get('i'),get('num')))"
        "print(get('x'))"
        "set('z',add(get('x'),get('y')))"
        "set('x',get('y'))"
        "set('y',get('z'))"
        "set('i',add(get('i'),1))"
        ";"

    );
    runcode("set(kw,10)print(get(kw))");


    //print("Hello World!");
    return 0;
}
