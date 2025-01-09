#include <pthread.h>

// Máximo número de procesos permitidos en la cola
#define MAXPROCESSES 20


struct MM {
    unsigned int *code;  // Puntero a la dirección virtual del segmento de código (.text)
    unsigned int *data;  // Puntero a la dirección virtual del segmento de datos (.data)
    unsigned int *pgb;   // Puntero a la tabla de páginas
};

// Estructura de control de procesos (PCB)
typedef struct PCB {
    int pid;            // Identificador único del proceso
    struct MM mm;
} PCB; 

// Cola de procesos
typedef struct ProcessQueue {
    PCB* processes[MAXPROCESSES]; // Array de punteros a procesos
    int numProcesses;             // Número de procesos actualmente en la cola
} ProcessQueue;

// Estructura para describir la configuración de la máquina
typedef struct Machine {
    int numHilos; // Número de hilos
    int numCores; // Número de núcleos
} Machine;

// Estructura para los registros de cada hilo de CPU
typedef struct CPU_thread {
    unsigned int PC;       // Contador de programa (program counter)
    unsigned int RI;       // Registro de instrucción (Instruction Register)
    unsigned int TLB[16];  // Tabla de traducción (TLB)
    unsigned int PTBR;     // Puntero a la tabla de páginas (Page Table Base Register)
    unsigned int MMU[256]; // Simulación de la MMU
} CPU_thread;

// Declaraciones para la sincronización entre hilos
extern pthread_mutex_t timer_mutex;         // Mutex para el Timer
extern pthread_cond_t timer_cond;           // Condicional para el Timer
extern pthread_mutex_t process_queue_mutex; // Mutex para la cola de procesos
extern pthread_mutex_t scheduler_mutex;     // Mutex para el Scheduler
extern pthread_cond_t scheduler_cond;       // Condicional para el Scheduler

// Cola de procesos
extern ProcessQueue queue;

// Variables globales para la configuración de la máquina
extern int intervalTimer, politicaScheduler;

// Variable global de memoria física
extern unsigned char physicalMemory[16777216];  // Memoria física de 16MB
