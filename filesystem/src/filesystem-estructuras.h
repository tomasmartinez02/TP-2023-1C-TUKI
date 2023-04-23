// Biblioteca para guardar las estructuras y variables globales del filesystem
#ifndef FILESYSTEM_ESTRUCTURAS_H_
#define FILESYSTEM_ESTRUCTURAS_H_

// Bibliotecas commons
#include <commons/log.h>
#include <stdint.h>

// Estructuras
struct filesystem_config 
{
    char *IP_MEMORIA;
    char *PUERTO_MEMORIA;
    char *IP_ESCUCHA;
    char *PUERTO_ESCUCHA;
    char *PATH_SUPERBLOQUE;
    char *PATH_BITMAP;
    char *PATH_BLOQUES;
    char *PATH_FCB;
    uint32_t RETARDO_ACCESO_BLOQUE;
    int SOCKET_MEMORIA;
    int SOCKET_KERNEL;
};
typedef struct filesystem_config t_filesystem_config;

// Variables globales
extern t_log *filesystemLogger;
extern t_log *filesystemDebuggingLogger;
extern t_filesystem_config *filesystemConfig;

#endif