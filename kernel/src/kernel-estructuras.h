// Biblioteca para guardar las estructuras y variables globales del kernel
#ifndef CONSOLA_ESTRUCTURAS_H_
#define CONSOLA_ESTRUCTURAS_H_

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
    char* PUERTO_ESCUCHA;
    char* ALGORITMO_PLANIFICACION;
    int ESTIMACION_INICIAL;
    int HRRN_ALFA;
    int GRADO_MAX_MULTIPROGRAMACION;
    char** RECURSOS;
    char** INSTANCIAS_RECURSOS;
};
typedef struct kernel_config t_kernel_config;

// Variables globales
extern t_log *kernelDebuggingLogger;

#endif
