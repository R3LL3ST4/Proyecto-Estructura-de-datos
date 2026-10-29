#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

int encolarCliente(struct ListaEspera *lista, struct Cliente *cliente);
int clienteEnEspera(struct ListaEspera *lista, int IDcliente);
struct Cliente *desencolarCliente(struct ListaEspera *lista);

void inicializarListaEspera(struct ListaEspera *lista) {
    if (lista == NULL) {
        return;
    }

    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->cantidad = 0;
}

int solicitarEntradaSala(struct Sala *sala, struct Cliente *cliente) {
    if (sala == NULL || cliente == NULL) {
        return -1;
    }
    /* Evita registrar dos veces al mismo cliente */
    if (cliente->salaActual != 0 || clienteEnEspera(&sala->espera, cliente->IDcliente)) {
        return -1;
    }

    /*Si hay espacio entra*/
    if (sala->clientesDentro < sala->aforoMaximo) {
        sala->clientesDentro++;
        cliente->salaActual = sala->IDsala;
        return 1;
    }

    /*Si esta llena queda esperando*/
    if (encolarCliente(&sala->espera, cliente)) {
        return 0;
    }

    return -1;
}

/* Agrega un cliente al final de la cola */
int encolarCliente(struct ListaEspera *lista, struct Cliente *cliente) {
    struct NodoCliente *nuevo;

    if (lista == NULL || cliente == NULL) {
        return 0;
    }

    if (clienteEnEspera(lista, cliente->IDcliente)) {
        return 0;
    }

    nuevo = malloc(sizeof(struct NodoCliente));

    if (nuevo == NULL) {
        return 0;
    }

    nuevo->cliente = cliente;
    nuevo->sig = NULL;
    nuevo->ant = lista->ultimo;

    if (lista->primero == NULL) {
        lista->primero = nuevo;
    } else {
        lista->ultimo->sig = nuevo;
    }

    lista->ultimo = nuevo;
    lista->cantidad++;

    return 1;
}

/* Comprueba si un cliente ya esta en la cola */
int clienteEnEspera(struct ListaEspera *lista, int IDcliente) {
    struct NodoCliente *actual;

    if (lista == NULL) {
        return 0;
    }

    actual = lista->primero;

    while (actual != NULL) {
        if (actual->cliente != NULL && actual->cliente->IDcliente == IDcliente) {
            return 1;
        }
        actual = actual->sig;
    }
    return 0;
}

/* Retira al primero de la cola */
struct Cliente *desencolarCliente(struct ListaEspera *lista) {
    struct NodoCliente *eliminado;
    struct Cliente *cliente;

    if(lista == NULL || lista->primero == NULL){
        return NULL;
    }

    eliminado = lista->primero;
    cliente = eliminado->cliente;

    lista->primero = eliminado->sig;

    if (lista->primero != NULL) {
        lista->primero->ant = NULL;
    } else {
        lista->ultimo = NULL;
    }

    lista->cantidad--;

    free(eliminado); /*no estoy seguro si es valido*/

    return cliente;
}



int main(){

    return 0;
}