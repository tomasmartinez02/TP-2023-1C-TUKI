#ifndef CPU_UTILS_H
#define CPU_UTILS_H

// Bibliotecas static utils
#include <utils/common-utils.h>
// Bibliotecas internas
#include <cpu-estructuras.h>
#include <cpu-config.h>

// Prototipos

/**
 * @brief Destruye las estructuras principales del cpu
 */
void cpu_destroy(t_cpu_config *cpuConfig, t_log *cpuLogger, t_log *cpuDebuggingLogger);

#endif

