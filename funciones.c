#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"


char* copiarTexto(char* origen)
{
    char* copia;

    copia = (char*)malloc(strlen(origen) + 1);
    if (copia != NULL) {
        strcpy(copia, origen);
    }
    return copia;
}

/* Devuelve 1 si el cliente termina con un pase activo (ya lo tenia o lo compro)
 * y 0 si decide no comprar. */
int validarComprarPase(struct Tienda* tienda, struct Cliente* cliente)
{
    int paseValido = 0;
    int opcion = 0;
    int tipoOpcion = 0;
    char tipoPaseStr[20];
    int valorPase;
    char paseActivo[] = "activo";

    if (cliente->pase.estado != NULL && strcmp(cliente->pase.estado, paseActivo) == 0
        && cliente->pase.IDpase > 0) {
        paseValido = 1;
    }

    if (paseValido) {
        return 1;
    }

    do {
        printf("====================\n");
        printf("El cliente %s no posee un pase valido \n", cliente->nombre);
        printf("====================\n");

        printf("Que desea hacer?\n");
        printf("1.- Comprar un pase para la tienda?\n");
        printf("2.- No comprar un pase y retirarse de la tienda\n");
        printf("Elija una opcion (1-2): ");
        scanf("%d", &opcion);
    } while (opcion != 1 && opcion != 2);

    if (opcion == 2) {
        printf("El cliente ha decidido salir de la tienda\n");
        return 0;
    }

    do {
        printf("\n========== SELECCIONE EL TIPO DE PASE ========== \n");
        printf("1. General\n");
        printf("2. Infantil\n");
        printf("3. Familiar\n");
        printf("4. Prioritario (acceso rápido a firmas de autores)\n");
        printf("Elija una opción (1-4): ");
        scanf("%d", &tipoOpcion);

        if (tipoOpcion < 1 || tipoOpcion > 4) {
            printf("Opción no válida (debe ser un número entre 1 y 4). Intente nuevamente.\n");
        }
    } while (tipoOpcion < 1 || tipoOpcion > 4);

    switch (tipoOpcion) {
    case 2:
        strcpy(tipoPaseStr, "infantil");
        valorPase = 3000;
        break;
    case 3:
        strcpy(tipoPaseStr, "familiar");
        valorPase = 16000;
        break;
    case 4:
        strcpy(tipoPaseStr, "prioritario");
        valorPase = 10000;
        break;
    case 1:
    default:
        strcpy(tipoPaseStr, "general");
        valorPase = 5000;
        break;
    }

    cliente->pase.IDpase = tienda->siguienteIdPase;
    tienda->siguienteIdPase++;
    cliente->pase.tipo = copiarTexto(tipoPaseStr);
    cliente->pase.valor = valorPase;
    cliente->pase.estado = copiarTexto(paseActivo);

    tienda->pasesVendidos++;
    tienda->gananciaPases += valorPase;
    tienda->gananciaHoy += valorPase;

    printf("¡Compra exitosa! Se le ha asignado el Pase ID: %d | Tipo: %s | Valor: $%d.\n",
        cliente->pase.IDpase, cliente->pase.tipo, cliente->pase.valor);

    return 1;
}


