#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "loader.h"
#include "estructuras.h"


// Función que carga un programa desde un archivo
void load_program_from_file(char *filename, PCB *pcb) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    unsigned int code_start, data_start;
    fscanf(file, "%x %x\n", &code_start, &data_start);  // Leer las direcciones de inicio de código y datos

    // Asignar las direcciones virtuales de los segmentos al PCB
    pcb->mm->code = (unsigned int *)code_start;
    pcb->mm->data = (unsigned int *)data_start;

    // Crear la tabla de páginas (Page Table) y asignarla al PCB
    pcb->mm->pgb = (unsigned int *)malloc(sizeof(PageTable));
    if (pcb->mm->pgb == NULL) {
        perror("Error al asignar memoria para la tabla de páginas");
        fclose(file);
        return;
    }
    memset(pcb->mm->pgb, 0, sizeof(PageTable));  // Inicializar la tabla de páginas a 0

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
PCB* create_process(char *filename) {
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
    new_pcb->pid = rand();  // Asignar un pid aleatorio (o puedes generar uno secuencialmente)

    // Cargar el programa en la memoria
    load_program_from_file(filename, new_pcb);

    return new_pcb;
}

// Función que inicializa el generador de procesos
void* loader_thread() {

    char filename[11];
    int numFile = 0;
    while (1) {
        
        sprintf(filename, "prog%03d.elf", numFile);
        PCB *new_pcb = create_process(filename);

        // Agregar el proceso a la cola
        pthread_mutex_lock(&process_queue_mutex);
        if (queue.numProcesses < MAXPROCESSES) {
            queue.processes[queue.numProcesses] = new_pcb;
            queue.numProcesses++;
        } else {
            printf("Loader: Cola de procesos llena");
            free(new_pcb);
        }
        pthread_mutex_unlock(&process_queue_mutex);

        numFile++;

    }
    return NULL;
}