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

    intervalo_de_pausa(tiempo);
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

    pthread_create(&ejecutarTiempoIO, NULL, sleepHilo, (void*) parametrosHilo);
    pthread_detach(ejecutarTiempoIO);
    return;
}

// El proceso solicita que se le asigne una instancia del recurso indicado por parámetro.
void ejecutar_instruccion_wait(t_pcb *pcbEnEjecucion, char *nombreRecurso)
{   
    t_dictionary *diccionarioSemaforosRecursos = kernel_config_get_diccionario_semaforos(kernelConfig);
    if (!diccionario_semaforos_recursos_existe_recurso(diccionarioSemaforosRecursos, nombreRecurso))
    {
        log_error(kernelLogger, "El recurso solicitado no existe");
        log_error(kernelDebuggingLogger, "El recurso solicitado no existe");
        pcb_pasar_de_running_a_exit_public(pcbEnEjecucion);
    }
    t_semaforo_recurso *semaforoRecurso = diccionario_semaforos_recursos_get_semaforo_recurso(diccionarioSemaforosRecursos, nombreRecurso);
    uint32_t instanciasRecurso = semaforo_recurso_get_instancias(semaforoRecurso);

    semaforo_recurso_wait(semaforoRecurso);
    if (semaforo_recurso_debe_bloquear_proceso(semaforoRecurso))
    {
        semaforo_recurso_bloquear_proceso(semaforoRecurso, pcbEnEjecucion);
    }
    
    log_ejecucion_wait(pcbEnEjecucion, nombreRecurso, instanciasRecurso);
    return;
}

// El proceso solicita que se libere una instancia del recurso indicado por parámetro.
void ejecutar_instruccion_signal(t_pcb *pcbEnEjecucion, char *nombreRecurso)
{   
    t_dictionary *diccionarioSemaforosRecursos = kernel_config_get_diccionario_semaforos(kernelConfig);
    if (!diccionario_semaforos_recursos_existe_recurso(diccionarioSemaforosRecursos, nombreRecurso))
    {
        log_error(kernelLogger, "El recurso solicitado no existe");
        log_error(kernelDebuggingLogger, "El recurso solicitado no existe");
        pcb_pasar_de_running_a_exit_public(pcbEnEjecucion);
    }
    t_semaforo_recurso *semaforoRecurso = diccionario_semaforos_recursos_get_semaforo_recurso(diccionarioSemaforosRecursos, nombreRecurso);
    uint32_t instanciasRecurso = semaforo_recurso_get_instancias(semaforoRecurso);

    semaforo_recurso_post(semaforoRecurso);
    log_ejecucion_signal(pcbEnEjecucion, nombreRecurso, instanciasRecurso);

    if(semaforo_recurso_debe_desbloquear_recurso(semaforoRecurso))
    {
        t_pcb *pcbAEjecutar = semaforo_recurso_desbloquear_primer_proceso_bloqueado(semaforoRecurso);
        // Desbloquea al primer proceso de la cola de bloqueados del recurso 
        pcb_pasar_de_blocked_a_ready_public(pcbAEjecutar);
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
        
    }
    
    // NO SE SI HACE FALTA hacer un ELSE O SI YA CUANDO EL RECURSO SE BLOQUEA YA DEJA DE EJECUTAR LA FUNCION X ENDE NO EJECUTARIA LO SIGUIENTE
 
    if (archivo_existe_en_filesystem(nombreArchivo))
    {
        abrir_archivo(nombreArchivo);
    }
    else
    {
        pedir_creacion_archivo_a_filesystem(nombreArchivo);
        abrir_archivo(nombreArchivo);
    }

    // Agrego archivo a la tabla de archivos abiertos del proceso con el puntero en la posición 0. 
    abrir_archivo_en_tabla_de_pcb(pcbEnEjecucion, nombreArchivo);
    return;
}

void ejecutar_instruccion_fclose(t_pcb *pcbEnEjecucion, char *nombreArchivo)
{   
    cerrar_archivo_en_tabla_de_pcb(pcbEnEjecucion, nombreArchivo);
    return;
}

void ejecutar_instruccion_fseek(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t ubicacionNueva)
{
    return;
}