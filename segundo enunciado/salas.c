/* salas */

#include "funciones.h"

/* crea el arreglo de salas con una capacidad fija: retorna 0 si no pudo */
int inicializarSalas(struct Tienda *tienda, int capacidad) {
    tienda->cantSalas = 0;
    tienda->capacidadSalas = 0;
    tienda->salas = NULL;

    if (capacidad <= 0) {
        return 0;
    }

    tienda->salas = (struct Sala **) malloc(capacidad * sizeof(struct Sala *));
    tienda->capacidadSalas = capacidad;

    return 1;
}

/* busca una sala por su codigo: retorna NULL si no existe */
struct Sala *buscarSala(struct Tienda *tienda, int IDsala) {
    int i;

    for (i = 0; i < tienda->cantSalas; i++) {
        if (tienda->salas[i]->IDsala == IDsala) {
            return tienda->salas[i];
        }
    }

    return NULL;
}

/* agrega una sala: retorna 1 si se agrego, 0 si el codigo ya existe o no hay espacio */
int crearSala(struct Tienda *tienda, int IDsala, char *nombre, char *tematica, char *horario, int aforoMaximo) {
    struct Sala *sala;

    if (aforoMaximo <= 0) {
        return 0;
    }

    if (tienda->cantSalas >= tienda->capacidadSalas) {
        return 0;
    }

    if (buscarSala(tienda, IDsala) != NULL) {
        return 0;
    }

    sala = (struct Sala *) malloc(sizeof(struct Sala));
    sala->IDsala = IDsala;
    sala->nombre = copiarTexto(nombre);
    sala->tematica = copiarTexto(tematica);
    sala->horario = copiarTexto(horario);
    sala->aforoMaximo = aforoMaximo;
    sala->clientesDentro = 0;
    sala->espera.primero = NULL;
    sala->espera.ultimo = NULL;
    sala->espera.cantidad = 0;
    sala->titulos.raiz = NULL;
    sala->titulos.cantidad = 0;

    tienda->salas[tienda->cantSalas] = sala;
    tienda->cantSalas++;

    return 1;
}

/* modifica los datos de una sala: un texto NULL se deja igual, y un aforo <= 0
   o menor a los clientes dentro tambien se deja igual */
int modificarSala(struct Tienda *tienda, int IDsala, char *nombre, char *tematica, char *horario, int aforoMaximo) {
    struct Sala *sala;

    sala = buscarSala(tienda, IDsala);
    if (sala == NULL) {
        return 0;
    }

    if (nombre != NULL) {
        sala->nombre = copiarTexto(nombre);
    }

    if (tematica != NULL) {
        sala->tematica = copiarTexto(tematica);
    }

    if (horario != NULL) {
        sala->horario = copiarTexto(horario);
    }

    if (aforoMaximo > 0 && aforoMaximo >= sala->clientesDentro) {
        sala->aforoMaximo = aforoMaximo;
    }

    return 1;
}

/* elimina una sala vacia (sin clientes, titulos ni lista de espera): retorna 1 si se elimino */
int eliminarSala(struct Tienda *tienda, int IDsala) {
    int i;
    int pos = -1;

    for (i = 0; i < tienda->cantSalas; i++) {
        if (tienda->salas[i]->IDsala == IDsala) {
            pos = i;
        }
    }

    if (pos == -1) {
        return 0;
    }

    if (tienda->salas[pos]->clientesDentro > 0 ||
        tienda->salas[pos]->titulos.cantidad > 0 ||
        tienda->salas[pos]->espera.cantidad > 0) {
        return 0;
    }

    for (i = pos; i < tienda->cantSalas - 1; i++) {
        tienda->salas[i] = tienda->salas[i + 1];
    }

    tienda->cantSalas--;

    return 1;
}

/* lista las salas con su ocupacion y la cantidad de titulos que alberga */
void listarSalas(struct Tienda *tienda) {
    int i;
    struct Sala *sala;

    if (tienda->cantSalas == 0) {
        printf("No hay salas registradas\n");
        return;
    }

    for (i = 0; i < tienda->cantSalas; i++) {
        sala = tienda->salas[i];
        printf("Sala %d | %s | %s | %s | aforo: %d/%d | titulos: %d\n",
               sala->IDsala, sala->nombre, sala->tematica, sala->horario,
               sala->clientesDentro, sala->aforoMaximo, sala->titulos.cantidad);
    }
}
