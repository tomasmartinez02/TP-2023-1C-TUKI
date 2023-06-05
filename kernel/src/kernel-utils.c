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
    char *pidAmarillo = int_to_yellow_string(pid);
    char *estadoAnteriorAmarillo = string_to_yellow_string(estadoAnterior);
    char *estadoActualAmarillo = string_to_yellow_string(estadoActual);

    log_info(kernelLogger, "PID <%s> - Estado Anterior: <%s> - Estado Actual: <%s>", pidAmarillo, estadoAnteriorAmarillo, estadoActualAmarillo);
    log_info(kernelDebuggingLogger, "PID <%s> - Estado Anterior: <%s> - Estado Actual: <%s>", pidAmarillo, estadoAnteriorAmarillo, estadoActualAmarillo);

    free(estadoActualAmarillo);
    free(estadoAnteriorAmarillo);
    free(pidAmarillo);
    return;
}

void log_creacion_nuevo_proceso(t_pcb *pcbNuevo)
{
    char *pidAmarillo = int_to_yellow_string(pcb_get_pid(pcbNuevo));

    log_info(kernelLogger, "Creación de nuevo proceso con PID <%s> en NEW", pidAmarillo);
    log_info(kernelDebuggingLogger, "Creación de nuevo con proceso PID <%s> en NEW", pidAmarillo);

    free(pidAmarillo);
    return;
}

void log_ingreso_cola_ready(t_estado *estadoReady)
{
    char *stringPidsReady = string_pids_ready(estadoReady);
    char *stringPidsReadyAmarillo = string_to_yellow_string(stringPidsReady);
    char *algoritmoPlanificacionAmarillo = string_to_yellow_string(kernel_config_get_algoritmo_planificacion(kernelConfig));
    
    log_info(kernelLogger, "Cola Ready <%s>: %s", algoritmoPlanificacionAmarillo, stringPidsReadyAmarillo);
    log_info(kernelDebuggingLogger, "Cola Ready <%s>: %s", algoritmoPlanificacionAmarillo, stringPidsReadyAmarillo);

    free(algoritmoPlanificacionAmarillo);
    free(stringPidsReadyAmarillo);
    free(stringPidsReady);
    return;
}

void log_finalizacion_proceso(t_pcb *pcbFinalizado, char *motivoFinalizacion)
{
    char *pidAmarillo = int_to_yellow_string(pcb_get_pid(pcbFinalizado));
    char *motivoFinalizacionAmarillo = string_to_yellow_string(motivoFinalizacion);
    
    log_info(kernelLogger, "Finaliza el proceso con PID <%s> - Motivo: <%s>", pidAmarillo, motivoFinalizacionAmarillo);
    log_info(kernelDebuggingLogger, "Finaliza el proceso con PID <%s> - Motivo: <%s>", pidAmarillo, motivoFinalizacionAmarillo);

    free(motivoFinalizacionAmarillo);
    free(pidAmarillo);
    return;
}

void log_creacion_nuevo_segmento(t_pcb *pcb, uint32_t idSegmento, uint32_t tamanio)
{
    char *pidAmarillo = int_to_yellow_string(pcb_get_pid(pcb));
    char *idSegmentoAmarillo = int_to_yellow_string(pcb_get_pid(pcb));
    char *tamanioAmarillo = int_to_yellow_string(pcb_get_pid(pcb));

    log_info(kernelLogger, "PID: <%s> - Crear Segmento - Id: <%s> - Tamaño: <%s>", pidAmarillo, idSegmentoAmarillo, tamanioAmarillo);
    log_info(kernelDebuggingLogger, "PID: <%s> - Crear Segmento - Id: <%s> - Tamaño: <%s>", pidAmarillo, idSegmentoAmarillo, tamanioAmarillo);
    
    free(pidAmarillo);
    free(idSegmentoAmarillo);
    free(tamanioAmarillo);
    return;
}

void log_ejecucion_wait(t_pcb* pcb, char* nombreRecurso, int32_t instanciasRecurso)
{   
    char *pidAmarillo = int_to_yellow_string(pcb_get_pid(pcb));
    char *nombreRecursoAmarillo = string_to_yellow_string(nombreRecurso);
    char *instanciasRecursoAmarillo = int_to_yellow_string(instanciasRecurso);

    log_info(kernelLogger, "PID: <%s> - Wait: <%s> - Instancias: <%s>", pidAmarillo, nombreRecursoAmarillo, instanciasRecursoAmarillo);
    log_info(kernelDebuggingLogger, "PID: <%s> - Wait: <%s> - Instancias: <%s>", pidAmarillo, nombreRecursoAmarillo, instanciasRecursoAmarillo);
    
    free(pidAmarillo);
    free(nombreRecursoAmarillo);
    free(instanciasRecursoAmarillo);
    return;
}

void log_ejecucion_signal(t_pcb* pcb, char* nombreRecurso, int32_t instanciasRecurso)
{   
    char *pidAmarillo = int_to_yellow_string(pcb_get_pid(pcb));
    char *nombreRecursoAmarillo = string_to_yellow_string(nombreRecurso);
    char *instanciasRecursoAmarillo = int_to_yellow_string(instanciasRecurso);

    log_info(kernelLogger, "PID: <%s> - Signal: <%s> - Instancias: <%s>", pidAmarillo, nombreRecursoAmarillo, instanciasRecursoAmarillo);
    log_info(kernelDebuggingLogger, "PID: <%s> - Signal: <%s> - Instancias: <%s>", pidAmarillo, nombreRecursoAmarillo, instanciasRecursoAmarillo);
    free(pidAmarillo);
    free(nombreRecursoAmarillo);
    free(instanciasRecursoAmarillo);
    return;
}

void log_ejecucion_io(t_pcb* pcb, uint32_t tiempo)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Ejecuta IO: <%d>", pid, tiempo);
    log_info(kernelDebuggingLogger, "PID: <%d> - Ejecuta IO: <%d>", pid, tiempo);
    return;   
}
void log_ejecucion_fopen(t_pcb* pcb, char *nombreArchivo)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Abrir Archivo: <%s>", pid, nombreArchivo);
    log_info(kernelDebuggingLogger, "PID: <%d> - Abrir Archivo: <%s>", pid, nombreArchivo);
    return;   
}

void log_ejecucion_fclose(t_pcb* pcb, char *nombreArchivo)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Cerrar Archivo: <%s>", pid, nombreArchivo);
    log_info(kernelDebuggingLogger, "PID: <%d> - Cerrar Archivo: <%s> <3", pid, nombreArchivo);
    return;   
}

void log_ejecucion_fseek(t_pcb* pcb, char *nombreArchivo, uint32_t puntero)
{
    //Nota: El valor del puntero debe ser luego de ejecutar F_SEEK.
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Actualizar puntero Archivo: <%s> - Puntero: <%d>", pid, nombreArchivo, puntero);
    log_info(kernelDebuggingLogger, "PID: <%d> - Actualizar puntero Archivo: <%s> - Puntero: <%d>", pid, nombreArchivo, puntero);
    return;   
}

void log_ejecucion_ftruncate(t_pcb* pcb, char *nombreArchivo, uint32_t tamanio)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Abrir Archivo: <%s> - Tamanio: <%d>", pid, nombreArchivo, tamanio);
    log_info(kernelDebuggingLogger, "PID: <%d> - Abrir Archivo: <%s> - Tamanio: <%d>", pid, nombreArchivo, tamanio);
    return;   
}

void log_ejecucion_fread(t_pcb* pcb, char *nombreArchivo, uint32_t puntero, uint32_t direccion, uint32_t tamanio)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Leer Archivo: <%s> - Puntero: <%d> - Direccion Memoria: <%d> - Tamanio: <%d>" , pid, nombreArchivo, puntero, direccion, tamanio);
    log_info(kernelDebuggingLogger, "PID: <%d> - Leer Archivo: <%s> - Puntero: <%d> - Direccion Memoria: <%d> - Tamanio: <%d>" , pid, nombreArchivo, puntero, direccion, tamanio);
    return;   
}

void log_ejecucion_fwrite(t_pcb* pcb, char *nombreArchivo, uint32_t puntero, uint32_t direccion, uint32_t tamanio)
{   
    uint32_t pid = pcb_get_pid(pcb);
    log_info(kernelLogger, "PID: <%d> - Escribir Archivo: <%s> - Puntero: <%d> - Direccion Memoria: <%d> - Tamanio: <%d>" , pid, nombreArchivo, puntero, direccion, tamanio);
    log_info(kernelDebuggingLogger, "PID: <%d> - Escribir Archivo: <%s> - Puntero: <%d> - Direccion Memoria: <%d> - Tamanio: <%d>" , pid, nombreArchivo, puntero, direccion, tamanio);
    return;   
}

