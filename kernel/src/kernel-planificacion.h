#ifndef KERNEL_PLANIFICACION_H_
#define KERNEL_PLANIFICACION_H_

//Bibliotecas estandares
#include <pthread.h>
#include <semaphore.h>

//Bibliotecas internas modulo kernel
#include <kernel-estructuras.h>
#include <kernel-estados.h>
#include <kernel-utils.h>

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

#endif