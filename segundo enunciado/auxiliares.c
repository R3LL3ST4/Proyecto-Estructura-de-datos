/* auxiliares */

#include "funciones.h"

/* funcion auxiliar para contar el largo de un texto sin usar strlen */
int largoTexto(char *texto) {
    int largo = 0;

    while (texto[largo] != '\0') {
        largo++;
    }

    return largo;
}

/* funcion auxiliar para copiar texto a memoria dinamica */
char *copiarTexto(char *texto) {
    char *copia;

    copia = (char *) malloc(largoTexto(texto) + 1);
    strcpy(copia, texto);

    return copia;
}
