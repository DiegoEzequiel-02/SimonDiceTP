#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED
#define MIN(x,y)(x<y)?(x):(y)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct s_nodo{
    unsigned tam;
    void* info;
    struct s_nodo* sig_nodo;
}t_nodo;


#endif // NODO_H_INCLUDED
