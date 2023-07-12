#ifndef CPU_UTILS_H_
#define CPU_UTILS_H_

// Bibliotecas static utils
#include <utils/common-utils.h>
#include <utils/instrucciones.h>
// Bibliotecas internas
#include <cpu-estructuras.h>
#include <cpu-config.h>
#include <cpu-pcb.h>

// Prototipos

/**
 * @brief Destruye las estructuras principales del cpu
 */
void cpu_destroy(t_cpu_config *cpuConfig, t_log *cpuLogger, t_log *cpuDebuggingLogger);

void log_instruccion_ejecutada(t_cpu_pcb *pcb, t_instruccion *instruccion);

void log_acceso_a_memoria(uint32_t pid, char* modo, uint32_t idSegmento, uint32_t dirFisica, void* valor, uint32_t tamanio);

#endif

