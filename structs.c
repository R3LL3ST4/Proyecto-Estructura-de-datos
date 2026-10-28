#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Pase {
    int IDpase;
    char* tipo;
    int valor;
    char* estado;
};

struct Cliente {
    int IDcliente;
    char* nombre;
    int edad;
    int dentroTienda; /* se trata como booleando 1,0 */
    int salaActual;  /* 0 si no esta en ninguna sala */
    struct Pase pase;
};

struct NodoCliente {
    struct Cliente* cliente;
    struct NodoCliente* sig;
    struct NodoCliente* ant;
};

struct ListaEspera {
    struct NodoCliente* primero;
    struct NodoCliente* ultimo;
    int cantidad;
};

struct Titulo {
    int IDtitulo;
    char* nombre;
    char* genero;         /*genero del libro*/
    char* tipo;           /*para saber si es parte de una saga, serie o libro individual*/
    int clasificacionEdad;
    int precio;
    int ejemplaresDisponibles;
    int cantDespacho;     /*cant de libros despachados*/
    int demoraDespacho;   /*cada cuanto llega un despacho*/
    int solicitudesDia;
    char* estado;         /*estado del libro; agotado, descontinuado, disponible y reservado*/
    struct ListaEspera espera;
};

struct NodoTitulo {
    struct Titulo titulo;
    struct NodoTitulo* izquierdo;
    struct NodoTitulo* derecho;
};

struct ArbolTitulos {
    struct NodoTitulo* raiz;
    int cantidad;
};

struct Sala {
    int IDsala;
    char* nombre;
    char* tematica;
    char* horario;        /*dias que la sala esta disponible para visitar*/
    int aforoMaximo;
    int clientesDentro;
    struct ListaEspera espera;
    struct ArbolTitulos titulos;
};

struct Tienda {
    int cantSalas;
    int capacidadSalas;
    struct Sala** salas;
    int cantidadClientes;
    struct NodoCliente** clientes;
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
    struct Bitacora bitacora; /*registro de eventos*/
};

struct Evento { /*en cada evento se aplican los ID correspondientes y el resto en = 0*/
    int IDevento;
    char* tipo;   /*ingreso, salida, pase anulado, título agotado, título descatalogado*/
    int IDcliente;
    int IDsala;
    int IDtitulo;
};

struct NodoEvento {
    struct Evento evento;
    struct NodoEvento* sig;
};

struct Bitacora {
    struct NodoEvento* primero;
    struct NodoEvento* ultimo;
    int cantidad;
};
