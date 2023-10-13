#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <cstring>
#include <stdlib.h>
#include "StackLess.h"
int print(const char* str) {
    return printf("%s\n", str);
    //int ret= fputs(str,stdout);
    //fputs("\n",stdout);
    //return ret;
    //puts(newline);
}
enum TokenType {
    NONE,
    KWD,
    NUM,
    STR,
    FUN,
    WHILE
};
char* codeptr;
char c;
char lc = '0';
char strbuf[100];
char numbuf[20];
uint8_t numbufindex = 0;
uint16_t strbufindex = 0;
char* strptr = 0;
bool inkw = 0;
bool innum = 0;

uint8_t lasttokentype;
char* lasttokendata;


typedef struct {
    uint8_t type;
    char* data;
}Token;

Token tokenStack[100];
typedef struct {
    char* key;
    Token val;
}MapEntry;
MapEntry varMap[100];
int mapIndex = 0;
int tokenStackIndex = 0;



char* curr_fun;
uint8_t funindex;
void pushToken(uint8_t type, char* data) {
    tokenStack[tokenStackIndex++] = { type,data };
    lasttokentype = type;
    lasttokendata = data;
}
void setTopToken(uint8_t type, char* data) {
    tokenStack[tokenStackIndex-1] = { type,data };
}
int indefOfFirstTokenType(uint8_t type) {
    for (size_t i = tokenStackIndex-1; i >= 0; i--)
    {
        if (tokenStack[i].type == type) {
            return i;
        }
    }
    return -1;
}
void reset() {
    if (inkw) {
        strbuf[strbufindex++] = 0;
        //print(strptr);
        inkw = 0;
        int l = strlen(strptr);
        if(strptr>&strbuf[0])
        for (size_t i = 0; i < strbufindex; i++)
        {
            if (strcmp(&strbuf[i], strptr) == 0) {
                strptr = &strbuf[i];
                strbufindex -= l ;
                break;
            }
        }
        pushToken(KWD, strptr);
    }
    else if (innum) {
        numbuf[numbufindex] = 0;
        numbufindex = 0;
        innum = 0;
        int val = atoi(numbuf);
        pushToken(NUM, (char*)atoi(numbuf));
    }
}
int outputsum;
int tokenIndex;
int getVarMapIndex(char* key) {
    for (size_t i = 0; i < mapIndex; i++)
    {
        if (strcmp(key, varMap[i].key) == 0) {
            return i;
        }
    }
    return -1;
}
void functions() {
    outputsum = 0;
    tokenIndex = tokenStackIndex;
    tokenStackIndex = funindex;
    int argcount = tokenIndex - (funindex + 1);
    int argindex = funindex + 1;
    if (strcmp(curr_fun, "print") == 0) {
        printf("o: ");
        for (int i = funindex + 1; i < tokenIndex; i++) {
            if (tokenStack[i].type == STR)
            {
                outputsum += printf("%s ", tokenStack[i].data);
            }
            else if (tokenStack[i].type == NUM)
            {
                outputsum += printf("%d ", *((int*)&tokenStack[i].data));
            }
        }
        printf("\n");
        pushToken(NUM, (char*)(uint64_t)outputsum);
    }
    else if (strcmp(curr_fun, "add")==0) {
        int mode = 0;
        for (int i = funindex + 1; i < tokenIndex; i++) {
            if (tokenStack[i].type == STR)
            {
                if (mode == NUM) 
                {

                }
                else if (mode == 0) 
                {
                    mode = STR;

                }
                else if (mode == STR) 
                {

                }
            }
            else if (tokenStack[i].type == NUM)
            {
                
                if (mode == NUM) 
                {
                    outputsum += *((int*)&tokenStack[i].data);
                }
                else if (mode == 0) 
                {
                    outputsum += *((int*)&tokenStack[i].data);
                    mode = NUM;
                }
                else if (mode == STR) 
                {

                }
                
            }
        }
        pushToken(NUM, (char*)(uint64_t)outputsum);
    }
    else if (strcmp(curr_fun, "set") == 0) {
        if (argcount == 2) {
            if (tokenStack[argindex].type == STR) {
                int i = getVarMapIndex(tokenStack[argindex].data);
                if (i == -1) {
                    varMap[mapIndex++] = { strptr,tokenStack[argindex] };
                }
            }
        }
    }
}
void runcode(const char* code) {
    print(code);
    codeptr = (char*)code;
    c = *codeptr;
    strptr = 0;
    inkw = 0;
    tokenStackIndex = 0;
    strbufindex = 0;
    while (c != 0) {
        c = *codeptr++;
        if (c == 0)continue;
        if (c == '(') {
            reset();
            if (lasttokentype == KWD) {
                if (strcmp(lasttokendata, "while")==0) {
                    tokenStack[tokenStackIndex - 1].type = WHILE;
                    tokenStack[tokenStackIndex - 1].data = codeptr;
                }
                else {
                    printf("funcall %s\n", lasttokendata);
                    tokenStack[tokenStackIndex - 1].type = FUN;
                }
            }
        }
        else if (c == ',') {
            reset();
            //argCounter++;
        }
        else if (c == ')') {
            reset();

            //int whileindex = indefOfFirstTokenType(WHILE);
            //if (whileindex > funindex) {
            //    tokenStackIndex = whileindex;
                
            //}
            //else {
            funindex = indefOfFirstTokenType(FUN);
            if (funindex == ((uint8_t)-1)) {
                printf("funcall not found!");
            }
            curr_fun = tokenStack[funindex].data;
            functions();
            //}
            //printf("arg count %d \n",argCounter);
        }
        else if (c == '{') {

        }
        else if (c == '}') {

        }
        else if (c == '\'') {
            reset();
            c = *codeptr++;
            lc = 0;
            strptr = strbuf + strbufindex;
            while (c != '\'' && c != 0 && lc != '\\') {
                strbuf[strbufindex++] = c;
                c = *codeptr++;
            }
            strbuf[strbufindex++] = 0;
            //c = *codeptr++;
            printf("str: %s\n", strptr);

            pushToken(STR, strptr);
            //argsarray[argscounter]=strptr;
            //argscounter+=1;
        }
        else if (isdigit(c)) {
            if (!innum) {
                innum = 1;
            }
            numbuf[numbufindex++] = c;
        }
        else if (c == ' ' || c == '\t' || c == '\n') {

        }
        else {
            if (!inkw) {
                inkw = 1;
                strptr = strbuf + strbufindex;
            }
            strbuf[strbufindex++] = c;
        }
    }
}