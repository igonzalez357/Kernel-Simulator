#include <stdio.h>
#include <stdlib.h>
#include "physicalMemory.h"

#define MEM_SIZE 16777216  // 16MB = 2^24 bytes
#define WORD_SIZE 4       // 4 bytes por palabra

unsigned char physicalMemory[MEM_SIZE];  // Memoria física de 16MB

// Función para inicializar la memoria física (ponemos todo a cero)
void init_physical_memory() {
    for (int i = 0; i < MEM_SIZE; i++) {
        physicalMemory[i] = 0;  // Inicializamos todos los valores a 0
    }
    printf("Memoria física inicializada.\n");
}
