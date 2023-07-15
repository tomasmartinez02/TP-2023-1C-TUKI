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

// Transforma cualquier string en un string amarillo

// Funciones publicas

void kernel_destroy(t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger)
{
    module_destroy((void *) kernelConfig, __config_destroyer, kernelLogger, kernelDebuggingLogger);

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

void log_transicion_estados(char *estadoAnterior, char *estadoActual, uint32_t pid) 
{
    log_info(kernelLogger, "PID <%d> - Estado Anterior: <%s> - Estado Actual: <%s>", pid, estadoAnterior, estadoActual);

    return;
}

void log_creacion_nuevo_proceso(t_pcb *pcb)
{
    log_info(kernelLogger, "Se crea el proceso <%d> en NEW", pcb_get_pid(pcb));

    return;
}

void log_ingreso_cola_ready(t_estado *estadoReady)
{
    char *stringPidsReady = string_pids_ready(estadoReady);
    char *algoritmoPlanificacion = kernel_config_get_algoritmo_planificacion(kernelConfig);
    
    log_info(kernelLogger, "Cola Ready <%s>: %s", algoritmoPlanificacion, stringPidsReady);

    free(stringPidsReady);
    
    return;
}

void log_finalizacion_proceso(t_pcb *pcb, char *motivoFinalizacion)
{
    log_info(kernelLogger, "Finaliza el proceso con PID <%d> - Motivo: <%s>", pcb_get_pid(pcb), motivoFinalizacion);

    return;
}

void log_creacion_nuevo_segmento(t_pcb *pcb, uint32_t idSegmento, uint32_t tamanio)
{
    log_info(kernelLogger, "PID: <%d> - Crear Segmento - Id: <%d> - Tamaño: <%d>", pcb_get_pid(pcb), idSegmento, tamanio);

    return;
}

void log_ejecucion_wait(t_pcb* pcb, char* nombreRecurso, int32_t instanciasRecurso)
{   
    log_info(kernelLogger, "PID: <%d> - Wait: <%s> - Instancias: <%d>", pcb_get_pid(pcb), nombreRecurso, instanciasRecurso);

    return;
}

void log_ejecucion_signal(t_pcb* pcb, char* nombreRecurso, int32_t instanciasRecurso)
{   
    log_info(kernelLogger, "PID: <%d> - Signal: <%s> - Instancias: <%d>", pcb_get_pid(pcb), nombreRecurso, instanciasRecurso);

    return;
}

void log_ejecucion_io(t_pcb* pcb, uint32_t tiempo)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Ejecuta IO: <%d>", pid, tiempo);

    return;   
}
void log_ejecucion_fopen(t_pcb* pcb, char *nombreArchivo)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Abrir Archivo: <%s>", pid, nombreArchivo);

    return;   
}

void log_ejecucion_fclose(t_pcb* pcb, char *nombreArchivo)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Cerrar Archivo: <%s>", pid, nombreArchivo);

    return;   
}

void log_ejecucion_fseek(t_pcb* pcb, char *nombreArchivo, uint32_t puntero)
{
    //Nota: El valor del puntero debe ser luego de ejecutar F_SEEK.
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Actualizar puntero Archivo: <%s> - Puntero: <%d>", pid, nombreArchivo, puntero);

    return;   
}

void log_ejecucion_ftruncate(t_pcb* pcb, char *nombreArchivo, uint32_t tamanio)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Truncar Archivo: <%s> - Tamanio: <%d>", pid, nombreArchivo, tamanio);

    return;   
}

void log_ejecucion_fread(t_pcb* pcb, char *nombreArchivo, uint32_t puntero, uint32_t direccion, uint32_t tamanio)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Leer Archivo: <%s> - Puntero: <%d> - Direccion Memoria: <%d> - Tamanio: <%d>" , pid, nombreArchivo, puntero, direccion, tamanio);

    return;   
}

void log_ejecucion_fwrite(t_pcb* pcb, char *nombreArchivo, uint32_t puntero, uint32_t direccion, uint32_t tamanio)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Escribir Archivo: <%s> - Puntero: <%d> - Direccion Memoria: <%d> - Tamanio: <%d>" , pid, nombreArchivo, puntero, direccion, tamanio);

    return;   
}

