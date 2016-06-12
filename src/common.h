#ifndef  COMMON_H
   #define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct REGISTER
{
    char ID[20];
    char name[20]; 
    char passwd[20];
    int socket_ID;
//    char IP[20];
//    int port;
    struct REGISTER* next;
    void * pada;
}REGISTER;
typedef struct TRANS
{
    char ID[20];
    char name[20];
    char data[200];
    void * pada;
}TRANS;
typedef struct LOGIN
{
    char ID[20];
    char name[20]; 
    char passwd[20];
    void * pada;
}LOGIN;
#endif
