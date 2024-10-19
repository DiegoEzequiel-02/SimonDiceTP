#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED
#include "nodo.h"

typedef struct{
    t_nodo* prim;
    t_nodo* ult;
} t_cola;

void crearCola(t_cola* c);
int agregarACola(t_cola* c, void* dato, unsigned tam);
int colaVacia (t_cola* c);
int colaLlena(const t_cola* c, unsigned tam);
int sacarDeCola(t_cola* c, void* d, unsigned tam);
int sacarElemento(t_cola* c, void* d, unsigned tam);
void vaciarCola(t_cola* c);
#endif // COLA_H_INCLUDED
