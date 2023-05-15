#include <kernel-instrucciones.h>
// Utils
void* sleepHilo(void* tiempoIo) {
    uint32_t tiempo = *(uint32_t*) tiempoIo;
    sleep(tiempo);
    return NULL;
}

//  El proceso se bloquea por una cantidad determinada de tiempo.
void ejecutar_instruccion_io(t_pcb *pcbEnEjecucion, uint32_t tiempo)
{
    pthread_t ejecutarTiempoIO;
    pcb_pasar_de_running_a_blocked_public(pcbEnEjecucion);

    pthread_create(&ejecutarTiempoIO, NULL, sleepHilo, (void*) &tiempo);
    pthread_detach(ejecutarTiempoIO);

    pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);
    return;
}

// El proceso solicita que se le asigne una instancia del recurso indicado por parámetro.
void ejecutar_instruccion_wait(t_pcb *pcbEnEjecucion, char *nombreRecurso)
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
void ejecutar_instruccion_signal(t_pcb *pcbEnEjecucion, char *nombreRecurso)
{   
    t_pcb *pcbAEjecutar;
    t_dictionary *diccionarioSemaforosRecursos = kernel_config_get_diccionario_semaforos(kernelConfig);
    t_semaforo_recurso * semaforoRecurso = diccionario_semaforos_recursos_get_semaforo_recurso(diccionarioSemaforosRecursos, nombreRecurso);
    uint32_t instaciasDisponibles = semaforo_recurso_get_instancias(semaforoRecurso);
    
    if (!diccionario_semaforos_recursos_existe_recurso(diccionarioSemaforosRecursos, nombreRecurso))
    {
        pcb_pasar_de_running_a_exit_public(pcbEnEjecucion);
        log_error(kernelDebuggingLogger, "El recurso solicitado no existe.");
    }

    semaforo_recurso_post(semaforoRecurso);
    log_ejecucion_signal(pcbEnEjecucion, nombreRecurso, instaciasDisponibles);

    if(semaforo_recurso_hay_procesos_bloqueados(semaforoRecurso) && instaciasDisponibles > 0)
    {
        pcbAEjecutar = semaforo_recurso_desbloquear_primer_proceso_bloqueado(semaforoRecurso);
        // Desbloquea al primer proceso de la cola de bloqueados del recurso 
        pcb_pasar_de_blocked_a_ready_public(pcbAEjecutar);
    }

     // Seguir la ejecucion del proceso que peticiono el SIGNAL
    return;
}