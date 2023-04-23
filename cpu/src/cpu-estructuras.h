#ifndef CPU_ESTRUCTURAS_H
#define CPU_ESTRUCTURAS_H

//Bibliotecas commons
#include <commons/log.h>
#include <utils/instrucciones.h>

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

// Aca coincidimos los datos que manda el buffer de kernel-adapter
typedef struct  {
    uint32_t pid;
    uint32_t programCounter;
    t_list* instrucciones;
    t_registros_cpu* registrosCpu;
} t_cpu_pcb;

extern t_log *cpuDebuggingLogger; 
extern t_log *cpuLogger;
extern t_cpu_config *cpuConfig;
#endif