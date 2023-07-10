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
#include <commons/collections/dictionary.h>

//Static-utils libraries
#include <serializacion/buffer.h>
#include <utils/instrucciones.h>
#include <utils/tablas-pcb.h>

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
    t_dictionary *DICCIONARIO_SEMAFOROS_RECURSOS;
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
    RECURSO
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
    t_dictionary *archivosAbiertos;
    uint32_t tamanioTablaSegmentos; 
    t_info_segmentos **tablaSegmentos;
    t_nombre_estado estadoActual;   
    t_nombre_estado estadoDeFinalizacion;
    t_nombre_estado estadoAnterior;
    bool procesoBloqueadoOTerminado;
    uint32_t socketProceso;
    pthread_mutex_t *mutex;
    char* dispositivoIoEnUso;
};
typedef struct pcb t_pcb;

struct semaforo_recurso
{
    int32_t instancias;
    t_estado *estadoRecurso;
};
typedef struct semaforo_recurso t_semaforo_recurso;

// Estructura de la tabla de segmentos general
typedef struct lista_tablas lista_tablas;
struct lista_tablas
{
    uint32_t pidProceso;
    t_info_segmentos** tablaSegmentos;
    lista_tablas* siguiente; 
};


// Variables globales
extern t_log *kernelDebuggingLogger;
extern t_log *kernelLogger;
extern t_kernel_config *kernelConfig;
extern t_dictionary *tablaArchivosAbiertos;
extern sem_t dispatchPermitido;
extern pthread_mutex_t mutexSocketMemoria;
extern pthread_mutex_t mutexSocketFilesystem;
extern sem_t semFRead;
extern sem_t semFWrite;
extern bool fRead;
extern bool fWrite;

// Estados
extern t_estado *estadoNew;
extern t_estado *estadoReady;
extern t_estado *estadoExecute;
extern t_estado *estadoBlocked; 
extern t_estado *estadoExit;

#endif
