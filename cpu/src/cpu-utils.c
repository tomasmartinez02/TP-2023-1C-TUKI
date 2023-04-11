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