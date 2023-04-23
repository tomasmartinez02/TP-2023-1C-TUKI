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

void log_transicion_estados(char *estadoAnterior, char *estadoActual, uint32_t pid) 
{
    log_info(kernelLogger, "PID <%d> - Estado Anterior: <%s> - Estado Actual: <%s>", pid, estadoAnterior, estadoActual);
    log_info(kernelDebuggingLogger, "PID <%d> - Estado Anterior: <%s> - Estado Actual: <%s>", pid, estadoAnterior, estadoActual);

    return;
}

void set_timespec(timestamp *timespec) 
{
    int retVal = clock_gettime(CLOCK_REALTIME, timespec);
    
    if (retVal == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
}

double obtener_diferencial_de_tiempo_en_milisegundos(timestamp *end, timestamp *start) 
{
    const uint32_t SECS_TO_MILISECS = 1000;
    const uint32_t NANOSECS_TO_MILISECS = 1000000;
    return (double) ( (end->tv_sec - start->tv_sec) * SECS_TO_MILISECS + (end->tv_nsec - start->tv_nsec) / NANOSECS_TO_MILISECS );
}