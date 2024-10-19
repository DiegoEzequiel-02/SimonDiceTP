#include "cola.h"

void crearCola(t_cola* c){
    c->prim = NULL;
    c->ult = NULL;
}

int agregarACola(t_cola* c, void* dato, unsigned tam){
    t_nodo* nue = (t_nodo*)malloc(sizeof(t_nodo));
    if(!nue){
        return 0;
    }

    nue->info = malloc(tam);
    if(!nue->info){
        free(nue);
        return 0;
    }

    memcpy(nue->info,dato,tam);
    nue->tam = tam;
    nue->sig_nodo = NULL;

    if (c->prim == NULL) {
        c->prim = nue;
    } else {
        // Conectar el nuevo nodo al final de la cola
        c->ult->sig_nodo = nue;
    }

    c->ult = nue;
    return 1;
}

int colaVacia (t_cola* c){
    return c->prim == NULL;
}

int colaLlena(const t_cola* c, unsigned tam){
    t_nodo* aux = (t_nodo*)malloc(sizeof(t_nodo));
    void* aux2 = malloc(tam);
    free(aux);
    free(aux2);
    return aux == NULL || aux2 == NULL;
}

int sacarElemento(t_cola* c, void* d, unsigned tam){
    t_nodo* aux;
    if(c->prim == NULL) // Recordemos ColaVacia
        return 0;
    // Tenemos que sacar el PRIMER elemento
    aux = c->prim;
    // Nuestro nuevo primero es el siguiente de nuestro primero
    c->prim = c->prim->sig_nodo;
    if(c->prim == NULL) // Si luego de esto no queda cola...
        c->ult = NULL;

    // Ya terminamos todo el cableado complejo, ahora nos falta
    // pasar la info y matar el nodo
    memcpy(d, aux->info, MIN(tam, aux->tam));
    free(aux->info);
    free(aux);

    return 1;
}

void vaciarCola(t_cola* c) {
    t_nodo* aux;

    while(c->prim != NULL) {
        aux = c->prim;
        c->prim = c->prim->sig_nodo;
        free(aux->info);
        free(aux);
    }
    c->ult = NULL;
}

int sacarDeCola(t_cola* cola, void* dato, unsigned tam) {
    if (!cola->prim) {  // Si la cola está vacía
        return 0;  // No hay elementos para sacar
    }

    t_nodo* actual = cola->prim;
    t_nodo* anterior = NULL;

    // Si la cola solo tiene un nodo (el primero es también el último)
    if (actual->sig_nodo == NULL) {
        memcpy(dato, actual->info, tam);
        free(actual);  // Liberar el único nodo
        cola->prim = NULL;
        cola->ult = NULL;  // La cola queda vacía
        return 1;
    }

    // Recorrer hasta el penúltimo nodo
    while (actual->sig_nodo != NULL) {
        anterior = actual;
        actual = actual->sig_nodo;
    }

    // Ahora 'actual' es el último nodo y 'anterior' es el penúltimo
    memcpy(dato, actual->info, tam);  // Copiar la info del último nodo
    free(actual);  // Liberar el último nodo

    anterior->sig_nodo = NULL;  // El penúltimo nodo es ahora el último
    cola->ult = anterior;  // Actualizar el puntero 'ult' al nuevo último nodo

    return 1;
}
