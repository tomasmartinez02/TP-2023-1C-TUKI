#include <kernel-pcb.h>

t_pcb* pcb_create(uint32_t pid, uint32_t* arraySegmentos) 
{
    t_pcb* pcb = malloc(sizeof(*pcb));
    pcb->pid = pid;
    pcb->instructionsBuffer = NULL;
    pcb->programCounter = 0;
    pcb->registrosCpu = NULL;
    pcb->estimadoProxRafaga = 0;
    pcb->tiempoLlegadaReady = 0;
    //pcb->tablaSegmentos = NULL;
    //pcb->archivosAbiertos = NULL;
    pcb->estadoActual = NEW;
    pcb->estadoDeFinalizacion = NEW;
    pcb->estadoAnterior = NEW;
    pcb->procesoBloqueadoOTerminado = false;

    /*
    pcb->arrayTablaPaginas = NULL;
    pcb->socketProceso = 0;
    pcb->dispositivoIoEnUso = NULL;
    pcb->cantidadUnidadesTiemposIo = 0;
    pcb->registroUsadoEnIo = REGISTRO_null;
    pcb->mutex = malloc(sizeof(*(pcb->mutex)));
    pthread_mutex_init(pcb->mutex, NULL);
    */
    return pcb;
}


void pcb_destroy(t_pcb* self) 
{
    if (self->instructionsBuffer != NULL) {
        
        buffer_destroy(self->instructionsBuffer);
    }

    if (self->registrosCpu != NULL) {
        
        free(self->registrosCpu);
    }

    /*
    if(self->dispositivoIoEnUso != NULL) {
        
        free(self->dispositivoIoEnUso);
    }

    close(self->socketProceso);
    pthread_mutex_unlock(pcb_get_mutex(self));

    pthread_mutex_destroy(self->mutex);
    free(self->mutex);
    */

    free(self);
}


uint32_t pcb_get_pid(t_pcb* self) 
{
    return self->pid;
}

