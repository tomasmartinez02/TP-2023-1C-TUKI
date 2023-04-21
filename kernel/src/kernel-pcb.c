#include <kernel-pcb.h>

t_pcb* crear_pcb(uint32_t pid)
{
    t_pcb* pcb = malloc(sizeof(*pcb));
    pcb->pid = pid;
    pcb->instrucciones = NULL;
    pcb->programCounter = 0;
    pcb->registrosCpu = NULL;
    pcb->estimadoProxRafaga = kernel_config_get_estimacion_inicial(kernelConfig); // el valor inicial se saca del config y después se calcula
    pcb->tiempoLlegadaReady = 0;
    pcb->tablaSegmentos = NULL;
    pcb->archivosAbiertos = NULL;
    pcb->estadoActual = NEW;
    pcb->estadoDeFinalizacion = NEW;
    pcb->estadoAnterior = NEW;
    pcb->procesoBloqueadoOTerminado = false;
    pcb->socketProceso = -1;
    pcb->mutex = malloc(sizeof(*(pcb->mutex)));
    pthread_mutex_init(pcb->mutex, NULL);
    
    /*
    pcb->dispositivoIoEnUso = NULL;
    pcb->cantidadUnidadesTiemposIo = 0;
    pcb->registroUsadoEnIo = REGISTRO_null;
    
    */
    return pcb;
}

void destruir_pcb(t_pcb* pcb) // Ir viendo que agregar o sacar a medida que termienmos la estrucura del PCB
{
    if (pcb->instrucciones != NULL) {
        
        buffer_destroy(pcb->instrucciones);
    }

    if (pcb->registrosCpu != NULL) {
        
        free(pcb->registrosCpu);
    }

    /*
    if(pcb->dispositivoIoEnUso != NULL) {
        
        free(pcb->dispositivoIoEnUso);
    }*/

    close(pcb->socketProceso);
    pthread_mutex_unlock(pcb_get_mutex(pcb));

    pthread_mutex_destroy(pcb->mutex);
    free(pcb->mutex);

    free(pcb);
}

uint32_t pcb_get_pid(t_pcb* pcb) 
{
    return pcb->pid;
}

t_buffer* pcb_get_instrucciones(t_pcb* pcb) 
{
    return pcb->instrucciones;
}

void pcb_set_instrucciones(t_pcb* pcb, t_buffer* instructionsBuffer) 
{
    pcb->instrucciones = instructionsBuffer;
}

uint32_t pcb_get_program_counter(t_pcb* pcb) 
{
    return pcb->programCounter;
}

void pcb_set_program_counter(t_pcb* pcb, uint32_t programCounter) 
{
    pcb->programCounter = programCounter;
}

uint32_t pcb_get_estimado_prox_rafaga (t_pcb *pcb)
{
    return pcb->estimadoProxRafaga;
}

void pcb_set_estimado_prox_rafaga (t_pcb *pcb, uint32_t estimadoProxRafaga)
{
    pcb->estimadoProxRafaga = estimadoProxRafaga;
}

uint32_t pcb_get_tiempo_llegada_ready (t_pcb *pcb)
{
    return pcb->tiempoLlegadaReady;
}

void pcb_set_tiempo_llegada_ready (t_pcb *pcb, uint32_t tiempoLlegadaReady)
{
    pcb->tiempoLlegadaReady = tiempoLlegadaReady;
}

void pcb_set_socket(t_pcb *pcb, uint32_t socket)
{
    pcb->socketProceso = socket;
}

uint32_t pcb_get_socket(t_pcb *pcb)
{
    return pcb->socketProceso;
}

t_nombre_estado pcb_get_estado_actual(t_pcb* pcb) 
{
    return pcb->estadoActual;
}

void pcb_set_estado_actual(t_pcb* pcb, t_nombre_estado estadoActual)
{
    pcb->estadoActual = estadoActual;
}

void pcb_set_estado_anterior(t_pcb* pcb, t_nombre_estado estadoAnterior)
{
    pcb->estadoAnterior = estadoAnterior;
}

t_nombre_estado pcb_get_estado_anterior(t_pcb* pcb) 
{
    return pcb->estadoAnterior;
}

void pcb_set_estado_de_finalizacion(t_pcb* pcb, t_nombre_estado estadoDeFin)
{
    pcb->estadoDeFinalizacion = estadoDeFin;
}

t_nombre_estado pcb_get_estado_finalizacion(t_pcb* pcb) 
{
    return pcb->estadoDeFinalizacion;
}

bool pcb_es_este_pcb_por_pid(t_pcb* unPcb, t_pcb* otroPcb)
{
    return pcb_get_pid(unPcb) == pcb_get_pid(otroPcb);
}

bool pcb_es_proceso_bloqueado_o_terminado(t_pcb* pcb)
{   
    return pcb->procesoBloqueadoOTerminado;
}

void pcb_set_proceso_bloqueado_o_terminado(t_pcb* pcb, bool procesoBloqueadoOTerminado)
{
    pcb->procesoBloqueadoOTerminado = procesoBloqueadoOTerminado;
}

t_registros_cpu* pcb_get_registros_cpu(t_pcb* pcb)
{
    return pcb->registrosCpu;
}

void pcb_set_registros_cpu(t_pcb *pcb, t_registros_cpu *registrosCpu)
{
    pcb->registrosCpu = registrosCpu;
}