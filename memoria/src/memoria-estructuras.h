// Biblioteca para guardar las estructuras y variables globales del kernel
#ifndef MEMORIA_ESTRUCTURAS_H_
#define MEMORIA_ESTRUCTURAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Bibliotecas commons
#include <commons/log.h>
#include <utils/tablas-pcb.h>
#include <commons/bitarray.h>

// Estructuras
struct memoria_config 
{
    char *IP_ESCUCHA;
    char *PUERTO_ESCUCHA;
    u_int32_t TAM_MEMORIA;
    u_int32_t TAM_SEGMENTO_0;
    u_int32_t CANT_SEGMENTOS;
    u_int32_t RETARDO_MEMORIA;
    u_int32_t RETARDO_COMPACTACION;
    char *ALGORITMO_ASIGNACION;
    int SOCKET_CPU;
    int SOCKET_KERNEL;
    int SOCKET_FILESYSTEM;
};
typedef struct memoria_config t_memoria_config;

typedef struct t_huecos_libres t_huecos_libres; 
struct t_huecos_libres
{
    t_info_segmentos* hueco;
    t_huecos_libres* siguiente;
};

typedef struct lista_tablas lista_tablas;
struct lista_tablas
{
    uint32_t pidProceso;
    t_info_segmentos** tablaSegmentos;
    lista_tablas* siguiente; 
};

typedef struct parametrosHilo parametrosHilo;
struct parametrosHilo
{
    int socketModulo;
    char* nombreModulo;
};

enum algoritmo
{   
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};
typedef enum algoritmo t_algoritmo;

typedef struct lista_para_compactar lista_para_compactar;
struct lista_para_compactar
{
    uint32_t pid;
    t_info_segmentos* segmento;
    lista_para_compactar* siguiente; 
};

// Variables globales
extern t_log *memoriaDebuggingLogger;
extern t_log *memoriaLogger;
extern t_memoria_config *memoriaConfig;
extern void *memoriaPrincipal;
extern t_info_segmentos *segmentoCero;
extern t_huecos_libres *listaHuecosLibres; 
extern lista_tablas *tablasDeSegmentos;
extern pthread_mutex_t mutexSocketKernel;

#endif