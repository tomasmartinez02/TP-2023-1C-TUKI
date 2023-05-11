#include <kernel-instrucciones.h>

//  El proceso se bloquea por una cantidad determinada de tiempo.
void* ejecutar_instruccion_io(t_pcb *pcbEnEjecucion, uint32_t tiempo)
{
    pcb_pasar_de_running_a_blocked_public(pcbEnEjecucion);
    sleep(tiempo);
    pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);
    return;
}

// El proceso solicita que se le asigne una instancia del recurso indicado por parámetro.
void *ejecutar_instruccion_wait(t_pcb *pcbEnEjecucion, char *nombreRecurso)
{   
    t_dictionary *diccionarioSemaforosRecursos = kernel_config_get_diccionario_semaforos(kernelConfig);
    t_semaforo_recurso *semaforoRecurso = diccionario_semaforos_recursos_get_semaforo_recurso(diccionarioSemaforosRecursos, nombreRecurso);
    uint32_t instanciasRecurso = semaforo_recurso_get_instancias(semaforoRecurso);

    if (!diccionario_semaforos_recursos_existe_recurso(diccionarioSemaforosRecursos, nombreRecurso))
    {
        log_error(kernelLogger, "El recurso solicitado no existe");
        log_error(kernelDebuggingLogger, "El recurso solicitado no existe");
    }

    semaforo_recurso_wait(semaforoRecurso);

    if (semaforo_recurso_debe_bloquear_proceso(semaforoRecurso))
    {
        semaforo_recurso_bloquear_proceso(semaforoRecurso, pcbEnEjecucion);
    }
    
    log_ejecucion_wait(pcbEnEjecucion, nombreRecurso, instanciasRecurso);
    return;
}

// El proceso solicita que se libere una instancia del recurso indicado por parámetro.
void *ejecutar_instruccion_signal(t_pcb *pcbEnEjecucion, char *nombreRecurso)
{
    t_dictionary *diccionarioSemaforosRecursos = kernel_config_get_diccionario_semaforos(kernelConfig);
    t_semaforo_recurso * semaforoRecurso = diccionario_semaforos_recursos_get_semaforo_recurso(diccionarioSemaforosRecursos, nombreRecurso);
    uint32_t instaciasDisponibles = semaforo_recurso_get_instancias(semaforoRecurso);
    
    if (!diccionario_semaforos_recursos_existe_recurso(diccionarioSemaforosRecursos, nombreRecurso))
    {
        /*__terminar_proceso(pcbEnEjecucion, char *motivoFinalizacion);*/ // la funcion es privada
        log_error(kernelDebuggingLogger, "El recurso solicitado no existe");
    }

    semaforo_recurso_post(semaforoRecurso);

    if(semaforo_recurso_hay_procesos_bloqueados(semaforoRecurso) && instaciasDisponibles > 0)
    {
        semaforo_recurso_desbloquear_primer_proceso_bloqueado(semaforoRecurso);
    }

    log_ejecucion_signal(pcbEnEjecucion, nombreRecurso, instaciasDisponibles);

    // Devolver la ejecucion al proceso que peticiono el SIGNAL

    return;
}