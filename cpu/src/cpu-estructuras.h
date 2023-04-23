#ifndef CPU_ESTRUCTURAS_H
#define CPU_ESTRUCTURAS_H

//Bibliotecas commons
#include <commons/log.h>

//Estructuras
struct cpu_config
{
    uint32_t RETARDO_INSTRUCCION;
    char *IP_MEMORIA;
    char *IP_ESCUCHA;
    char *PUERTO_MEMORIA;
    char *PUERTO_ESCUCHA;
    uint32_t TAM_MAX_SEGMENTO;
    int SOCKET_MEMORIA;
    int SOCKET_KERNEL;
};
typedef struct cpu_config t_cpu_config;

extern t_log *cpuDebuggingLogger; 
extern t_log *cpuLogger;
extern t_cpu_config *cpuConfig;
#endif