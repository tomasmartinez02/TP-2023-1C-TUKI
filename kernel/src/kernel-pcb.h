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

void pcb_set_socket(t_pcb *pcb, int socket);

#endif 