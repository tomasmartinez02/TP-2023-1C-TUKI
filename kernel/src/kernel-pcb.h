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

t_pcb* crear_pcb(uint32_t pid);
void destruir_pcb(t_pcb* pcb);

uint32_t pcb_get_pid(t_pcb* pcb);

t_buffer* pcb_get_instrucciones(t_pcb* pcb);
void pcb_set_instrucciones(t_pcb* pcb, t_buffer* instructionsBuffer);

uint32_t pcb_get_program_counter(t_pcb* pcb);
void pcb_set_program_counter(t_pcb* pcb, uint32_t programCounter);

//t_registros_cpu* pcb_get_registros_cpu(t_pcb* pcb);
//void pcb_set_registros_cpu(t_pcb* pcb, t_registros_cpu* registrosCpu);

uint32_t pcb_get_estimado_prox_rafaga (t_pcb *pcb);
void pcb_set_estimado_prox_rafaga (t_pcb *pcb, uint32_t estimadoProxRafaga);

uint32_t pcb_get_tiempo_llegada_ready (t_pcb *pcb);
void pcb_set_tiempo_llegada_ready (t_pcb *pcb, uint32_t tiempoLlegadaReady);

void pcb_set_socket(t_pcb *pcb, uint32_t socket);
uint32_t pcb_get_socket(t_pcb *pcb);

t_nombre_estado pcb_get_estado_actual(t_pcb* self);
void pcb_set_estado_actual(t_pcb* self, t_nombre_estado estadoActual);

t_nombre_estado pcb_get_estado_anterior(t_pcb* self);
t_nombre_estado pcb_get_estado_finalizacion(t_pcb* self);
void pcb_set_estado_anterior(t_pcb* self, t_nombre_estado estadoAnterior);
void pcb_set_estado_finalizacion(t_pcb* self,t_nombre_estado estadoDeFin);

bool pcb_es_este_pcb_por_pid(t_pcb* unPcb, t_pcb* otroPcb);
bool pcb_es_proceso_bloqueado_o_terminado(t_pcb* self);
void pcb_set_proceso_bloqueado_o_terminado(t_pcb* self, bool procesoBloqueadoOTerminado);

/*
void pcb_set_estado_anterior(t_pcb* self, t_nombre_estado estadoAnterior);
bool pcb_es_proceso_bloqueado_o_terminado_atomic(t_pcb* self);
void pcb_set_proceso_bloqueado_o_terminado_atomic(t_pcb* self, bool procesoBloqueadoOTerminado);
uint32_t pcb_get_registro_ax_cpu(t_pcb* self);
uint32_t pcb_get_registro_bx_cpu(t_pcb* self);
uint32_t pcb_get_registro_cx_cpu(t_pcb* self);
uint32_t pcb_get_registro_dx_cpu(t_pcb* self);
void pcb_set_registro_ax_cpu(t_pcb* self, uint32_t registro);
void pcb_set_registro_bx_cpu(t_pcb* self, uint32_t registro);
void pcb_set_registro_cx_cpu(t_pcb* self, uint32_t registro);
void pcb_set_registro_dx_cpu(t_pcb* self, uint32_t registro);
*/

#endif 