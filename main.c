#define _CRT_SECURE_NO_WARNINGS  //solo en caso de que tire un error en visual studio a causa de extensiones
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

#define MAX_SALAS 4
/* ------------------------------------------------------------------ */
/* PROTOTIPOS DE FUNCIONES                                            */
/* ------------------------------------------------------------------ */

char* copiarTexto(char* origen);
int inicializarTienda(struct Tienda* tienda, int capSalas, int capClientes, int aforoTienda);

/* Pases y Control de Acceso */
int validarPase(struct Cliente* cliente);
int comprarPase(struct Tienda* tienda, struct Cliente* cliente, int tipoOpcion);
int anularPaseCliente(struct Tienda* tienda, int IDcliente);
void listarPasesTienda(struct Tienda* tienda);

/* Listas de Espera (FIFO) */
void inicializarListaEspera(struct ListaEspera* lista);
int clienteEnEspera(struct ListaEspera* lista, int IDcliente);
int encolarCliente(struct ListaEspera* lista, struct Cliente* cliente);
struct Cliente* desencolarCliente(struct ListaEspera* lista);

/* ABB Títulos */
struct NodoTitulo* buscarNodoTitulo(struct NodoTitulo* raiz, int IDtitulo);
struct NodoTitulo* insertarNodoTitulo(struct NodoTitulo* raiz, struct NodoTitulo* nuevo, int* insertado);
struct NodoTitulo* quitarMinimo(struct NodoTitulo* raiz);
struct NodoTitulo* eliminarNodoTitulo(struct NodoTitulo* raiz, int IDtitulo, int* eliminado);
void listarArbol(struct NodoTitulo* raiz);

/* Salas */
int inicializarSalas(struct Tienda* tienda, int capacidad);
struct Sala* buscarSala(struct Tienda* tienda, int IDsala);
int crearSala(struct Tienda* tienda, int IDsala, char* nombre, char* tematica, char* horario, int aforoMaximo);
void listarSalas(struct Tienda* tienda);

/* Mostrador, Títulos y Despachos */
struct Sala* ubicarTitulo(struct Tienda* tienda, int IDtitulo);
int procesarEscaneoMostrador(struct Tienda* tienda, int IDcliente, int IDsala, int IDtitulo, char* nombre, char* tipoPub, int clasif, int precio, int ejemplares, int cantDesp, int demoraDesp, int* tituloNuevo);
int procesarLlegadaDespacho(struct Tienda* tienda, int IDtitulo, int cantidadLlegada);
int descatalogarTitulo(struct Tienda* tienda, int IDtitulo);
int eliminarTitulo(struct Tienda* tienda, int IDtitulo);
void listarTitulosSala(struct Tienda* tienda, int IDsala);

/* Clientes */
int inicializarClientes(struct Tienda* tienda, int capacidad);
struct Cliente* buscarCliente(struct Tienda* tienda, int IDcliente);
int crearCliente(struct Tienda* tienda, char* nombre, int edad);
int modificarCliente(struct Tienda* tienda, int IDcliente, char* nuevoNombre, int nuevaEdad);
int eliminarCliente(struct Tienda* tienda, int IDcliente);
void listarClientes(struct Tienda* tienda);

/* Acceso y Salida Tienda / Salas */
int ingresoAutomaticoCliente(struct Tienda* tienda, int IDcliente, int IDsalaDestino, int seleccionPase, int decisionCompra);
int cambiarSalaCliente(struct Tienda* tienda, int IDcliente, int IDsalaNueva);
int salidaAutomaticaCliente(struct Tienda* tienda, int IDcliente);

/* Bitácora y Reportes */
void inicializarBitacora(struct Bitacora* bitacora);
int registrarEvento(struct Tienda* tienda, char* tipoEvento, int IDcliente, int IDsala, int IDtitulo);
void listarBitacora(struct Tienda* tienda);
void generarReporteCierre(struct Tienda* tienda);
int estimarTiempoEsperaTitulo(struct Tienda* tienda, int IDtitulo, int* despachos, int* dias);

/* ------------------------------------------------------------------ */
/* IMPLEMENTACIÓN DE FUNCIONES                                        */
/* ------------------------------------------------------------------ */

char* copiarTexto(char* origen) {
    char* copia;
    if (origen == NULL) return NULL;
    copia = (char*)malloc(strlen(origen) + 1);
    if (copia != NULL) strcpy(copia, origen);
    return copia;
}

int inicializarTienda(struct Tienda* tienda, int capSalas, int capClientes, int aforoTienda) {
    if (tienda == NULL) return 0;
    tienda->cantSalas = 0;
    tienda->capacidadSalas = 0;
    tienda->salas = NULL;
    tienda->cantidadClientes = 0;
    tienda->clientes = NULL;
    tienda->aforoTienda = aforoTienda;
    tienda->clientesDentro = 0;
    tienda->clientesIngresadosHoy = 0;
    tienda->pasesVendidos = 0;
    tienda->pasesUtilizadosDia = 0;
    tienda->gananciaPases = 0;
    tienda->gananciaHoy = 0;
    tienda->siguienteIdCliente = 1;
    tienda->siguienteIdPase = 1;
    tienda->siguienteIdEvento = 1;

    if (!inicializarSalas(tienda, capSalas)) return 0;
    if (!inicializarClientes(tienda, capClientes)) return 0;
    inicializarBitacora(&tienda->bitacora);
    return 1;
}

int validarPase(struct Cliente* cliente) {
    if (cliente == NULL) return 0;
    if (cliente->pase.estado != NULL && strcmp(cliente->pase.estado, "activo") == 0 && cliente->pase.IDpase > 0) {
        return 1;
    }
    return 0;
}

int comprarPase(struct Tienda* tienda, struct Cliente* cliente, int tipoOpcion) {
    char* tipoPaseStr;
    int valorPase;

    if (tienda == NULL || cliente == NULL) return 0;
    switch (tipoOpcion) {
    case 2: tipoPaseStr = "infantil"; valorPase = 3000; break;
    case 3: tipoPaseStr = "familiar"; valorPase = 16000; break;
    case 4: tipoPaseStr = "prioritario"; valorPase = 10000; break;
    case 1:
    default: tipoPaseStr = "general"; valorPase = 5000; break;
    }

    cliente->pase.IDpase = tienda->siguienteIdPase++;
    cliente->pase.tipo = copiarTexto(tipoPaseStr);
    cliente->pase.valor = valorPase;
    cliente->pase.estado = copiarTexto("activo");

    tienda->pasesVendidos++;
    tienda->gananciaPases += valorPase;
    tienda->gananciaHoy += valorPase;
    return 1;
}

int anularPaseCliente(struct Tienda* tienda, int IDcliente) {
    struct Cliente* c = buscarCliente(tienda, IDcliente);
    if (c == NULL || c->pase.IDpase == 0) return 0;
    if (c->pase.estado != NULL && strcmp(c->pase.estado, "anulado") == 0) return 0;
    c->pase.estado = copiarTexto("anulado");
    registrarEvento(tienda, "pase anulado", IDcliente, 0, 0);
    return 1;
}

void listarPasesTienda(struct Tienda* tienda) {
    int i;
    if (tienda == NULL || tienda->cantidadClientes == 0) {
        printf("No hay pases registrados.\n");
        return;
    }
    printf("\n================= ESTADO DE PASES DE CLIENTES =================");
    for (i = 0; i < tienda->cantidadClientes; i++) {
        if (tienda->clientes[i] && tienda->clientes[i]->cliente) {
            struct Cliente* c = tienda->clientes[i]->cliente;
            printf("\nCliente ID: %d (%s) | Pase ID: %d | Tipo: %s | Estado: %s",
                c->IDcliente, c->nombre, c->pase.IDpase,
                c->pase.tipo ? c->pase.tipo : "Ninguno",
                c->pase.estado ? c->pase.estado : "Inactivo");
        }
    }
    printf("\nPases utilizados hoy: %d\n===============================================================\n", tienda->pasesUtilizadosDia);
}

void inicializarListaEspera(struct ListaEspera* lista) {
    if (lista == NULL) return;
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->cantidad = 0;
}

int clienteEnEspera(struct ListaEspera* lista, int IDcliente) {
    struct NodoCliente* actual;
    if (lista == NULL) return 0;
    actual = lista->primero;
    while (actual != NULL) {
        if (actual->cliente && actual->cliente->IDcliente == IDcliente) return 1;
        actual = actual->sig;
    }
    return 0;
}

int encolarCliente(struct ListaEspera* lista, struct Cliente* cliente) {
    struct NodoCliente* nuevo;
    if (lista == NULL || cliente == NULL) return 0;
    if (clienteEnEspera(lista, cliente->IDcliente)) return 0;

    nuevo = (struct NodoCliente*)malloc(sizeof(struct NodoCliente));
    if (nuevo == NULL) return 0;
    nuevo->cliente = cliente;
    nuevo->sig = NULL;
    nuevo->ant = lista->ultimo;

    if (lista->primero == NULL) lista->primero = nuevo;
    else lista->ultimo->sig = nuevo;
    lista->ultimo = nuevo;
    lista->cantidad++;
    return 1;
}

struct Cliente* desencolarCliente(struct ListaEspera* lista) {
    struct NodoCliente* eliminado;
    struct Cliente* cliente;
    if (lista == NULL || lista->primero == NULL) return NULL;

    eliminado = lista->primero;
    cliente = eliminado->cliente;
    lista->primero = eliminado->sig;
    if (lista->primero != NULL) lista->primero->ant = NULL;
    else lista->ultimo = NULL;
    lista->cantidad--;
    return cliente;
}

struct NodoTitulo* buscarNodoTitulo(struct NodoTitulo* raiz, int IDtitulo) {
    if (raiz == NULL) return NULL;
    if (IDtitulo == raiz->titulo.IDtitulo) return raiz;
    if (IDtitulo < raiz->titulo.IDtitulo) return buscarNodoTitulo(raiz->izquierdo, IDtitulo);
    return buscarNodoTitulo(raiz->derecho, IDtitulo);
}

struct NodoTitulo* insertarNodoTitulo(struct NodoTitulo* raiz, struct NodoTitulo* nuevo, int* insertado) {
    if (raiz == NULL) { *insertado = 1; return nuevo; }
    if (nuevo->titulo.IDtitulo < raiz->titulo.IDtitulo)
        raiz->izquierdo = insertarNodoTitulo(raiz->izquierdo, nuevo, insertado);
    else if (nuevo->titulo.IDtitulo > raiz->titulo.IDtitulo)
        raiz->derecho = insertarNodoTitulo(raiz->derecho, nuevo, insertado);
    return raiz;
}

struct NodoTitulo* quitarMinimo(struct NodoTitulo* raiz) {
    if (raiz->izquierdo == NULL) return raiz->derecho;
    raiz->izquierdo = quitarMinimo(raiz->izquierdo);
    return raiz;
}

struct NodoTitulo* eliminarNodoTitulo(struct NodoTitulo* raiz, int IDtitulo, int* eliminado) {
    struct NodoTitulo* sucesor;
    if (raiz == NULL) return NULL;
    if (IDtitulo < raiz->titulo.IDtitulo) { raiz->izquierdo = eliminarNodoTitulo(raiz->izquierdo, IDtitulo, eliminado); return raiz; }
    if (IDtitulo > raiz->titulo.IDtitulo) { raiz->derecho = eliminarNodoTitulo(raiz->derecho, IDtitulo, eliminado); return raiz; }

    *eliminado = 1;
    if (raiz->izquierdo == NULL) return raiz->derecho;
    if (raiz->derecho == NULL) return raiz->izquierdo;

    sucesor = raiz->derecho;
    while (sucesor->izquierdo != NULL) sucesor = sucesor->izquierdo;
    raiz->titulo = sucesor->titulo;
    raiz->derecho = quitarMinimo(raiz->derecho);
    return raiz;
}

void listarArbol(struct NodoTitulo* raiz) {
    if (raiz == NULL) return;
    listarArbol(raiz->izquierdo);
    printf("    [ID: %d] %s | Tipo: %s | Edad: %d+ | $%d | Stock: %d | Estado: %s | Solicitudes: %d | En espera: %d\n",
        raiz->titulo.IDtitulo, raiz->titulo.nombre, raiz->titulo.tipo, raiz->titulo.clasificacionEdad,
        raiz->titulo.precio, raiz->titulo.ejemplaresDisponibles, raiz->titulo.estado,
        raiz->titulo.solicitudesDia, raiz->titulo.espera.cantidad);
    listarArbol(raiz->derecho);
}

int inicializarSalas(struct Tienda* tienda, int capacidad) {
    tienda->cantSalas = 0;
    tienda->capacidadSalas = 0;
    tienda->salas = NULL;
    if (capacidad <= 0) return 0;
    tienda->salas = (struct Sala**)malloc(capacidad * sizeof(struct Sala*));
    if (tienda->salas == NULL) return 0;
    tienda->capacidadSalas = capacidad;
    return 1;
}

struct Sala* buscarSala(struct Tienda* tienda, int IDsala) {
    int i;
    if (tienda == NULL || tienda->salas == NULL) return NULL;
    for (i = 0; i < tienda->cantSalas; i++) {
        if (tienda->salas[i]->IDsala == IDsala) return tienda->salas[i];
    }
    return NULL;
}

int crearSala(struct Tienda* tienda, int IDsala, char* nombre, char* tematica, char* horario, int aforoMaximo) {
    struct Sala* sala;
    if (aforoMaximo <= 0 || tienda->cantSalas >= tienda->capacidadSalas) return 0;
    if (buscarSala(tienda, IDsala) != NULL) return 0;

    sala = (struct Sala*)malloc(sizeof(struct Sala));
    if (sala == NULL) return 0;
    sala->IDsala = IDsala;
    sala->nombre = copiarTexto(nombre);
    sala->tematica = copiarTexto(tematica);
    sala->horario = copiarTexto(horario);
    sala->aforoMaximo = aforoMaximo;
    sala->clientesDentro = 0;
    inicializarListaEspera(&sala->espera);
    sala->titulos.raiz = NULL;
    sala->titulos.cantidad = 0;

    tienda->salas[tienda->cantSalas] = sala;
    tienda->cantSalas++;
    return 1;
}

void listarSalas(struct Tienda* tienda) {
    int i;
    if (tienda->cantSalas == 0) { printf("No hay salas registradas.\n"); return; }
    for (i = 0; i < tienda->cantSalas; i++) {
        struct Sala* s = tienda->salas[i];
        printf("Sala ID: %d | %s | Temática: %s | Horario: %s | Aforo: %d/%d | Títulos: %d\n",
            s->IDsala, s->nombre, s->tematica, s->horario, s->clientesDentro, s->aforoMaximo, s->titulos.cantidad);
    }
}

struct Sala* ubicarTitulo(struct Tienda* tienda, int IDtitulo) {
    int i;
    if (tienda == NULL || tienda->salas == NULL) return NULL;
    for (i = 0; i < tienda->cantSalas; i++) {
        if (buscarNodoTitulo(tienda->salas[i]->titulos.raiz, IDtitulo) != NULL) return tienda->salas[i];
    }
    return NULL;
}

/* Retorna: 1 ejemplar entregado, 2 cliente en lista de espera, 0 error,
   -1 cliente inexistente o fuera de la tienda, -2 sala inexistente,
   -3 titulo descatalogado sin ejemplares, -4 edad insuficiente,
   -5 cliente no esta en esa sala, -6 titulo pertenece a otra sala,
   -7 cliente ya esta en la lista de espera del titulo */
int procesarEscaneoMostrador(struct Tienda* tienda, int IDcliente, int IDsala, int IDtitulo, char* nombre, char* tipoPub, int clasif, int precio, int ejemplares, int cantDesp, int demoraDesp, int* tituloNuevo) {
    struct Sala* sala;
    struct Sala* salaTitulo;
    struct NodoTitulo* nodo;
    int insertado = 0;
    struct Cliente* cliente = buscarCliente(tienda, IDcliente);

    *tituloNuevo = 0;
    if (cliente == NULL || cliente->dentroTienda == 0) return -1;

    sala = buscarSala(tienda, IDsala);
    if (sala == NULL) return -2;
    if (cliente->salaActual != IDsala) return -5;

    salaTitulo = ubicarTitulo(tienda, IDtitulo);
    if (salaTitulo != NULL && salaTitulo != sala) return -6;

    nodo = buscarNodoTitulo(sala->titulos.raiz, IDtitulo);
    if (nodo == NULL) {
        if (ejemplares < 0 || precio < 0 || clasif < 0) return 0;
        nodo = (struct NodoTitulo*)malloc(sizeof(struct NodoTitulo));
        if (nodo == NULL) return 0;

        nodo->titulo.IDtitulo = IDtitulo;
        nodo->titulo.nombre = copiarTexto(nombre);
        nodo->titulo.genero = copiarTexto(sala->tematica);
        nodo->titulo.tipo = copiarTexto(tipoPub);
        nodo->titulo.clasificacionEdad = clasif;
        nodo->titulo.precio = precio;
        nodo->titulo.ejemplaresDisponibles = ejemplares;
        nodo->titulo.cantDespacho = cantDesp;
        nodo->titulo.demoraDespacho = demoraDesp;
        nodo->titulo.solicitudesDia = 0;

        if (ejemplares > 0) {
            nodo->titulo.estado = copiarTexto("disponible");
        }
        else {
            nodo->titulo.estado = copiarTexto("agotado");
            registrarEvento(tienda, "título agotado", 0, IDsala, IDtitulo);
        }

        inicializarListaEspera(&nodo->titulo.espera);
        nodo->izquierdo = NULL;
        nodo->derecho = NULL;

        sala->titulos.raiz = insertarNodoTitulo(sala->titulos.raiz, nodo, &insertado);
        if (insertado == 1) sala->titulos.cantidad++;
        *tituloNuevo = 1;
    }

    if (cliente->edad < nodo->titulo.clasificacionEdad) return -4;

    if (nodo->titulo.ejemplaresDisponibles > 0) {
        nodo->titulo.solicitudesDia++;
        nodo->titulo.ejemplaresDisponibles--;
        tienda->gananciaHoy += nodo->titulo.precio;
        if (nodo->titulo.ejemplaresDisponibles == 0 && strcmp(nodo->titulo.estado, "descatalogado") != 0) {
            nodo->titulo.estado = copiarTexto("agotado");
            registrarEvento(tienda, "título agotado", 0, IDsala, IDtitulo);
        }
        registrarEvento(tienda, "escaneo_mostrador", IDcliente, IDsala, IDtitulo);
        return 1;
    }

    if (strcmp(nodo->titulo.estado, "descatalogado") == 0) return -3;
    if (clienteEnEspera(&nodo->titulo.espera, IDcliente)) return -7;
    if (!encolarCliente(&nodo->titulo.espera, cliente)) return 0;

    nodo->titulo.solicitudesDia++;
    registrarEvento(tienda, "cliente en espera de título", IDcliente, IDsala, IDtitulo);
    registrarEvento(tienda, "escaneo_mostrador", IDcliente, IDsala, IDtitulo);
    return 2;
}

int procesarLlegadaDespacho(struct Tienda* tienda, int IDtitulo, int cantidadLlegada) {
    struct Sala* sala = ubicarTitulo(tienda, IDtitulo);
    struct NodoTitulo* nodo;
    if (sala == NULL || cantidadLlegada <= 0) return 0;

    nodo = buscarNodoTitulo(sala->titulos.raiz, IDtitulo);
    if (nodo == NULL || strcmp(nodo->titulo.estado, "descatalogado") == 0) return 0;

    nodo->titulo.ejemplaresDisponibles += cantidadLlegada;
    if (nodo->titulo.ejemplaresDisponibles > 0 && strcmp(nodo->titulo.estado, "agotado") == 0) {
        nodo->titulo.estado = copiarTexto("disponible");
    }

    while (nodo->titulo.espera.cantidad > 0 && nodo->titulo.ejemplaresDisponibles > 0) {
        struct Cliente* cAtendido = desencolarCliente(&nodo->titulo.espera);
        if (cAtendido != NULL) {
            nodo->titulo.ejemplaresDisponibles--;
            tienda->gananciaHoy += nodo->titulo.precio;
            registrarEvento(tienda, "titulo entregado por lista de espera", cAtendido->IDcliente, sala->IDsala, IDtitulo);
        }
    }
    if (nodo->titulo.ejemplaresDisponibles == 0) {
        nodo->titulo.estado = copiarTexto("agotado");
    }
    return 1;
}

int descatalogarTitulo(struct Tienda* tienda, int IDtitulo) {
    struct Sala* sala = ubicarTitulo(tienda, IDtitulo);
    struct NodoTitulo* nodo;
    if (sala == NULL) return 0;
    nodo = buscarNodoTitulo(sala->titulos.raiz, IDtitulo);
    if (nodo == NULL) return 0;
    if (strcmp(nodo->titulo.estado, "descatalogado") == 0) return 0;

    nodo->titulo.estado = copiarTexto("descatalogado");
    registrarEvento(tienda, "título descatalogado", 0, sala->IDsala, IDtitulo);
    return 1;
}

int eliminarTitulo(struct Tienda* tienda, int IDtitulo) {
    struct Sala* sala = ubicarTitulo(tienda, IDtitulo);
    int eliminado = 0;
    if (sala == NULL) return 0;
    sala->titulos.raiz = eliminarNodoTitulo(sala->titulos.raiz, IDtitulo, &eliminado);
    if (eliminado == 1) sala->titulos.cantidad--;
    return eliminado;
}

void listarTitulosSala(struct Tienda* tienda, int IDsala) {
    struct Sala* sala = buscarSala(tienda, IDsala);
    if (sala == NULL) { printf("Sala no existe.\n"); return; }
    printf("\n--- TÍTULOS EN SALA '%s' ---\n", sala->nombre);
    if (sala->titulos.raiz == NULL) { printf("    (Sin títulos registrados)\n"); return; }
    listarArbol(sala->titulos.raiz);
}

int inicializarClientes(struct Tienda* tienda, int capacidad) {
    tienda->cantidadClientes = 0;
    tienda->clientes = NULL;
    if (capacidad <= 0) return 0;
    tienda->clientes = (struct NodoCliente**)malloc(capacidad * sizeof(struct NodoCliente*));
    if (tienda->clientes == NULL) return 0;
    return 1;
}

struct Cliente* buscarCliente(struct Tienda* tienda, int IDcliente) {
    int i;
    if (tienda == NULL || tienda->clientes == NULL) return NULL;
    for (i = 0; i < tienda->cantidadClientes; i++) {
        if (tienda->clientes[i] && tienda->clientes[i]->cliente && tienda->clientes[i]->cliente->IDcliente == IDcliente) {
            return tienda->clientes[i]->cliente;
        }
    }
    return NULL;
}

int crearCliente(struct Tienda* tienda, char* nombre, int edad) {
    struct Cliente* nuevoC;
    struct NodoCliente* nuevoN;
    if (tienda == NULL || nombre == NULL || edad < 0) return 0;
    if (tienda->clientes == NULL) return 0;

    nuevoC = (struct Cliente*)malloc(sizeof(struct Cliente));
    if (nuevoC == NULL) return 0;

    nuevoC->IDcliente = tienda->siguienteIdCliente++;
    nuevoC->nombre = copiarTexto(nombre);
    nuevoC->edad = edad;
    nuevoC->dentroTienda = 0;
    nuevoC->salaActual = 0;
    nuevoC->pase.IDpase = 0;
    nuevoC->pase.tipo = NULL;
    nuevoC->pase.valor = 0;
    nuevoC->pase.estado = copiarTexto("inactivo");

    nuevoN = (struct NodoCliente*)malloc(sizeof(struct NodoCliente));
    if (nuevoN == NULL) return 0;
    nuevoN->cliente = nuevoC;
    nuevoN->sig = NULL;
    nuevoN->ant = NULL;

    tienda->clientes[tienda->cantidadClientes] = nuevoN;
    tienda->cantidadClientes++;
    return nuevoC->IDcliente;
}

int modificarCliente(struct Tienda* tienda, int IDcliente, char* nuevoNombre, int nuevaEdad) {
    struct Cliente* c = buscarCliente(tienda, IDcliente);
    if (c == NULL) return 0;
    if (nuevoNombre != NULL) c->nombre = copiarTexto(nuevoNombre);
    if (nuevaEdad >= 0) c->edad = nuevaEdad;
    return 1;
}

int eliminarCliente(struct Tienda* tienda, int IDcliente) {
    int i, pos = -1;
    for (i = 0; i < tienda->cantidadClientes; i++) {
        if (tienda->clientes[i] && tienda->clientes[i]->cliente && tienda->clientes[i]->cliente->IDcliente == IDcliente) {
            pos = i; break;
        }
    }
    if (pos == -1) return 0;
    if (tienda->clientes[pos]->cliente->dentroTienda == 1) return -1;

    for (i = pos; i < tienda->cantidadClientes - 1; i++) {
        tienda->clientes[i] = tienda->clientes[i + 1];
    }
    tienda->cantidadClientes--;
    return 1;
}

void listarClientes(struct Tienda* tienda) {
    int i;
    if (tienda == NULL || tienda->cantidadClientes == 0) { printf("No hay clientes.\n"); return; }
    printf("\n================ LISTADO DE CLIENTES ================\n");
    for (i = 0; i < tienda->cantidadClientes; i++) {
        if (tienda->clientes[i] && tienda->clientes[i]->cliente) {
            struct Cliente* c = tienda->clientes[i]->cliente;
            printf("ID: %d | Nombre: %s | Edad: %d | Dentro: %s | Sala: %d | Pase: %s\n",
                c->IDcliente, c->nombre, c->edad, c->dentroTienda ? "Sí" : "No", c->salaActual, c->pase.estado ? c->pase.estado : "Sin pase");
        }
    }
    printf("=====================================================\n");
}

int ingresoAutomaticoCliente(struct Tienda* tienda, int IDcliente, int IDsalaDestino, int seleccionPase, int decisionCompra) {
    struct Cliente* cliente = buscarCliente(tienda, IDcliente);
    struct Sala* sala = buscarSala(tienda, IDsalaDestino);
    if (cliente == NULL || sala == NULL) return 0;
    if (cliente->dentroTienda == 1) return -1;

    if (tienda->clientesDentro >= tienda->aforoTienda) return -3;
    if (sala->clientesDentro >= sala->aforoMaximo) return -4;

    if (!validarPase(cliente)) {
        if (decisionCompra == 2) return -2;
        if (!comprarPase(tienda, cliente, seleccionPase)) return 0;
    }

    if (cliente->pase.estado && strcmp(cliente->pase.estado, "activo") == 0) {
        cliente->pase.estado = copiarTexto("utilizado");
        tienda->pasesUtilizadosDia++;
    }

    cliente->dentroTienda = 1;
    cliente->salaActual = IDsalaDestino;
    tienda->clientesDentro++;
    sala->clientesDentro++;
    tienda->clientesIngresadosHoy++;

    registrarEvento(tienda, "ingreso a tienda y sala", cliente->IDcliente, IDsalaDestino, 0);
    return 1;
}

int cambiarSalaCliente(struct Tienda* tienda, int IDcliente, int IDsalaNueva) {
    struct Cliente* cliente = buscarCliente(tienda, IDcliente);
    struct Sala* nueva = buscarSala(tienda, IDsalaNueva);
    struct Sala* anterior;
    if (cliente == NULL || nueva == NULL) return 0;
    if (cliente->dentroTienda == 0) return -1;
    if (cliente->salaActual == IDsalaNueva) return 1;
    if (nueva->clientesDentro >= nueva->aforoMaximo) return -4;

    anterior = buscarSala(tienda, cliente->salaActual);
    if (anterior != NULL && anterior->clientesDentro > 0) anterior->clientesDentro--;
    nueva->clientesDentro++;
    cliente->salaActual = IDsalaNueva;
    return 1;
}

int salidaAutomaticaCliente(struct Tienda* tienda, int IDcliente) {
    struct Cliente* cliente = buscarCliente(tienda, IDcliente);
    struct Sala* sala;
    if (cliente == NULL || cliente->dentroTienda == 0) return -1;

    sala = buscarSala(tienda, cliente->salaActual);
    if (sala && sala->clientesDentro > 0) {
        sala->clientesDentro--;
    }

    registrarEvento(tienda, "salida", cliente->IDcliente, cliente->salaActual, 0);
    cliente->dentroTienda = 0;
    cliente->salaActual = 0;
    if (tienda->clientesDentro > 0) tienda->clientesDentro--;
    return 1;
}

void inicializarBitacora(struct Bitacora* bitacora) {
    if (bitacora == NULL) return;
    bitacora->primero = NULL;
    bitacora->ultimo = NULL;
    bitacora->cantidad = 0;
}

int registrarEvento(struct Tienda* tienda, char* tipoEvento, int IDcliente, int IDsala, int IDtitulo) {
    struct NodoEvento* nuevo = (struct NodoEvento*)malloc(sizeof(struct NodoEvento));
    if (nuevo == NULL) return 0;
    nuevo->evento.IDevento = tienda->siguienteIdEvento++;
    nuevo->evento.tipo = copiarTexto(tipoEvento);
    nuevo->evento.IDcliente = IDcliente;
    nuevo->evento.IDsala = IDsala;
    nuevo->evento.IDtitulo = IDtitulo;
    nuevo->sig = NULL;

    if (tienda->bitacora.primero == NULL) {
        tienda->bitacora.primero = nuevo;
        tienda->bitacora.ultimo = nuevo;
    }
    else {
        tienda->bitacora.ultimo->sig = nuevo;
        tienda->bitacora.ultimo = nuevo;
    }
    tienda->bitacora.cantidad++;
    return 1;
}

void listarBitacora(struct Tienda* tienda) {
    struct NodoEvento* actual = tienda->bitacora.primero;
    if (actual == NULL) { printf("Bitácora vacía.\n"); return; }
    printf("\n==================== BITÁCORA DE EVENTOS ====================\n");
    while (actual != NULL) {
        printf("ID: %d | Tipo: %-25s | Cliente: %d | Sala: %d | Título: %d\n",
            actual->evento.IDevento, actual->evento.tipo, actual->evento.IDcliente, actual->evento.IDsala, actual->evento.IDtitulo);
        actual = actual->sig;
    }
    printf("=============================================================\n");
}

void generarReporteCierre(struct Tienda* tienda) {
    printf("\n================ REPORTE DE CIERRE DIARIO ================\n");
    printf(" Clientes ingresados hoy     : %d\n", tienda->clientesIngresadosHoy);
    printf(" Clientes actuales en tienda : %d\n", tienda->clientesDentro);
    printf(" Pases vendidos hoy          : %d\n", tienda->pasesVendidos);
    printf(" Pases utilizados hoy        : %d\n", tienda->pasesUtilizadosDia);
    printf(" Ganancia total por pases    : $%d\n", tienda->gananciaPases);
    printf(" Ganancia total del día      : $%d\n", tienda->gananciaHoy);
    printf(" Total eventos en bitácora   : %d\n", tienda->bitacora.cantidad);
    printf("===========================================================\n");
}

int estimarTiempoEsperaTitulo(struct Tienda* tienda, int IDtitulo, int* despachos, int* dias) {
    struct Sala* sala = ubicarTitulo(tienda, IDtitulo);
    struct NodoTitulo* nodo;
    if (sala == NULL) return -1;
    nodo = buscarNodoTitulo(sala->titulos.raiz, IDtitulo);
    if (nodo == NULL) return -1;

    if (nodo->titulo.espera.cantidad == 0) return 1;
    if (nodo->titulo.cantDespacho <= 0 || nodo->titulo.demoraDespacho <= 0) return -2;

    *despachos = (nodo->titulo.espera.cantidad + nodo->titulo.cantDespacho - 1) / nodo->titulo.cantDespacho;
    *dias = (*despachos) * nodo->titulo.demoraDespacho;
    return 2;
}

/* ------------------------------------------------------------------ */
/* FUNCIÓN PRINCIPAL (MAIN COMPLETO ANSI C)                           */
/* ------------------------------------------------------------------ */

int main(void) {
    struct Tienda miTienda;
    int opcion = 0, subOpcion = 0;
    int aforoTotalTienda, cantSalasConfig;
    int id, id2, edad, precio, stock, cantDesp, demoraDesp, clasif, aforoSala;
    char nombre[100], tipoPub[50];
    int res, selPase, decCompra, desp, dias, i, seleccionTematica, j, nuevoTitulo;

    char* tematicasOficiales[MAX_SALAS];
    int tematicaUsada[MAX_SALAS];
    struct Cliente* c;
    struct Cliente* cli;

    tematicasOficiales[0] = "Superheroes";
    tematicasOficiales[1] = "Manga/Anime";
    tematicasOficiales[2] = "Novela Grafica";
    tematicasOficiales[3] = "Infantil/Humor Grafico";

    for (j = 0; j < MAX_SALAS; j++) {
        tematicaUsada[j] = 0;
    }

    printf("==================================================\n");
    printf("     SISTEMA DE GESTION - IBC-omics Inc.          \n");
    printf("==================================================\n");

    do {
        printf("Ingrese la capacidad maxima de aforo de la tienda: ");
        scanf("%d", &aforoTotalTienda);
        if (aforoTotalTienda <= 0) printf("-> Error: El aforo debe ser mayor a 0.\n");
    } while (aforoTotalTienda <= 0);

    do {
        printf("¿Cuántas salas posee la tienda? (Entre 1 y %d): ", MAX_SALAS);
        scanf("%d", &cantSalasConfig);
        if (cantSalasConfig <= 0 || cantSalasConfig > MAX_SALAS) printf("-> Error: Debe ser entre 1 y %d.\n", MAX_SALAS);
    } while (cantSalasConfig <= 0 || cantSalasConfig > MAX_SALAS);

    if (!inicializarTienda(&miTienda, cantSalasConfig, 50, aforoTotalTienda)) {
        printf("Error al inicializar la tienda.\n");
        return 1;
    }

    for (i = 0; i < cantSalasConfig; i++) {
        char* nombreTematica;
        printf("\nConfigurando Sala %d de %d:\n", i + 1, cantSalasConfig);
        do {
            printf("  - ID de la Sala (entero mayor a 0): "); scanf("%d", &id);
            if (id <= 0 || buscarSala(&miTienda, id) != NULL) printf("-> Error: ID invalido o ya existente.\n");
        } while (id <= 0 || buscarSala(&miTienda, id) != NULL);
        printf("  - Seleccione la temática fija:\n");
        for (j = 0; j < MAX_SALAS; j++) {
            if (!tematicaUsada[j]) printf("    [%d] %s\n", j + 1, tematicasOficiales[j]);
        }
        do {
            scanf("%d", &seleccionTematica);
        } while (seleccionTematica < 1 || seleccionTematica > MAX_SALAS || tematicaUsada[seleccionTematica - 1]);

        tematicaUsada[seleccionTematica - 1] = 1;
        nombreTematica = tematicasOficiales[seleccionTematica - 1];

        do {
            printf("  - Aforo máximo personalizado para esta sala: ");
            scanf("%d", &aforoSala);
            if (aforoSala <= 0) printf("-> Error: El aforo debe ser mayor a 0.\n");
        } while (aforoSala <= 0);

        crearSala(&miTienda, id, nombreTematica, nombreTematica, "Lunes a Domingo", aforoSala);
    }

    do {
        printf("\n=================== MENU PRINCIPAL ===================\n");
        printf("1. Gestion de Salas\n");
        printf("2. Mostrador de Atencion (Escaneo y Solicitud de Titulos)\n");
        printf("3. Gestion de Clientes (Registrar, Buscar, Modificar, Eliminar)\n");
        printf("4. Control de Acceso y Salida de Tienda\n");
        printf("5. Gestion de Pases (Listar y Anular)\n");
        printf("6. Gestion de Títulos (Listar, Eliminar, Descatalogar, Despachos)\n");
        printf("7. Bitacora y Reportes Analiticos (Cierre, Tiempos de Espera)\n");
        printf("0. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1:
            listarSalas(&miTienda);
            break;

        case 2:
            printf("\n--- MOSTRADOR DE ATENCION ---\n");
            printf("ID del cliente: "); scanf("%d", &id);
            c = buscarCliente(&miTienda, id);
            if (!c) { printf("-> Error: Cliente no encontrado.\n"); break; }

            printf("ID de la Sala a visitar: "); scanf("%d", &id2);

            if (c->dentroTienda == 0) {
                selPase = 1; decCompra = 1;
                if (!validarPase(c)) {
                    printf("El cliente no tiene pase activo. 1. Comprar pase | 2. Retirarse: ");
                    scanf("%d", &decCompra);
                    if (decCompra == 1) {
                        printf("Tipo (1. General $5k, 2. Infantil $3k, 3. Familiar $16k, 4. Prioritario $10k): ");
                        scanf("%d", &selPase);
                    }
                }
                res = ingresoAutomaticoCliente(&miTienda, id, id2, selPase, decCompra);
                if (res == -3) { printf("-> Error: Aforo máximo de la tienda alcanzado.\n"); break; }
                if (res == -4) { printf("-> Error: La sala seleccionada está llena.\n"); break; }
                if (res == -2) { printf("-> El cliente decidió retirarse.\n"); break; }
                if (res <= 0) { printf("-> Error al ingresar (sala no encontrada).\n"); break; }
                printf("-> Acceso concedido a la tienda y sala. Pase marcado como utilizado.\n");
            }
            else {
                res = cambiarSalaCliente(&miTienda, c->IDcliente, id2);
                if (res == -4) { printf("-> Error: La sala seleccionada está llena.\n"); break; }
                if (res <= 0) { printf("-> Error: Sala no encontrada.\n"); break; }
            }

            printf("\n[ESCANEO DE TÍTULO]\n");
            printf("ID Unico del Titulo: "); scanf("%d", &id);
            clasif = 0; precio = 0; stock = 0; cantDesp = 0; demoraDesp = 0;
            nombre[0] = '\0'; tipoPub[0] = '\0';
            if (ubicarTitulo(&miTienda, id) == NULL) {
                printf("Nombre del titulo: "); scanf(" %99[^\n]", nombre);
                printf("Tipo de publicacion (Serie regular/Miniserie/Tomo recopilatorio/Novela grafica/Tira comica/Fanzine): "); scanf(" %49[^\n]", tipoPub);
                printf("Clasificacion de Edad (ej: 0, 14, 18): "); scanf("%d", &clasif);
                printf("Precio: "); scanf("%d", &precio);
                printf("Ejemplares Disponibles: "); scanf("%d", &stock);
                printf("Cant. por Despacho: "); scanf("%d", &cantDesp);
                printf("Demora Despacho (dias): "); scanf("%d", &demoraDesp);
            }

            res = procesarEscaneoMostrador(&miTienda, c->IDcliente, id2, id, nombre, tipoPub, clasif, precio, stock, cantDesp, demoraDesp, &nuevoTitulo);
            if (nuevoTitulo) printf("-> Título nuevo registrado con éxito.\n");
            if (res == 1) printf("-> Ejemplar entregado al cliente.\n");
            else if (res == 2) printf("-> Sin ejemplares: el cliente quedó en la lista de espera.\n");
            else if (res == -3) printf("-> Título descatalogado y sin ejemplares: no admite lista de espera.\n");
            else if (res == -4) printf("-> El cliente no cumple la edad mínima del título.\n");
            else if (res == -5) printf("-> El cliente no se encuentra en esa sala.\n");
            else if (res == -6) printf("-> Ese ID de título pertenece a otra sala.\n");
            else if (res == -7) printf("-> El cliente ya está en la lista de espera de este título.\n");
            else printf("-> Error en mostrador.\n");
            break;

        case 3:
            do {
                printf("\n--- GESTION DE CLIENTES ---\n");
                printf("1. Registrar Cliente\n2. Buscar Cliente\n3. Modificar Cliente\n4. Eliminar Cliente\n5. Listar Clientes\n0. Volver\nOpcion: ");
                scanf("%d", &subOpcion);
                if (subOpcion == 1) {
                    printf("Nombre: "); scanf(" %99[^\n]", nombre);
                    printf("Edad: "); scanf("%d", &edad);
                    res = crearCliente(&miTienda, nombre, edad);
                    if (res > 0) printf("-> Cliente creado con ID: %d\n", res);
                    else printf("-> No se pudo registrar el cliente (capacidad llena o edad invalida).\n");
                }
                else if (subOpcion == 2) {
                    printf("ID a buscar: "); scanf("%d", &id);
                    cli = buscarCliente(&miTienda, id);
                    if (cli) printf("-> Encontrado: %s (Edad: %d, Dentro: %s)\n", cli->nombre, cli->edad, cli->dentroTienda ? "Sí" : "No");
                    else printf("-> No encontrado.\n");
                }
                else if (subOpcion == 3) {
                    printf("ID a modificar: "); scanf("%d", &id);
                    printf("Nuevo nombre: "); scanf(" %99[^\n]", nombre);
                    printf("Nueva edad: "); scanf("%d", &edad);
                    if (modificarCliente(&miTienda, id, nombre, edad)) printf("-> Modificado con éxito.\n");
                    else printf("-> Error al modificar.\n");
                }
                else if (subOpcion == 4) {
                    printf("ID a eliminar: "); scanf("%d", &id);
                    res = eliminarCliente(&miTienda, id);
                    if (res == 1) printf("-> Eliminado.\n");
                    else if (res == -1) printf("-> No se puede eliminar, el cliente está dentro de la tienda.\n");
                    else printf("-> No encontrado.\n");
                }
                else if (subOpcion == 5) {
                    listarClientes(&miTienda);
                }
            } while (subOpcion != 0);
            break;

        case 4:
            printf("ID del cliente que abandona la tienda: "); scanf("%d", &id);
            res = salidaAutomaticaCliente(&miTienda, id);
            if (res == 1) printf("-> Salida registrada. Aforo actualizado.\n");
            else printf("-> El cliente no se encuentra dentro de la tienda.\n");
            break;

        case 5:
            do {
                printf("\n--- GESTION DE PASES ---\n");
                printf("1. Listar Pases y Uso Diario\n2. Anular Pase de Cliente\n0. Volver\nOpcion: ");
                scanf("%d", &subOpcion);
                if (subOpcion == 1) listarPasesTienda(&miTienda);
                else if (subOpcion == 2) {
                    printf("ID Cliente cuyo pase se anulará: "); scanf("%d", &id);
                    if (anularPaseCliente(&miTienda, id)) printf("-> Pase anulado con éxito.\n");
                    else printf("-> Error al anular pase.\n");
                }
            } while (subOpcion != 0);
            break;

        case 6:
            do {
                printf("\n--- GESTION DE TITULOS Y DESPACHOS ---\n");
                printf("1. Listar Títulos de una Sala\n2. Registrar Llegada de Despacho\n3. Descatalogar Título\n4. Eliminar Título\n0. Volver\nOpcion: ");
                scanf("%d", &subOpcion);
                if (subOpcion == 1) {
                    printf("ID de la Sala: "); scanf("%d", &id);
                    listarTitulosSala(&miTienda, id);
                }
                else if (subOpcion == 2) {
                    printf("ID del Título que recibe despacho: "); scanf("%d", &id);
                    printf("Cantidad de ejemplares que llegan: "); scanf("%d", &stock);
                    if (procesarLlegadaDespacho(&miTienda, id, stock)) printf("-> Despacho procesado y lista de espera atendida.\n");
                    else printf("-> Error al procesar despacho.\n");
                }
                else if (subOpcion == 3) {
                    printf("ID del Título a descatalogar: "); scanf("%d", &id);
                    if (descatalogarTitulo(&miTienda, id)) printf("-> Título descatalogado.\n");
                    else printf("-> Título no encontrado o ya descatalogado.\n");
                }
                else if (subOpcion == 4) {
                    printf("ID del Título a eliminar: "); scanf("%d", &id);
                    if (eliminarTitulo(&miTienda, id)) printf("-> Título eliminado del árbol.\n");
                    else printf("-> Título no encontrado.\n");
                }
            } while (subOpcion != 0);
            break;

        case 7:
            do {
                printf("\n--- REPORTES Y BITACORA ---\n");
                printf("1. Ver Reporte de Cierre Diario\n2. Estimar Tiempo de Espera para un Título\n3. Ver Bitácora\n0. Volver\nOpcion: ");
                scanf("%d", &subOpcion);
                if (subOpcion == 1) {
                    generarReporteCierre(&miTienda);
                }
                else if (subOpcion == 2) {
                    printf("ID del Título en espera: "); scanf("%d", &id);
                    res = estimarTiempoEsperaTitulo(&miTienda, id, &desp, &dias);
                    if (res == 2) printf("-> Despachos necesarios: %d | Tiempo estimado: %d días.\n", desp, dias);
                    else if (res == 1) printf("-> No hay clientes esperando por este título.\n");
                    else printf("-> Título no encontrado o sin datos de despacho.\n");
                }
                else if (subOpcion == 3) {
                    listarBitacora(&miTienda);
                }
            } while (subOpcion != 0);
            break;

        case 0:
            printf("Saliendo del sistema...\n");
            break;
        }
    } while (opcion != 0);

    return 0;
}
