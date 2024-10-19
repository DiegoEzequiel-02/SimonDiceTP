#ifndef FUNCION_H_INCLUDED
#define FUNCION_H_INCLUDED
#define MAX_NUMS 10
#define AMAYOR(x)(x>='A' && x<='Z') ? (x):(x-32)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <curl/curl.h>
#include <conio.h>
#include "cola.h"

void guardarConfiguracion (char dif);
void leerConfiguraciones(int* vidas,int* segs,int* secuen);
void vaciarVector(int vec[], int filas);
void** crearMatriz(int filas, int columnas, size_t tamElem);
void mostrarNombres(char** nombres, int cant);
int pedirNombres();
void guardarNombres(char** nombres);
void dificultad(char* dif);
void sortearJugadores(char** nombres, int cantJug);
void menu();
void iniciarJuego(char** nombres, int cantJug,int* puntos, int segsParaCompletar, int secuen, int vidas);
void mostrarDeAUno(t_cola* c, int cant);
int compararColas(t_cola* cola1, t_cola* cola2, int cant);
void* buscarMayor (void* vec, size_t ce, size_t tam,int cmp(const void* a, const void* b));
void guardarColaEnArchivo(t_cola* cola, FILE* informe, int cant);
void mostrarYGuardarGanadores(char** nombres, int puntos[],int cantJug, FILE* archivo);
#endif // FUNCION_H_INCLUDED
