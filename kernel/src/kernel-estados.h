#ifndef KERNEL_ESTADOS_H_
#define KERNEL_ESTADOS_H_

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
#include <utils/common-utils.h>

/**
 * @brief Crea el estado 
 *
 * @param nombreEstado: Nombre del estado a crear
 * 
 * @return t_estado*: Estructura del estado
 * 
 * @example t_estado* new = crear_estado(NEW);
 */
t_estado *crear_estado(t_nombre_estado nombreEstado);

/**
 * @brief Destruye la estructura en memoria del estado 
 * 
 * @param estado: Estado a destruir
 * 
 * @example destruir_estado(new); 
 */
void destruir_estado(t_estado *self); 

/**
 * @brief Agrega a la cola del estado destino el pcb recibido, teniendo en cuenta que ningún otro proceso lo esté haciendo al mismo tiempo
 * 
 * @param estadoDestino: Estructura del estado al que se quiere encolar el pcb
 * @param pcbAEncolar: Pcb a encolar
 * 
 * @example estado_encolar_pcb(estadoNew, nuevoPcb);
 */
void estado_encolar_pcb_atomic(t_estado *estadoDestino, t_pcb *pcbAEncolar);

/**
 * @brief Desencola el primer pcb de un estado de forma atomica
 * 
 * @param self: Estado correspondiente 
 * @return t_pcb*: Retorna el pcb desencolado 
 */
t_pcb *estado_desencolar_primer_pcb_atomic(t_estado *self);

/**
 * @brief Desencola un pcb pasado por parametro por pid
 * 
 * @param self: Estado correspondiente 
 * @param pcbADesencolar: Pcb que se quiere desencolar 
 * @return t_pcb*: Pcb desencolado 
 */
t_pcb *estado_remover_pcb_de_cola_atomic(t_estado *self, t_pcb *pcbADesencolar);

/**
 * @brief Retorna si un estado contiene o no un pcb en su lista
 * 
 * @param self: Estado correspondiente 
 * @param pcbBuscado: Pcb que se quiere encontrar
 * @return true: Fue encontrado el pcb
 * @return false: No fue encontrado el pcb 
 */
bool estado_contiene_pcb_atomic(t_estado *self, t_pcb *pcbBuscado);

t_pcb *estado_remover_pcb_segun_maximo_hrrn_atomic(t_estado * estado);

bool estado_contiene_pcbs_atomic(t_estado *estado);

t_nombre_estado estado_get_nombre_estado(t_estado *self);

/**
 * @brief Devuelve la lista de procesos del estado
 * 
 * @param self: Estructura del estado del que se quiere obtener la lista de procesos
 * 
 * @return t_list*: Lista de procesos
 *
 * @example estado_get_list(estadoNew);
 */
t_list *estado_get_list(t_estado *self);

/**
 * @brief Devuelve el semaforo del estado
 * 
 * @param self: Semaforo del estado
 * @return sem_t*: Semaforo del estado 
 */
sem_t *estado_get_semaforo(t_estado *self);

/**
 * @brief Devuelve el mutex del estado
 * 
 * @param self: Estructura del estado del que se quiere obtener el mutex
 * 
 * @return pthread_mutex_t*: Mutex
 *
 * @example estado_get_mutex(estadoNew);
 */
pthread_mutex_t *estado_get_mutex(t_estado *self);

#endif