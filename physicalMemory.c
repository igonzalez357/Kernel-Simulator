#include <stdio.h>
#include <stdlib.h>
#include "physicalMemory.h"
//#include "estructuras.h"

// Memoria física de 16MB * 4B = 64MB
// Memoria para el kernel = 4MB * 4B = 16MB (0x000000 - 0x3FFFFF)
// Función para inicializar la memoria física (ponemos todo a cero)
void init_physical_memory() {
    for (int i = 0; i < sizeof(physicalMemory); i++) {
        physicalMemory[i] = 0;  // Inicializamos todos los valores a 0
    }
    printf("Memoria física inicializada.\n");
}