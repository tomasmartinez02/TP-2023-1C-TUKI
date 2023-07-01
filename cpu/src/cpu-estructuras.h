#ifndef CPU_ESTRUCTURAS_H_
#define CPU_ESTRUCTURAS_H_

//Bibliotecas commons
#include <commons/log.h>
// Bibliotecas static utils
#include <utils/instrucciones.h>
#include <utils/tablas-pcb.h>
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
struct cpu_pcb
{
    uint32_t pid;
    uint32_t programCounter;
    t_list *instrucciones;
    t_registros_cpu *registrosCpu;
    t_info_segmentos **tablaSegmentos;
    uint32_t tamanioTablaSegmentos;
};
typedef struct cpu_pcb t_cpu_pcb;

extern t_log *cpuDebuggingLogger; 
extern t_log *cpuLogger;
extern t_cpu_config *cpuConfig;
#endif