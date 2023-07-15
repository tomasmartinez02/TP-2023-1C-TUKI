#include <kernel-instrucciones.h>
// Utils
struct parametrosHilo{
    t_pcb* pcb;
    uint32_t tiempo;
} ParametrosHilo;
typedef struct parametrosHilo t_parametros_hilo;

void* sleepHilo(void* parametrosHilo) {
    t_parametros_hilo* parametros = (t_parametros_hilo*)parametrosHilo;
    uint32_t tiempo = parametros->tiempo;
    t_pcb *pcbEnEjecucion = parametros->pcb;

    intervalo_de_pausa(tiempo*1000);
    t_pcb* pcb = estado_remover_pcb_de_cola_atomic(estadoBlocked, pcbEnEjecucion); 
    pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);

    free(parametros);
    return NULL;
}

//  El proceso se bloquea por una cantidad determinada de tiempo.
void ejecutar_instruccion_io(t_pcb *pcbEnEjecucion, uint32_t tiempo)
{   
    pthread_t ejecutarTiempoIO;
    t_parametros_hilo* parametrosHilo = (t_parametros_hilo*)malloc(sizeof(t_parametros_hilo));
    parametrosHilo->tiempo = tiempo;
    parametrosHilo->pcb = pcbEnEjecucion;

    pcb_pasar_de_running_a_blocked_public(pcbEnEjecucion);
    log_info(kernelLogger, "PID: <%u> - Bloqueado por: IO", pcb_get_pid(pcbEnEjecucion));

    pthread_create(&ejecutarTiempoIO, NULL, sleepHilo, (void*) parametrosHilo);
    pthread_detach(ejecutarTiempoIO);
    log_ejecucion_io(pcbEnEjecucion, tiempo);
    return;
}

// El proceso solicita que se le asigne una instancia del recurso indicado por parámetro.
void ejecutar_instruccion_wait(t_pcb *pcbEnEjecucion, char *nombreRecurso)
{   
    t_dictionary *diccionarioSemaforosRecursos = kernel_config_get_diccionario_semaforos(kernelConfig);
    if (!diccionario_semaforos_recursos_existe_recurso(diccionarioSemaforosRecursos, nombreRecurso))
    {
        uint32_t pid = pcbEnEjecucion->pid;
        log_info(kernelLogger, "ERROR - PID: <%u> - <%s> NO existe", pid, nombreRecurso);
        log_error(kernelDebuggingLogger, "El recurso solicitado no existe");
        pcb_pasar_de_running_a_exit_public(pcbEnEjecucion);
        sem_post(&dispatchPermitido);
    }
    else 
    {
        t_semaforo_recurso *semaforoRecurso = diccionario_semaforos_recursos_get_semaforo_recurso(diccionarioSemaforosRecursos, nombreRecurso);
        int32_t instanciasRecurso = semaforo_recurso_get_instancias(semaforoRecurso);

        semaforo_recurso_wait(semaforoRecurso);
        if (semaforo_recurso_debe_bloquear_proceso(semaforoRecurso))
        {   
            semaforo_recurso_bloquear_proceso(semaforoRecurso, pcbEnEjecucion);
            pcb_pasar_de_running_a_blocked_public(pcbEnEjecucion);
            log_info(kernelLogger, "PID: <%u> - Bloqueado por: %s", pcb_get_pid(pcbEnEjecucion), nombreRecurso);
            sem_post(&dispatchPermitido);
        } else {
            // Si el proceso no se bloquea porque no hay ningún otro proceso usando este recurso
            seguir_ejecutando(pcbEnEjecucion);
        }

        log_ejecucion_wait(pcbEnEjecucion, nombreRecurso, instanciasRecurso);
    }
    return;
}

// El proceso solicita que se libere una instancia del recurso indicado por parámetro.
void ejecutar_instruccion_signal(t_pcb *pcbEnEjecucion, char *nombreRecurso)
{   
    t_dictionary *diccionarioSemaforosRecursos = kernel_config_get_diccionario_semaforos(kernelConfig);
    if (!diccionario_semaforos_recursos_existe_recurso(diccionarioSemaforosRecursos, nombreRecurso))
    {
        uint32_t pid = pcbEnEjecucion->pid;
        log_info(kernelLogger, "ERROR - PID: <%u> - <%s> NO existe", pid, nombreRecurso);
        log_error(kernelDebuggingLogger, "El recurso solicitado no existe.");
        pcb_pasar_de_running_a_exit_public(pcbEnEjecucion);
        sem_post(&dispatchPermitido);
    }
   else
    {
        t_semaforo_recurso *semaforoRecurso = diccionario_semaforos_recursos_get_semaforo_recurso(diccionarioSemaforosRecursos, nombreRecurso);
        int32_t instanciasRecurso = semaforo_recurso_get_instancias(semaforoRecurso);

        semaforo_recurso_post(semaforoRecurso);

        if(semaforo_recurso_debe_desbloquear_recurso(semaforoRecurso))
        {   
            // Desbloquea al primer proceso de la cola de bloqueados del recurso 
            t_pcb *pcbAEjecutar = semaforo_recurso_desbloquear_primer_proceso_bloqueado(semaforoRecurso);
            pcb_pasar_de_blocked_a_ready_public(pcbAEjecutar);
        }
        seguir_ejecutando(pcbEnEjecucion);
        log_ejecucion_signal(pcbEnEjecucion, nombreRecurso, instanciasRecurso);
    }

     // Seguir la ejecucion del proceso que peticiono el SIGNAL
    return;
}

void ejecutar_instruccion_fopen(t_pcb *pcbEnEjecucion, char *nombreArchivo)
{
    if (archivo_esta_abierto(nombreArchivo))
    {
        t_semaforo_recurso *semaforoArchivo = diccionario_semaforos_recursos_get_semaforo_recurso(tablaArchivosAbiertos, nombreArchivo);
        semaforo_recurso_bloquear_proceso(semaforoArchivo, pcbEnEjecucion);
        pcb_pasar_de_running_a_blocked_public(pcbEnEjecucion);
        log_info(kernelLogger, "PID: <%u> - Bloqueado por: %s", pcb_get_pid(pcbEnEjecucion), nombreArchivo);
        semaforo_recurso_wait(semaforoArchivo);
        sem_post(&dispatchPermitido);
        return;
    }
    uint32_t respuestaFilesystem = adapter_filesystem_existe_archivo(nombreArchivo);
    if (respuestaFilesystem == 0) {
        adapter_filesystem_pedir_creacion_archivo(nombreArchivo);
    }
    if (respuestaFilesystem == 2) {
        pcb_pasar_de_running_a_exit_public(pcbEnEjecucion);
        sem_post(&dispatchPermitido);
        return;
    }
    abrir_archivo_globalmente(nombreArchivo);
    // Agrego archivo a la tabla de archivos abiertos del proceso con el puntero en la posición 0. 
    abrir_archivo_en_tabla_de_pcb(pcbEnEjecucion, nombreArchivo);
    seguir_ejecutando(pcbEnEjecucion);
    log_ejecucion_fopen(pcbEnEjecucion, nombreArchivo);
    return;
}

void ejecutar_instruccion_fclose(t_pcb *pcbEnEjecucion, char *nombreArchivo)
{   
    t_semaforo_recurso *semaforoArchivo = diccionario_semaforos_recursos_get_semaforo_recurso(tablaArchivosAbiertos, nombreArchivo);
    cerrar_archivo_en_tabla_de_pcb(pcbEnEjecucion, nombreArchivo);
    // Checkea que haya procesos esperando y que el archivo este disponible
    if (semaforo_archivo_debe_desbloquear_archivo(semaforoArchivo))
    {
        // Desbloquea al primer proceso de la cola de bloqueados del recurso 
        t_pcb *pcbAEjecutar = semaforo_recurso_desbloquear_primer_proceso_bloqueado(semaforoArchivo);
        pcb_pasar_de_blocked_a_ready_public(pcbAEjecutar);
    }
    else
    {
        // Si no queda ningún proceso que tenga abierto el archivo, se elimina la entrada de la tabla global de archivos abiertos.
        cerrar_archivo_globalmente(nombreArchivo);
    }
    seguir_ejecutando(pcbEnEjecucion);
    log_ejecucion_fclose(pcbEnEjecucion, nombreArchivo);
    return;
}

void ejecutar_instruccion_fseek(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t ubicacionNueva)
{   
    actualizar_puntero_archivo_en_tabla_de_pcb(pcbEnEjecucion, nombreArchivo, ubicacionNueva);
    log_ejecucion_fseek(pcbEnEjecucion, nombreArchivo, ubicacionNueva);
    seguir_ejecutando(pcbEnEjecucion);
    return;
}

void ejecutar_instruccion_ftruncate(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t tamanio)
{   
    // El PCB se bloquea hasta que FS avisa que ya termino de truncar el archivo
    pcb_pasar_de_running_a_blocked_public(pcbEnEjecucion);
    adapter_filesystem_pedir_truncar_archivo(pcbEnEjecucion, nombreArchivo, tamanio);
    log_ejecucion_ftruncate(pcbEnEjecucion, nombreArchivo, tamanio);
    return;
}

void ejecutar_instruccion_fread(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t direccionFisica, uint32_t cantidadBytes)
{   
    t_dictionary *archivosAbiertos = pcb_get_archivos_abiertos(pcbEnEjecucion);
    uint32_t pidProceso = pcb_get_pid(pcbEnEjecucion);
    int32_t punteroArchivo = (int32_t)(intptr_t)dictionary_get(archivosAbiertos, nombreArchivo);
    // El PCB se bloquea hasta que FS avisa que ya termino de leer del archivo
    pcb_pasar_de_running_a_blocked_public(pcbEnEjecucion);
    adapter_filesystem_pedir_leer_archivo(pcbEnEjecucion, nombreArchivo, punteroArchivo, direccionFisica, cantidadBytes, pidProceso);
    log_ejecucion_fread(pcbEnEjecucion, nombreArchivo, punteroArchivo, direccionFisica, cantidadBytes);
    return;
}

void ejecutar_instruccion_fwrite(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t direccionFisica, uint32_t cantidadBytes)
{   
    t_dictionary *archivosAbiertos = pcb_get_archivos_abiertos(pcbEnEjecucion);
    uint32_t pidProceso = pcb_get_pid(pcbEnEjecucion);
    int32_t punteroArchivo = (int32_t)(intptr_t)dictionary_get(archivosAbiertos, nombreArchivo);
      // El PCB se bloquea hasta que FS avisa que ya termino de escribir el archivo
    pcb_pasar_de_running_a_blocked_public(pcbEnEjecucion);
    adapter_filesystem_pedir_escribir_archivo(pcbEnEjecucion, nombreArchivo, punteroArchivo, direccionFisica, cantidadBytes, pidProceso);
    log_ejecucion_fwrite(pcbEnEjecucion, nombreArchivo, punteroArchivo, direccionFisica, cantidadBytes);
    return;
}
