#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "structs.h"

/* auxiliares */
int largoTexto(char *texto);
char *copiarTexto(char *texto);

/* abbTitulos */
struct NodoTitulo *buscarNodoTitulo(struct NodoTitulo *raiz, int IDtitulo);
struct NodoTitulo *insertarNodoTitulo(struct NodoTitulo *raiz, struct NodoTitulo *nuevo, int *insertado);
struct NodoTitulo *quitarMinimo(struct NodoTitulo *raiz);
struct NodoTitulo *eliminarNodoTitulo(struct NodoTitulo *raiz, int IDtitulo, int *eliminado);
void listarArbol(struct NodoTitulo *raiz);

/* salas */
int inicializarSalas(struct Tienda *tienda, int capacidad);
struct Sala *buscarSala(struct Tienda *tienda, int IDsala);
int crearSala(struct Tienda *tienda, int IDsala, char *nombre, char *tematica, char *horario, int aforoMaximo);
int modificarSala(struct Tienda *tienda, int IDsala, char *nombre, char *tematica, char *horario, int aforoMaximo);
int eliminarSala(struct Tienda *tienda, int IDsala);
void listarSalas(struct Tienda *tienda);

/* titulosAsociados */
struct Sala *ubicarTitulo(struct Tienda *tienda, int IDtitulo);
int agregarTitulo(struct Tienda *tienda, int IDsala, int IDtitulo, char *nombre, char *genero, char *tipo, int clasificacionEdad, int precio, int ejemplares, int cantDespacho, int demoraDespacho);
int modificarTitulo(struct Tienda *tienda, int IDtitulo, char *nombre, int precio, int ejemplares);
int eliminarTitulo(struct Tienda *tienda, int IDtitulo);
void listarTitulosSala(struct Tienda *tienda, int IDsala);

#endif
