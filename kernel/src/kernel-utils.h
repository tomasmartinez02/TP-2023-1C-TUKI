// Modulo con funciones utilitarias del modulo consola
#ifndef KERNEL_UTILS_H_
#define KERNEL_UTILS_H_

// Bibliotecas static utils
#include <utils/common-utils.h>
// Bibliotecas internas
#include <kernel-estructuras.h>
#include <kernel-config.h>
#include <kernel-pcb.h>
#include <kernel-estados.h>

// Prototipos

/**
 * @brief Destruye las estructuras principales del kernel
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * 
 * @example kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger); 
 */
void kernel_destroy(t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger);

void set_timespec(timestamp *timespec);
double obtener_diferencial_de_tiempo_en_milisegundos(timestamp *end, timestamp *start);
char *string_pids_ready(t_estado *estadoReady);
void log_transicion_estados(char *estadoAnterior, char *estadoActual, uint32_t pid);
void log_creacion_nuevo_proceso(t_pcb *pcbNuevo);
void log_ingreso_cola_ready(t_estado *estadoReady);
void log_finalizacion_proceso(t_pcb *pcbFinalizado, char *motivoFinalizacion);
void log_ejecucion_signal(t_pcb* pcb, char* nombreRecurso, uint32_t instanciasRecurso);
void log_ejecucion_wait(t_pcb* pcb, char* nombreRecurso, uint32_t instanciasRecurso);
void log_ejecucion_io(t_pcb* pcb, uint32_t tiempo);

#endif