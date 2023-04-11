#ifndef CPU_ESTRUCTURAS_H
#define CPU_ESTRUCTURAS_H

//Bibliotecas commons
#include <commons/log.h>

//Estructuras
struct cpu_config
{
    int32_t RETARDO_INSTRUCCION;
    char* IP_MEMORIA;
    int32_t PUERTO_MEMORIA;
    int32_t PUERTO_ESCUCHA;
    int32_t TAM_MAX_SEGMENTO;
};
typedef struct cpu_config t_cpu_config;
#endif