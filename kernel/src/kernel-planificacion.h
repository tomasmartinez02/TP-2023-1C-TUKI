#ifndef KERNEL_PLANIFICACION_H_
#define KERNEL_PLANIFICACION_H_

// Defines constantes
#define ESTADO_NULL "NULL"
#define ESTADO_NEW "NEW"
#define ESTADO_READY "READY"
#define ESTADO_EXECUTE "EXEC"
#define ESTADO_BLOCKED "BLOCKED"
#define ESTADO_EXIT "EXIT"
#define PLANIFICACION_FIFO "FIFO"
#define PLANIFICACION_HRRN "HRRN"

//Bibliotecas estandares
#include <pthread.h>
#include <semaphore.h>

//Bibliotecas internas modulo kernel
#include <kernel-estructuras.h>
#include <kernel-estados.h>
#include <kernel-utils.h>
#include <kernel-adapter-memoria.h>
#include <kernel-pcb.h>

//Bibliotecas Static Utils
#include <serializacion/buffer.h>
#include <serializacion/stream.h>

//Prototipos

/**
 * @brief Encola en new al pcb del hilo de ejecucion de consola, obtiene pid y se lo envia a consola
 * 
 * @param socketCliente: Socket de la consola
 * 
 * @example encolar_en_new_a_nuevo_pcb_entrante(socketProceso); 
 */
void *encolar_en_new_a_nuevo_pcb_entrante(void *socketCliente);

/**
 * @brief Inicializa las estructuras del modulo
 * 
 */
void inicializar_estructuras(void);

/**
 * @brief 
 * 
 */
void* pcb_pasar_de_estado(t_pcb* pcb, t_estado *nuevoEstado);

static t_pcb* elegir_pcb_segun_fifo(t_estado* estado);
static t_pcb* elegir_pcb_segun_hrrn(t_estado* estado);
t_pcb* elegir_pcb(t_estado* estadoReady);



#endif