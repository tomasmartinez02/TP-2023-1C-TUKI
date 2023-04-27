#ifndef KERNEL_ADAPTER_MEMORIA_
#define KERNEL_ADAPTER_MEMORIA_

// Bibliotecas
#include <pthread.h>
#include <semaphore.h>

#include <kernel-estructuras.h>
#include <kernel-pcb.h>

#include <serializacion/stream.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>


// Acá hay q ver cuales poner

// Prototipos

/**
 * @brief Solicita la inicializacion del proceso con la tabla de segmentos inicial
 * 
 * @return t_info_segmentos: tabla de segmentos inicial
 * 
 * @example t_info_segmentos *tablaSegmentos = adapter_memoria_pedir_inicializacion_proceso(pcbAReady);
 */
t_info_segmentos *adapter_memoria_pedir_inicializacion_proceso (t_pcb *pcbAMandar);

/**
 * @brief Solicita la finalizacion del proceso a memoria
 * 
 * @param pcbATerminar: PCB que finalizo
 * 
 * @example adapter_memoria_finalizar_proceso(pcbATerminar);
 */
void adapter_memoria_finalizar_proceso (t_pcb *pcbATerminar);




#endif