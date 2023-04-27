// Biblioteca para guardar las estructuras y variables globales del kernel
#ifndef KERNEL_ESTRUCTURAS_H_
#define KERNEL_ESTRUCTURAS_H_

// Bibliotecas estandar
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
// Bibliotecas commons
#include <commons/log.h>
#include <commons/collections/list.h>

//Static-utils libraries
#include <serializacion/buffer.h>
#include <utils/instrucciones.h>

// Estructuras
struct kernel_config 
{
    char *IP_MEMORIA;
    char *PUERTO_MEMORIA;
    char *IP_FILESYSTEM;
    char *PUERTO_FILESYSTEM;
    char *IP_CPU;
    char *PUERTO_CPU;
    char *IP_ESCUCHA;
    char *PUERTO_ESCUCHA;
    char *ALGORITMO_PLANIFICACION;
    double ESTIMACION_INICIAL;
    double HRRN_ALFA;
    uint32_t GRADO_MAX_MULTIPROGRAMACION;
    char **RECURSOS;
    char **INSTANCIAS_RECURSOS;
    int SOCKET_MEMORIA;
    int SOCKET_CPU;
    int SOCKET_FILESYSTEM;
    int SOCKET_KERNEL;
};
typedef struct kernel_config t_kernel_config;

enum nombre_estado
{   
    NEW,
    READY,
    EXEC,
    EXIT,
    BLOCKED,
};
typedef enum nombre_estado t_nombre_estado;

struct estado
{
    t_nombre_estado nombreEstado;
    t_list *listaProcesos;
    sem_t *semaforoEstado;
    pthread_mutex_t *mutexEstado;
};
typedef struct estado t_estado;

struct info_segmentos 
{
    uint32_t id;
    uint32_t direccionBase;
    uint32_t tamanio;
};
typedef struct info_segmentos t_info_segmentos;

struct info_archivos
{
    char *nombreArchivo;
    uint32_t posicionPuntero;
};
typedef struct info_archivos t_info_archivos;

// Estructura para guardar el timestamp de llegada a ready
typedef struct timespec timestamp;

struct pcb
{
    uint32_t pid;
    t_buffer* instrucciones;
    uint32_t programCounter;
    t_registros_cpu* registrosCpu;
    double estimadoProxRafaga;
    timestamp *tiempoLlegadaReady;
    t_list *archivosAbiertos; 
    t_info_segmentos *tablaSegmentos;
    t_nombre_estado estadoActual; 
    t_nombre_estado estadoDeFinalizacion;
    t_nombre_estado estadoAnterior;
    bool procesoBloqueadoOTerminado;
    uint32_t socketProceso;
    pthread_mutex_t *mutex;

    /*
    char* dispositivoIoEnUso;
    int32_t cantidadUnidadesTiemposIo;
    t_registro registroUsadoEnIo;
    */
};
typedef struct pcb t_pcb;

// Variables globales
extern t_log *kernelDebuggingLogger;
extern t_log *kernelLogger;
extern t_kernel_config *kernelConfig;

#endif
