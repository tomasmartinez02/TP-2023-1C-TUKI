#include <kernel-pcb.h>

t_pcb* crear_pcb(uint32_t pid)
{
    t_pcb* pcb = malloc(sizeof(*pcb));
    pcb->pid = pid;
    pcb->instrucciones = NULL;
    pcb->programCounter = 0;
    //pcb->registrosCpu = NULL;
    pcb->estimadoProxRafaga = 0; // el valor inicial se saca del config y después se calcula
    pcb->tiempoLlegadaReady = 0;
    //pcb->tablaSegmentos = NULL;
    //pcb->archivosAbiertos = NULL;
    pcb->estadoActual = NEW;
    //pcb->estadoDeFinalizacion = NEW;
    //pcb->estadoAnterior = NEW;
    pcb->procesoBloqueadoOTerminado = false;
    
    
    //pcb->arrayTablaPaginas = NULL;
    pcb->socketProceso = -1;
    /*
    pcb->dispositivoIoEnUso = NULL;
    pcb->cantidadUnidadesTiemposIo = 0;
    pcb->registroUsadoEnIo = REGISTRO_null;
    pcb->mutex = malloc(sizeof(*(pcb->mutex)));
    pthread_mutex_init(pcb->mutex, NULL);
    */
    return pcb;
}

void destruir_pcb(t_pcb* pcb) // HABRIA QUE CHEQUEAR QUE HAY QUE DESCOMENTAR ACA YA QUE AGREGAMOS COSAS A LA ESTRUCTURA DEL PCB
{
    if (pcb->instrucciones != NULL) {
        
        buffer_destroy(pcb->instrucciones);
    }

    /*if (pcb->registrosCpu != NULL) {
        
        free(pcb->registrosCpu);
    }*/

    /*
    if(self->dispositivoIoEnUso != NULL) {
        
        free(self->dispositivoIoEnUso);
    }

    close(self->socketProceso);
    pthread_mutex_unlock(pcb_get_mutex(self));

    pthread_mutex_destroy(self->mutex);
    free(self->mutex);
    */

    free(pcb);
}

uint32_t obtener_siguiente_pid()
{
    // Esta funcion deberia devolver un pcb nuevo que no esté en uso. Chequear que este bien asignarle desde el 1 en adelante. 
    
    pidAnterior++;

    return pidAnterior;
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

/*t_registros_cpu* pcb_get_registros_cpu(t_pcb* pcb)
{
    return pcb->registrosCpu;
}*/

/*void pcb_set_registros_cpu(t_pcb* pcb, t_registros_cpu* registrosCpu)
{
    pcb->registrosCpu = registrosCpu;
}*/

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

void pcb_set_socket(t_pcb *pcb, int socket)
{
    pcb->socketProceso = socket;
}
