#include <pthread.h>

// Máximo número de procesos permitidos en la cola
#define MAXPROCESSES 50

// ESTRUCTURAS PARA LA GESTIÓN DE PROCESOS

struct MM {
    unsigned int *code;     // Puntero a la dirección virtual del segmento de código (.text)
    unsigned int *data;     // Puntero a la dirección virtual del segmento de datos (.data)
    unsigned int pgb;       // Dirección física a la tabla de páginas
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

// ------------------------------

// ESTRUCTURAS PARA LA GESTIÓN DE LOS NÚCLEOS DE LA MÁQUINA

// Estructura para la MMU
typedef struct MMU{                                                                                                    
    int *TLB;           // Puntero al TLB (Translation Lookaside Buffer)                                            
} MMU;  

// Estructura para los registros de cada hilo de CPU
typedef struct HWThread {
    unsigned int PC;       // Contador de programa (program counter)
    unsigned int RI;       // Registro de instrucción (Instruction Register)
    unsigned int *PTBR;    // Puntero a la tabla de páginas (Page Table Base Register)
    MMU mmu;              // Puntero a la MMU    
} HWThread;

// Estructura para los núcleos de la máquina
typedef struct Core {
    int idCore;             // Identificador del núcleo
    int numThreads;     // Número de hilos de CPU
    HWThread *threads;  // Array de hilos de CPU
} Core;

// Estructura para describir la configuración de la máquina
typedef struct Machine {
    int numCores; // Número de núcleos
    Core *cores;  // Array de núcleos
} Machine;

extern Machine machine;         // Variable global para la configuración de la máquina
extern int numHilosTotales;     // Número total de hilos en la máquina
extern int numHilosDisponibles; // Número de hilos disponibles en la máquina

// ------------------------------

// ESTRUCTURAS PARA LA GESTIÓN DE MEMORIA VIRTUAL

#define MEM_SIZE 16777216       // 2^24 direcciones de memoria
#define WORD_SIZE 4             // 4 bytes por palabra
#define NUMMARCOS 15360         // 60MB para marcos de 4KB
#define DIRKERNEL 4194304       // Tablas de paginas desde 0x000000 hasta 0x3FFFFF
#define TAMPAGINA 4096          // Paginas de 4KB
#define TAMPALABRA 4            // Una palabra = 4 bytes (8 caracteres hexadecimales)


typedef struct Pagina{
    int numPagina;  // Número de página
    int numMarco;   // Número de marco en la memoria fisica
    int valida;     // 0: no válido, 1: válido
} Pagina;

typedef struct Marco{
    int numMarco;   // Número de marco
    int ocupado;    // 0: libre, 1: ocupado
    int pid;        // PID del proceso que ocupa este marco
} Marco;

typedef struct TablaPaginas {
    int numPaginas;     // Número de páginas de la memoria virtual
    Pagina *paginas;    // Array de páginas del proceso
} TablaPaginas;

typedef struct PhysicalMemory {
    unsigned char mem[MEM_SIZE * WORD_SIZE];    // Memoria física de 16MB * 4B = 64MB
    int nextMarco;                              // Siguiente marco disponible
    Marco marcos[NUMMARCOS];                    // Array de marcos
    int siguienteTabla;                         // Dirección de la siguiente tabla
} PhysicalMemory;

extern PhysicalMemory physicalMemory;

// ------------------------------

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


