/* titulosAsociados */

#include "funciones.h"

/* busca en que sala esta un titulo: retorna NULL si no esta en ninguna */
struct Sala *ubicarTitulo(struct Tienda *tienda, int IDtitulo) {
    int i;

    for (i = 0; i < tienda->cantSalas; i++) {
        if (buscarNodoTitulo(tienda->salas[i]->titulos.raiz, IDtitulo) != NULL) {
            return tienda->salas[i];
        }
    }

    return NULL;
}

/* agrega un titulo a una sala: el IDtitulo debe ser unico en toda la tienda */
int agregarTitulo(struct Tienda *tienda, int IDsala, int IDtitulo, char *nombre, char *genero, char *tipo, int clasificacionEdad, int precio, int ejemplares, int cantDespacho, int demoraDespacho) {
    struct Sala *sala;
    struct NodoTitulo *nodo;
    int insertado = 0;

    if (ejemplares < 0 || precio < 0) {
        return 0;
    }

    sala = buscarSala(tienda, IDsala);
    if (sala == NULL) {
        return 0;
    }

    if (ubicarTitulo(tienda, IDtitulo) != NULL) {
        return 0;
    }

    nodo = (struct NodoTitulo *) malloc(sizeof(struct NodoTitulo));
    nodo->titulo.IDtitulo = IDtitulo;
    nodo->titulo.nombre = copiarTexto(nombre);
    nodo->titulo.genero = copiarTexto(genero);
    nodo->titulo.tipo = copiarTexto(tipo);
    nodo->titulo.clasificacionEdad = clasificacionEdad;
    nodo->titulo.precio = precio;
    nodo->titulo.ejemplaresDisponibles = ejemplares;
    nodo->titulo.cantDespacho = cantDespacho;
    nodo->titulo.demoraDespacho = demoraDespacho;
    nodo->titulo.solicitudesDia = 0;

    if (ejemplares > 0) {
        nodo->titulo.estado = copiarTexto("disponible");
    } else {
        nodo->titulo.estado = copiarTexto("agotado");
    }

    nodo->titulo.espera.primero = NULL;
    nodo->titulo.espera.ultimo = NULL;
    nodo->titulo.espera.cantidad = 0;
    nodo->izquierdo = NULL;
    nodo->derecho = NULL;

    sala->titulos.raiz = insertarNodoTitulo(sala->titulos.raiz, nodo, &insertado);
    if (insertado == 1) {
        sala->titulos.cantidad++;
    }

    return insertado;
}

/* modifica nombre (NULL = igual), precio (<0 = igual) y stock (<0 = igual) de un titulo */
int modificarTitulo(struct Tienda *tienda, int IDtitulo, char *nombre, int precio, int ejemplares) {
    struct Sala *sala;
    struct NodoTitulo *nodo;

    sala = ubicarTitulo(tienda, IDtitulo);
    if (sala == NULL) {
        return 0;
    }

    nodo = buscarNodoTitulo(sala->titulos.raiz, IDtitulo);

    if (nombre != NULL) {
        nodo->titulo.nombre = copiarTexto(nombre);
    }

    if (precio >= 0) {
        nodo->titulo.precio = precio;
    }

    if (ejemplares >= 0) {
        nodo->titulo.ejemplaresDisponibles = ejemplares;

        /* un titulo descatalogado no cambia de estado por stock */
        if (strcmp(nodo->titulo.estado, "descatalogado") != 0) {
            if (ejemplares > 0) {
                nodo->titulo.estado = copiarTexto("disponible");
            } else {
                nodo->titulo.estado = copiarTexto("agotado");
            }
        }
    }

    return 1;
}

/* elimina un titulo de la sala donde este */
int eliminarTitulo(struct Tienda *tienda, int IDtitulo) {
    struct Sala *sala;
    int eliminado = 0;

    sala = ubicarTitulo(tienda, IDtitulo);
    if (sala == NULL) {
        return 0;
    }

    sala->titulos.raiz = eliminarNodoTitulo(sala->titulos.raiz, IDtitulo, &eliminado);
    if (eliminado == 1) {
        sala->titulos.cantidad--;
    }

    return eliminado;
}

/* lista los titulos de una sala en orden de IDtitulo */
void listarTitulosSala(struct Tienda *tienda, int IDsala) {
    struct Sala *sala;

    sala = buscarSala(tienda, IDsala);
    if (sala == NULL) {
        printf("La sala %d no existe\n", IDsala);
        return;
    }

    printf("Titulos de %s:\n", sala->nombre);
    if (sala->titulos.raiz == NULL) {
        printf("    (sin titulos)\n");
        return;
    }

    listarArbol(sala->titulos.raiz);
}
