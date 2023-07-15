#ifndef KERNEL_PLANIFICACION_H_
#define KERNEL_PLANIFICACION_H_

// Defines constantes
#define ESTADO_NULL "NULL"
#define ESTADO_NEW "NEW"
#define ESTADO_READY "READY"
#define ESTADO_EXECUTE "EXEC"
#define ESTADO_BLOCKED "BLOCKED"
#define ESTADO_EXIT "EXIT"

#define FINALIZACION_SUCCESS "SUCCESS"
#define FINALIZACION_SEGFAULT "SEG_FAULT"
#define FINALIZACION_OUTOFMEMORY "OUT_OF_MEMORY"
#define FINALIZACION_RECURSOS "INVALID_RESOURCE"

//Bibliotecas estandares
#include <pthread.h>
#include <semaphore.h>

//Bibliotecas internas modulo kernel
#include <kernel-estructuras.h>
#include <kernel-estados.h>
#include <kernel-utils.h>
#include <kernel-adapter-memoria.h>
#include <kernel-adapter-cpu.h>
#include <kernel-pcb.h>
#include <kernel-instrucciones.h>

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


void pcb_pasar_de_blocked_a_ready_public(t_pcb* pcbAReady);
void pcb_pasar_de_running_a_blocked_public(t_pcb* pcbABlocked);
void pcb_pasar_de_running_a_exit_public(t_pcb* pcbAExit);
void seguir_ejecutando(t_pcb* pcbEnEjecucion);
void terminar_proceso(t_pcb* pcbFinalizado, char *motivoFinalizacion);

#endif
