// Biblioteca para guardar las estructuras y variables globales del kernel
#ifndef MEMORIA_ESTRUCTURAS_H_
#define MEMORIA_ESTRUCTURAS_H_

// Bibliotecas commons
#include <commons/log.h>

// Estructuras
struct memoria_config 
{
    char* IP_ESCUCHA;
    char* PUERTO_ESCUCHA;
    u_int32_t TAM_MEMORIA;
    u_int32_t TAM_SEGMENTO_0;
    u_int32_t CANT_SEGMENTOS;
    u_int32_t RETARDO_MEMORIA;
    u_int32_t RETARDO_COMPACTACION;
    char* ALGORITMO_ASIGNACION;
};
typedef struct memoria_config t_memoria_config;

// Variables globales
extern t_log *memoriaDebuggingLogger;
extern t_log *memoriaLogger;
extern t_memoria_config *memoriaConfig;

#endif