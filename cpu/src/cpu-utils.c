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
    char *pidAmarillo = int_to_yellow_string(cpu_pcb_get_pid(pcb)); 
    char *instruccionString = instruccion_get_to_string(instruccion);
    char *instruccionStringAmarillo = string_to_yellow_string(instruccionString);

    log_info(cpuLogger, "PID: <%s> - Ejecutando: %s", pidAmarillo, instruccionStringAmarillo);
    log_info(cpuDebuggingLogger, "PID: <%s> - Ejecutando: %s", pidAmarillo, instruccionStringAmarillo);

    free(instruccionStringAmarillo);
    free(instruccionString);
    free(pidAmarillo);
    return;
}