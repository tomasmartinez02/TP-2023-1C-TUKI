#ifndef KERNEL_PCB_H
#define KERNEL_PCB_H

//Standard Libraries
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

//Static-utils libraries
#include <serializacion/buffer.h>

// Bibliotecas internas
#include <kernel-estructuras.h>
#include <kernel-config.h>


t_pcb* crear_pcb(uint32_t pid);

void destruir_pcb(t_pcb* pcb);

uint32_t pcb_get_pid(t_pcb* pcb);

t_buffer* pcb_get_instrucciones(t_pcb* pcb);

void pcb_set_instrucciones(t_pcb* pcb, t_buffer* instructionsBuffer);

uint32_t pcb_get_program_counter(t_pcb* pcb);

void pcb_set_program_counter(t_pcb* pcb, uint32_t programCounter);

uint32_t pcb_get_estimado_prox_rafaga(t_pcb *pcb);

void pcb_set_estimado_prox_rafaga (t_pcb *pcb, uint32_t estimadoProxRafaga);

uint32_t pcb_get_tiempo_llegada_ready(t_pcb *pcb);

void pcb_set_tiempo_llegada_ready(t_pcb *pcb, uint32_t tiempoLlegadaReady);

uint32_t pcb_get_socket(t_pcb *pcb);

void pcb_set_socket(t_pcb *pcb, uint32_t socket);

t_nombre_estado pcb_get_estado_actual(t_pcb* pcb);

void pcb_set_estado_actual(t_pcb* pcb, t_nombre_estado estadoActual);

t_nombre_estado pcb_get_estado_anterior(t_pcb* pcb);

void pcb_set_estado_anterior(t_pcb* pcb, t_nombre_estado estadoAnterior);

t_nombre_estado pcb_get_estado_finalizacion(t_pcb* pcb);

void pcb_set_estado_finalizacion(t_pcb* pcb,t_nombre_estado estadoDeFinalizacion);

bool pcb_es_este_pcb_por_pid(t_pcb* unPcb, t_pcb* otroPcb);

bool pcb_es_proceso_bloqueado_o_terminado(t_pcb* pcb);

void pcb_set_proceso_bloqueado_o_terminado(t_pcb* pcb, bool procesoBloqueadoOTerminado);

t_registros_cpu* pcb_get_registros_cpu(t_pcb* pcb);

void pcb_set_registros_cpu(t_pcb *pcb, t_registros_cpu *registrosCpu);

#endif 