#ifndef KERNEL_ADAPTER_MEMORIA_
#define KERNEL_ADAPTER_MEMORIA_

// Bibliotecas
#include <pthread.h>
#include <semaphore.h>

#include <kernel-estructuras.h>
#include <kernel-pcb.h>
#include <kernel-planificacion.h>

#include <serializacion/stream.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>

// Acá hay q ver cuales poner

// Prototipos

/**
 * @brief Solicita la inicializacion del proceso con la tabla de segmentos inicial
 * 
 * @return t_buffer: tabla de segmentos inicial
 * 
 * @example t_buffer *tablaSegmentos = adapter_memoria_pedir_inicializacion_proceso(pcbAReady);
 */
t_info_segmentos **adapter_memoria_pedir_inicializacion_proceso(t_pcb *pcbAInicializar);

/**
 * @brief Solicita la finalizacion del proceso a memoria
 * 
 * @param pcbATerminar: PCB que finalizo
 * 
 * @example adapter_memoria_finalizar_proceso(pcbATerminar);
 */
void adapter_memoria_finalizar_proceso (t_pcb *pcbATerminar);

void __pcb_pasar_a_exit(t_pcb* pcbAExit, char *stringEstadoViejo);

void adapter_memoria_pedir_creacion_segmento(uint32_t idSegmento, uint32_t tamanio, t_pcb* pcb);
void adapter_memoria_pedir_eliminar_segmento(uint32_t idSegmento, t_pcb* pcb);
void adapter_memoria_pedir_compactacion(); 

void actualizar_tabla_segmentos(lista_tablas *bufferTablaDeSegmentosActualizada, t_list *listaProcesos);

#endif