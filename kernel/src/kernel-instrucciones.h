#ifndef KERNEL_INSTRUCCIONES_H
#define KERNEL_INSTRUCCIONES_H

// Bibliotecas internas
#include <kernel-estructuras.h>
#include <kernel-semaforos-recursos.h>
#include <kernel-utils.h>
#include <kernel-planificacion.h>

#include <unistd.h>
#include <pthread.h>

void ejecutar_instruccion_io(t_pcb *pcbEnEjecucion, uint32_t tiempoEjecucion);
void ejecutar_instruccion_wait(t_pcb *pcbEnEjecucion, char *recurso);
void ejecutar_instruccion_signal(t_pcb *pcbEnEjecucion, char *recurso);

void ejecutar_instruccion_fopen(t_pcb *pcbEnEjecucion, char *nombreArchivo);
void ejecutar_instruccion_fclose(t_pcb *pcbEnEjecucion, char *nombreArchivo);
void ejecutar_instruccion_fseek(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t ubicacionNueva);

#endif