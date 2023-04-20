// Biblioteca para guardar las estructuras y variables globales del kernel
#ifndef KERNEL_ESTRUCTURAS_H_
#define KERNEL_ESTRUCTURAS_H_

// Bibliotecas commons
#include <commons/log.h>
#include <commons/collections/list.h>

//Static-utils libraries
#include <serializacion/buffer.h>

// Estructuras
struct kernel_config 
{
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* IP_FILESYSTEM;
    char* PUERTO_FILESYSTEM;
    char* IP_CPU;
    char* PUERTO_CPU;
    char *IP_ESCUCHA;
    char* PUERTO_ESCUCHA;
    char* ALGORITMO_PLANIFICACION;
    uint32_t ESTIMACION_INICIAL;
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

typedef enum 
{   NEW,
    READY,
    EXEC,
    EXIT,
    BLOCKED,
} t_nombre_estado;

struct pcb
{
    uint32_t pid;
    t_buffer* instrucciones;
    uint32_t programCounter;
    //t_registros_cpu* registrosCpu;
    uint32_t estimadoProxRafaga;
    uint32_t tiempoLlegadaReady;
    //archivosAbiertos (no sabemos qué tipo de dato debería ser)
    //uint32_t* tablaSegmentos [no sabemos si deberia ser un int]
    t_nombre_estado estadoActual; 
    t_nombre_estado estadoDeFinalizacion;
    t_nombre_estado estadoAnterior;
    bool procesoBloqueadoOTerminado;
    uint32_t socketProceso;
    pthread_mutex_t* mutex;
    /*uint32_t* arrayTablaPaginas;
    char* dispositivoIoEnUso;
    int32_t cantidadUnidadesTiemposIo;
    t_registro registroUsadoEnIo;
    */
};
typedef struct pcb t_pcb;

typedef struct
{
    t_nombre_estado nombreEstado;
    t_list* listaProcesos;
    pthread_mutex_t* mutexEstado;
} t_estado;

// Variables globales
extern t_log *kernelDebuggingLogger;
extern t_log *kernelLogger;
extern t_kernel_config *kernelConfig;

#endif
