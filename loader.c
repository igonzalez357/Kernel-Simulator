#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "loader.h"
#include "physicalMemory.h"
#include "estructuras.h"


// Función que carga un programa desde un archivo
void load_program_from_file(char *filename, PCB *pcb) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    unsigned long code_start, data_start;
    
    // Leer y descartar la etiqueta .text, luego leer la dirección
    fscanf(file, "%*s %lx\n", &code_start);

    // Leer y descartar la etiqueta .data, luego leer la dirección
    fscanf(file, "%*s %lx\n", &data_start);

    // Calcular el número de páginas necesarias para el programa
    int paginasTotales = (code_start + data_start) / 256;

    // Calcular marcos libres
    int *marcosLibres = malloc(paginasTotales * sizeof(int));

    buscarMarcos(marcosLibres, paginasTotales); // Buscar marcos libres

    // Asignar las direcciones virtuales de los segmentos al PCB
    pcb->mm.code = (uintptr_t *)code_start;
    pcb->mm.data = (uintptr_t *)data_start;
    pcb->mm.pgb = physicalMemory.siguienteTabla;
    nuevaTabla(paginasTotales, pcb->mm->marcosLibres);

    // Leer y cargar el código (.text) en la memoria física
    unsigned int code_line;
    int i = 0;
    while (fscanf(file, "%x\n", &code_line) != EOF) {
        // Cargar la instrucción en la memoria física
        if (i < 256) {
            physicalMemory[code_start + i] = code_line;
            i++;
        } else {
            break;
        }
    }

    // Leer y cargar los datos (.data) en la memoria física
    unsigned int data_line;
    i = 0;
    while (fscanf(file, "%x\n", &data_line) != EOF) {
        if (i < 256) {
            physical_memory.mem[data_start + i] = data_line;  // Cargar datos en la memoria
            i++;
        } else {
            break;
        }
    }

    fclose(file);
    printf("Programa cargado desde el archivo: %s\n", filename);
}

// Función para crear el PCB y cargar el programa
PCB* create_process(char *filename, int PID) {
    PCB *new_pcb = (PCB *)malloc(sizeof(PCB));
    if (new_pcb == NULL) {
        perror("Error al crear PCB");
        return NULL;
    }

    // Crear e inicializar la estructura MM (memoria del proceso)
    new_pcb->mm = (MM *)malloc(sizeof(MM));
    if (new_pcb->mm == NULL) {
        perror("Error al crear MM");
        free(new_pcb);
        return NULL;
    }

    // Inicializar el PCB
    new_pcb->pid = PID;  // Asignar un pid aleatorio (o puedes generar uno secuencialmente)

    // Cargar el programa en la memoria
    load_program_from_file(filename, new_pcb);

    return new_pcb;
}

// Función que inicializa el generador de procesos
void* loader_thread() {

    char filename[11];
    int numFile = 0;
    int PID = 0;
    while (1) {

        // Agregar el proceso a la cola
        pthread_mutex_lock(&process_queue_mutex);
        if (queue.numProcesses < MAXPROCESSES) {

            sprintf(filename, "procesos/prog%03d.elf", numFile);
            PCB *new_pcb = create_process(filename, PID);
            queue.processes[queue.numProcesses] = new_pcb;
            queue.numProcesses++;
            numFile++;
            PID++;

        } else {
            printf("Loader: Cola de procesos llena");
        }
        pthread_mutex_unlock(&process_queue_mutex);

    }
    return NULL;
}