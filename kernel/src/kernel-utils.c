#include <kernel-utils.h>

// Funciones privadas

// Adapta la funcion para destruir el config
static void __config_destroyer(void *moduleConfig)
{
    t_kernel_config *kernelConfigTemp = (t_kernel_config *) moduleConfig;
    kernel_config_destroy(kernelConfigTemp);

    return;
}

// Funcion utilizada para mapear un pcb con su pid
static void *__pcb_to_pid_transformer(void *pcbATransformar)
{
    t_pcb *tempPcbATransformar = (t_pcb *) pcbATransformar;
    uint32_t *tempPid = malloc(sizeof(*tempPid));
    
    *tempPid = pcb_get_pid(tempPcbATransformar);

    return (void *) tempPid; 
}

// Libera la memoria de un pid guardado en una lista
static void __pid_destroyer(void *pidADestruir)
{
    free(pidADestruir);
}

// Setea el estado final y logea la transicion 


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

char *string_pids_ready(t_estado *estadoReady)
{
    // Agarro la lista de pcbs en ready de forma atomica y la mapeo a una lista de sus pids
    t_list *tempPidList = list_map(estado_get_list(estadoReady), __pcb_to_pid_transformer);

    // Creo el string de los pids en ready a partir de los pcbs en ready
    char *listaPidsString = string_new();
    string_append(&listaPidsString, "[");
    for (int i = 0; i < tempPidList->elements_count; i++) {
        
        uint32_t tempPid = *(uint32_t *) list_get(tempPidList, i);

        char *stringPid = string_itoa(tempPid);
        string_append(&listaPidsString, stringPid);
        free(stringPid);
        
        if (i != tempPidList->elements_count - 1) {

            string_append(&listaPidsString, ", ");
        }
    }
    string_append(&listaPidsString, "]");

    list_destroy_and_destroy_elements(tempPidList, __pid_destroyer);
    return listaPidsString;
}

void log_ingreso_cola_ready(t_estado *estadoReady)
{
    char *stringPidsReady = string_pids_ready(estadoReady);
    char *algoritmoPlanificacion = kernel_config_get_algoritmo_planificacion(kernelConfig);
    log_info(kernelLogger, "Cola Ready <%s>: %s", algoritmoPlanificacion, stringPidsReady);
    log_info(kernelDebuggingLogger, "Cola Ready <%s>: %s", algoritmoPlanificacion, stringPidsReady);
    free(stringPidsReady);

    return;
}

void log_finalizacion_proceso(t_pcb *pcbFinalizado, char *motivoFinalizacion)
{
    log_info(kernelLogger, "Finaliza el proceso con PID <%d> - Motivo: <%s>", pcb_get_pid(pcbFinalizado), motivoFinalizacion);
    log_info(kernelDebuggingLogger, "Finaliza el proceso con PID <%d> - Motivo: <%s>", pcb_get_pid(pcbFinalizado), motivoFinalizacion);

    return;
}