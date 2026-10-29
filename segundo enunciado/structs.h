#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Pase {
    int IDpase;
    char *tipo;
    int valor;
    char *estado;
};

struct Cliente {
    int IDcliente;
    char *nombre;
    int edad;
    int dentroTienda; /* se trata como booleano 1,0 */
    int salaActual;   /* 0 si no esta en ninguna sala */
    struct Pase pase;
};

struct NodoCliente {
    struct Cliente *cliente;
    struct NodoCliente *sig;
    struct NodoCliente *ant;
};

struct ListaEspera {
    struct NodoCliente *primero;
    struct NodoCliente *ultimo;
    int cantidad;
};

struct Titulo {
    int IDtitulo;
    char *nombre;
    char *genero;
    char *tipo;
    int clasificacionEdad;
    int precio;
    int ejemplaresDisponibles;
    int cantDespacho;
    int demoraDespacho;
    int solicitudesDia;
    char *estado;
    struct ListaEspera espera;
};

struct NodoTitulo {
    struct Titulo titulo;
    struct NodoTitulo *izquierdo;
    struct NodoTitulo *derecho;
};

struct ArbolTitulos {
    struct NodoTitulo *raiz;
    int cantidad;
};

struct Sala {
    int IDsala;
    char *nombre;
    char *tematica;
    char *horario;
    int aforoMaximo;
    int clientesDentro;
    struct ListaEspera espera;
    struct ArbolTitulos titulos;
};

/* Evento, NodoEvento y Bitacora van ANTES de Tienda */
struct Evento {
    int IDevento;
    char *tipo;
    int IDcliente;
    int IDsala;
    int IDtitulo;
};

struct NodoEvento {
    struct Evento evento;
    struct NodoEvento *sig;
};

struct Bitacora {
    struct NodoEvento *primero;
    struct NodoEvento *ultimo;
    int cantidad;
};

struct Tienda {
    int cantSalas;
    int capacidadSalas;
    struct Sala **salas;
    int cantidadClientes;
    struct NodoCliente **clientes;
    int aforoTienda;
    int clientesDentro;
    int clientesIngresadosHoy;
    int pasesVendidos;
    int pasesUtilizadosDia;
    int gananciaPases;
    int gananciaHoy;
    int siguienteIdCliente;
    int siguienteIdPase;
    int siguienteIdEvento;
    struct Bitacora bitacora;
};

#endif
