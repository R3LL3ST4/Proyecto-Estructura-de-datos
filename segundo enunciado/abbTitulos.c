/* abbTitulos */

#include "funciones.h"

/* busca un nodo por IDtitulo dentro de un arbol */
struct NodoTitulo *buscarNodoTitulo(struct NodoTitulo *raiz, int IDtitulo) {
    if (raiz == NULL) {
        return NULL;
    }

    if (IDtitulo == raiz->titulo.IDtitulo) {
        return raiz;
    }

    if (IDtitulo < raiz->titulo.IDtitulo) {
        return buscarNodoTitulo(raiz->izquierdo, IDtitulo);
    }

    return buscarNodoTitulo(raiz->derecho, IDtitulo);
}

/* inserta un nodo en el arbol: retorna la raiz y marca insertado en 1 si se agrego */
struct NodoTitulo *insertarNodoTitulo(struct NodoTitulo *raiz, struct NodoTitulo *nuevo, int *insertado) {
    if (raiz == NULL) {
        *insertado = 1;
        return nuevo;
    }

    if (nuevo->titulo.IDtitulo < raiz->titulo.IDtitulo) {
        raiz->izquierdo = insertarNodoTitulo(raiz->izquierdo, nuevo, insertado);
    } else if (nuevo->titulo.IDtitulo > raiz->titulo.IDtitulo) {
        raiz->derecho = insertarNodoTitulo(raiz->derecho, nuevo, insertado);
    }

    return raiz;
}

/* funcion auxiliar para desenlazar el nodo minimo del subarbol */
struct NodoTitulo *quitarMinimo(struct NodoTitulo *raiz) {
    if (raiz->izquierdo == NULL) {
        return raiz->derecho;
    }

    raiz->izquierdo = quitarMinimo(raiz->izquierdo);

    return raiz;
}

/* elimina un nodo del arbol con los 3 casos: hoja, un hijo y dos hijos */
struct NodoTitulo *eliminarNodoTitulo(struct NodoTitulo *raiz, int IDtitulo, int *eliminado) {
    struct NodoTitulo *sucesor;

    if (raiz == NULL) {
        return NULL;
    }

    if (IDtitulo < raiz->titulo.IDtitulo) {
        raiz->izquierdo = eliminarNodoTitulo(raiz->izquierdo, IDtitulo, eliminado);
        return raiz;
    }

    if (IDtitulo > raiz->titulo.IDtitulo) {
        raiz->derecho = eliminarNodoTitulo(raiz->derecho, IDtitulo, eliminado);
        return raiz;
    }

    /* nodo encontrado */
    *eliminado = 1;

    if (raiz->izquierdo == NULL) { /* hoja o solo hijo derecho */
        return raiz->derecho;
    }

    if (raiz->derecho == NULL) { /* solo hijo izquierdo */
        return raiz->izquierdo;
    }

    /* dos hijos: el sucesor inorden pasa a ocupar este nodo */
    sucesor = raiz->derecho;
    while (sucesor->izquierdo != NULL) {
        sucesor = sucesor->izquierdo;
    }

    raiz->titulo = sucesor->titulo;
    raiz->derecho = quitarMinimo(raiz->derecho);

    return raiz;
}

/* muestra los titulos en inorden (ordenados por IDtitulo) */
void listarArbol(struct NodoTitulo *raiz) {
    if (raiz == NULL) {
        return;
    }

    listarArbol(raiz->izquierdo);
    printf("    Titulo %d | %s | %s | $%d | stock: %d | %s\n",
           raiz->titulo.IDtitulo, raiz->titulo.nombre, raiz->titulo.tipo,
           raiz->titulo.precio, raiz->titulo.ejemplaresDisponibles, raiz->titulo.estado);
    listarArbol(raiz->derecho);
}
