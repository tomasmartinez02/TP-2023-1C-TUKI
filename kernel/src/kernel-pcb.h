#ifndef KERNEL_PCB_H_
#define KERNEL_PCB_H_

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
#include <kernel-utils.h>
#include <kernel-estados.h>
#include <kernel-planificacion.h>

// Prototipos

// Builders y destroyers de estructuras
t_pcb *crear_pcb(uint32_t pid);
void destruir_pcb(t_pcb* pcb);
void destruir_info_segmentos(t_info_segmentos *infoSegmentos);

// Interfaz pcb
uint32_t pcb_get_pid(t_pcb* pcb);
t_buffer* pcb_get_instrucciones(t_pcb* pcb);
void pcb_set_instrucciones(t_pcb* pcb, t_buffer* instructionsBuffer);
uint32_t pcb_get_program_counter(t_pcb* pcb);
void pcb_set_program_counter(t_pcb* pcb, uint32_t programCounter);
double pcb_get_estimado_prox_rafaga(t_pcb *pcb);
void pcb_set_estimado_prox_rafaga (t_pcb *pcb, double estimadoProxRafaga);
pthread_mutex_t* pcb_get_mutex(t_pcb* pcb);
t_dictionary *pcb_get_archivos_abiertos(t_pcb *pcb);
t_info_segmentos **pcb_get_tabla_segmentos(t_pcb *pcb);
timestamp *pcb_get_tiempo_llegada_ready(t_pcb *pcb);
void pcb_set_tiempo_llegada_ready(t_pcb *pcb);
uint32_t pcb_get_socket(t_pcb *pcb);
void pcb_set_socket(t_pcb *pcb, uint32_t socket);
t_nombre_estado pcb_get_estado_actual(t_pcb* pcb);
void pcb_set_estado_actual(t_pcb* pcb, t_nombre_estado estadoActual);
t_nombre_estado pcb_get_estado_anterior(t_pcb* pcb);
void pcb_set_estado_anterior(t_pcb* pcb, t_nombre_estado estadoAnterior);
t_nombre_estado pcb_get_estado_finalizacion(t_pcb* pcb);
void pcb_set_estado_finalizacion(t_pcb* pcb, t_nombre_estado estadoDeFin);
bool pcb_es_este_pcb_por_pid(void *unPcb, void *otroPcb);
bool pcb_es_proceso_bloqueado_o_terminado(t_pcb* pcb);
void pcb_set_proceso_bloqueado_o_terminado(t_pcb* pcb, bool procesoBloqueadoOTerminado);
t_registros_cpu* pcb_get_registros_cpu(t_pcb* pcb);
void pcb_set_registros_cpu(t_pcb *pcb, t_registros_cpu *registrosCpu);
void pcb_set_tabla_segmentos(t_pcb *pcb, t_info_segmentos **tablaSegmentos);
void *comparar_pcb_segun_hrrn(void *pcbA, void *pcbB);
void pcb_estimar_proxima_rafaga(t_pcb *pcbEjecutado, double tiempoRealEjecutadoEnCpu);
void pcb_set_tamanio_tabla_segmentos (t_pcb *pcb, uint32_t tamanioTablaSegmentos); 
uint32_t pcb_get_tamanio_tabla_segmentos (t_pcb *pcb);
char *pcb_get_dispositivoIO(t_pcb* pcb);
void pcb_set_dispositivoIO(t_pcb* pcb, char* dispositivoIo);
#endif 