// Biblioteca para guardar las estructuras y variables globales del kernel
#ifndef KERNEL_ESTRUCTURAS_H_
#define KERNEL_ESTRUCTURAS_H_

// Bibliotecas commons
#include <commons/log.h>

// Estructuras
struct kernel_config 
{
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_FILESYSTEM;
    char* PUERTO_FILESYSTEM;
    char* IP_CPU;
    char* PUERTO_CPU;
    char *IP_ESCUCHA;
    char* PUERTO_ESCUCHA;
    char* ALGORITMO_PLANIFICACION;
    uint32_t ESTIMACION_INICIAL;
    double HRRN_ALFA;
    uint32_t GRADO_MAX_MULTIPROGRAMACION;
    char **RECURSOS;
    char **INSTANCIAS_RECURSOS;
    int SOCKET_MEMORIA;
    int SOCKET_CPU;
    int SOCKET_FILESYSTEM;
    int SOCKET_KERNEL;
};
typedef struct kernel_config t_kernel_config;

typedef struct 
{

} t_pcb;

typedef enum 
{
    NEW,
    READY,
    EXEC,
    EXIT,
    BLOCKED,
} t_nombre_estado;

typedef struct
{
    t_nombre_estado nombreEstado;
    t_list* listaProcesos;
} t_estado;

// Variables globales
extern t_log *kernelDebuggingLogger;
extern t_log *kernelLogger;
extern t_kernel_config *kernelConfig;

#endif
