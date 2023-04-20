#include <kernel-utils.h>

// Funciones privadas

// Adapta la funcion para destruir el config
static void __config_destroyer(void *moduleConfig)
{
    t_kernel_config *kernelConfigTemp = (t_kernel_config *) moduleConfig;
    kernel_config_destroy(kernelConfigTemp);

    return;
}


// Funciones publicas
void kernel_destroy(t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger)
{
    module_destroy((void *) kernelConfig, __config_destroyer, kernelLogger, kernelDebuggingLogger);

    return;
}

void log_transicion(char* prev, char* post, uint32_t pid) 
{
    log_info(kernelLogger, "Transición de %s a %s PCB <ID %d>", prev, post, pid);
    log_info(kernelDebuggingLogger, "Transición de %s a %s PCB <ID %d>", prev, post, pid);

    return;
}