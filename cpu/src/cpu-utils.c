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
    uint32_t pid = cpu_pcb_get_pid(pcb);
    char *instruccionString = instruccion_get_to_string(instruccion);

    log_info(cpuLogger, "PID: <%u> - Ejecutando: %s", pid, instruccionString);

    free(instruccionString);
    return;
}

void log_acceso_a_memoria(uint32_t pid, char* modo, uint32_t idSegmento, uint32_t dirFisica, void* valor, uint32_t tamanio)
{
    char* valorPrinteable = agregarCaracterNulo(valor, tamanio);
    log_info(cpuLogger, "PID: <%d> - Acción: <%s> - Segmento: <%d> - Dirección Física: <%d> - Valor: <%s>", pid, modo, idSegmento, dirFisica, valorPrinteable);
    free(valorPrinteable);
    return;
}