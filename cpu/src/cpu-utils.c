#include <cpu-utils.h>

// Funciones privadas

// Adapta la funcion para destruir el config
static void __config_destroyer(void *moduleConfig)
{
    t_cpu_config *cpuConfigTemp = (t_cpu_config *) moduleConfig;
    cpu_config_destroy(cpuConfigTemp);

    return;
}


// Funciones publicas
void cpu_destroy(t_cpu_config *cpuConfig, t_log *cpuLogger, t_log *cpuDebuggingLogger)
{
    module_destroy((void *) cpuConfig, __config_destroyer, cpuLogger, cpuDebuggingLogger);

    return;
}

void log_instruccion_ejecutada(t_cpu_pcb *pcb, t_instruccion *instruccion)
{
    log_info(cpuLogger, "PID: <%d> - Ejecutando: %s", cpu_pcb_get_pid(pcb), instruccion_get_to_string(instruccion));
    log_info(cpuDebuggingLogger, "PID: <%d> - Ejecutando: %s", cpu_pcb_get_pid(pcb), instruccion_get_to_string(instruccion));
    return;
}