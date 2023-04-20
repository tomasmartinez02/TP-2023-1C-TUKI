#ifndef KERNEL_ESTADOS_H
#define KERNEL_ESTADOS_H

//Bibliotecas estandares
#include <pthread.h>
#include <semaphore.h>

//Bibliotecas internas modulo kernel
#include <kernel-estructuras.h>
#include <kernel-utils.h>
#include <kernel-pcb.h>

//Bibliotecas Static Utils
#include <serializacion/buffer.h>
#include <serializacion/stream.h>

//Estados
static t_estado* estadoNew;
static t_estado* estadoReady;
static t_estado* estadoExecute;
static t_estado* estadoExit;
static t_estado* estadoBlocked; 

/**
 * @brief Crea el estado 
 *
 * @param nombre: Nombre del estado a crear
 * 
 * @return t_estado*: Estructura del estado
 * 
 * @example t_estado* new = crear_estado(NEW);
 */
t_estado* crear_estado(t_nombre_estado nombre);

/**
 * @brief Destruye la estructura en memoria del estado 
 * 
 * @param estado: Estado a destruir
 * 
 * @example destruir_estado(new); 
 */
void destruir_estado(t_estado* estado); 

/**
 * @brief Se utiliza para crear todos los estados por los cuales puede pasar un proceso (new, ready, execute, exit, blocked) 
 * 
 * @example inicializar_estructuras_estados();
 */
void inicializar_estructuras_estados(void);

/**
 * @brief Agrega a la cola del estado destino el pcb recibido, teniendo en cuenta que ningún otro proceso lo esté haciendo al mismo tiempo
 * 
 * @param estadoDestino: Estructura del estado al que se quiere encolar el pcb
 * @param pcbAAgregar: Pcb a encolar
 * 
 * @example estado_encolar_pcb(estadoNew,nuevoPcb);
 */
void estado_encolar_pcb(t_estado* estadoDestino, t_pcb* pcbAAgregar);

/**
 * @brief Devuelve la lista de procesos del estado
 * 
 * @param estado: Estructura del estado del que se quiere obtener la lista de procesos
 * 
 * @return t_list*: Lista de procesos
 *
 * @example estado_get_list(estadoNew);
 */
t_list* estado_get_list(t_estado* estado);

/**
 * @brief Devuelve el mutex del estado
 * 
 * @param estado: Estructura del estado del que se quiere obtener el mutex
 * 
 * @return pthread_mutex_t*: Mutex
 *
 * @example estado_get_mutex(estadoNew);
 */
pthread_mutex_t* estado_get_mutex(t_estado* estado);

#endif