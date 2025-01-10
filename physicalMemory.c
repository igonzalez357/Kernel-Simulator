#include <stdio.h>
#include <stdlib.h>
#include "physicalMemory.h"
#include "estructuras.h"


// Memoria física de 16MB * 4B = 64MB
// Memoria para el kernel = 4MB * 4B = 16MB (0x000000 - 0x3FFFFF)
// Función para inicializar la memoria física (ponemos todo a cero)
void init_physical_memory() {
    for (int i = 0; i < sizeof(physicalMemory); i++) {
        physicalMemory[i] = 0;  // Inicializamos todos los valores a 0
    }
    printf("Memoria física inicializada.\n");
}


void nuevaTabla (int paginasTotales, int *marcosLibres) {                                          
    physicalMemory.mem[pyhsicalMemory.siguientetTabla] = paginasTotales;                           
    for (int i = 0; i < paginasTotales; i++) {                                                     
        physicalMemory.siguienteTabla = (physicalMemory.siguienteTabla + 1) % DIRKERNEL;          
        physicalMemory.mem[physicalMemory.siguienteTabla] = i;                                    
        physicalMemory.siguienteTabla = (physicalMemory.siguienteTabla + 1) % DIRKERNEL;          
        physicalMemory.mem[physicalMemory.siguienteTabla] = marcosLibres[i];
        physicalMemory.siguienteTabla = (physicalMemory.siguienteTabla + 1) % DIRKERNEL;
        physicalMemory.mem[physicalMemory.siguienteTabla] = 1;
        printf("Página %d asignada al marco %d\n", i, marcosLibres[i]);

    }
    physicalMemory.siguienteTabla += (physicalMemory.siguienteTabla + 1) % DIRKERNEL;
}

int buscarMarcos(int *marcosLibres, int paginasTotales) {
    int nextMarco = pyhsicalMemory.nextMarco;
    int encontradas = 0;
    int i = 0;
    while (i < NUMMARCOS && encontradas < paginasTotales) {
        if (!physicalMemory.marcos[nextMarco].ocupado) {
            marcosLibres[encontradas] = physicalMemory.marcos[nextMarco].numMarco;
            encontradas++;
        }
        nextMarco = (nextMarco + 1) % NUMMARCOS;
    }

    if (encontradas != paginasTotales) {
        printf("No hay suficientes marcos libres\n");
        return -1;
    }
    physicalMemory.nextMarco = nextMarco;
    return 0;
}
