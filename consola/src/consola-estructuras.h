// Biblioteca para guardar las estructuras y variables globales de la consola
#ifndef CONSOLA_ESTRUCTURAS_H_
#define CONSOLA_ESTRUCTURAS_H_

//Bibliotecas estandar
#include <stdint.h>
// Bibliotecas commons
#include <commons/log.h>

// Estructuras
struct consola_config 
{
    char* IP_KERNEL;
    char* PUERTO_KERNEL;
    int SOCKET_KERNEL;
    uint32_t PID;
};
typedef struct consola_config t_consola_config;

// Variables globales
extern t_log *consolaDebuggingLogger;
extern t_log *consolaLogger;
extern t_consola_config *consolaConfig;

#endif