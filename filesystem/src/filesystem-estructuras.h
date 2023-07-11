// Biblioteca para guardar las estructuras y variables globales del filesystem
#ifndef FILESYSTEM_ESTRUCTURAS_H_
#define FILESYSTEM_ESTRUCTURAS_H_

// Bibliotecas commons
#include <commons/log.h>
#include <commons/bitarray.h>
#include <commons/collections/dictionary.h>
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

struct superbloque
{
    uint32_t BLOCK_SIZE;
    uint32_t BLOCK_COUNT;
};
typedef struct superbloque t_superbloque;

struct bitmap
{
    char *direccion;
    uint32_t tamanio;
    t_bitarray *bitarray;
};
typedef struct bitmap t_bitmap;

struct fcb
{
    char *NOMBRE_ARCHIVO;
    uint32_t TAMANIO_ARCHIVO;
    uint32_t PUNTERO_DIRECTO;
    uint32_t PUNTERO_INDIRECTO;
    uint32_t cantidad_bloques_asignados;
};
typedef struct fcb t_fcb;

// Variables globales
extern t_log *filesystemLogger;
extern t_log *filesystemDebuggingLogger;
extern t_filesystem_config *filesystemConfig;
extern t_superbloque *superbloque;
extern t_bitmap *bitmap;
extern t_fcb *fcb;
extern t_dictionary *listaFcbs;
extern FILE* archivoDeBloques;
extern uint32_t tiempoRetardo;
extern uint32_t tamanioBloques;
extern char *directorioFcbs;
extern char *pathArchivoDeBloques;

#endif