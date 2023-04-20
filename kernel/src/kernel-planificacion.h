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

// Variable global
static uint32_t pidAnterior;
static pthread_mutex_t mutexPid; 

//Prototipos

/**
 * @brief Inicializa el pid y crea el mutex a utilizar despues
 * 
 * @example inicializar_pid();
 */
void inicializar_pid();

/**
 * @brief Obtiene el siguiente pid a asignar
 * 
 * @return unit32_t: Pid para proceso
 * 
 * @example unit32_t nuevoPid = obtener_siguiente_pid(); 
 */
uint32_t obtener_siguiente_pid();

/**
 * @brief Encola en new al pcb del hilo de ejecucion de consola, obtiene pid y se lo envia a consola
 * 
 * @param socket: Socket de la consola
 * 
 * @example encolar_en_new_a_nuevo_pcb_entrante(socketProceso); 
 */
void* encolar_en_new_a_nuevo_pcb_entrante(void* socket);

#endif