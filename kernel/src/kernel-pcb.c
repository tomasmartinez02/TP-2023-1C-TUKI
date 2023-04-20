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
    pcb->estadoDeFinalizacion = NEW;
    pcb->estadoAnterior = NEW;
    pcb->procesoBloqueadoOTerminado = false;
    //pcb->arrayTablaPaginas = NULL;
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

t_nombre_estado pcb_get_estado_actual(t_pcb* self) 
{
    return self->estadoActual;
}

void pcb_set_estado_actual(t_pcb* self, t_nombre_estado estadoActual)
{
    self->estadoActual = estadoActual;
}

void pcb_set_estado_anterior(t_pcb* self, t_nombre_estado estadoAnterior)
{
    self->estadoAnterior = estadoAnterior;
}

t_nombre_estado pcb_get_estado_anterior(t_pcb* self) 
{
    return self->estadoAnterior;
}

void pcb_set_estado_de_finalizacion(t_pcb* self, t_nombre_estado estadoDeFin)
{
    self->estadoDeFinalizacion = estadoDeFin;
}

t_nombre_estado pcb_get_estado_finalizacion(t_pcb* self) 
{
    return self->estadoDeFinalizacion;
}

bool pcb_es_este_pcb_por_pid(t_pcb* unPcb, t_pcb* otroPcb)
{
    return pcb_get_pid(unPcb) == pcb_get_pid(otroPcb);
}

bool pcb_es_proceso_bloqueado_o_terminado(t_pcb* self)
{   
    return self->procesoBloqueadoOTerminado;
}

void pcb_set_proceso_bloqueado_o_terminado(t_pcb* self, bool procesoBloqueadoOTerminado)
{
    self->procesoBloqueadoOTerminado = procesoBloqueadoOTerminado;
}

/*
t_registros_cpu* pcb_get_registros_cpu(t_pcb* pcb)
{
    return pcb->registrosCpu;
}

void* pcb_set_registros_cpu(t_pcb* pcb, t_registros_cpu* registrosCpu)
{
    pcb->registrosCpu = registrosCpu;
}

uint32_t pcb_get_registro_ax_cpu(t_pcb* self)
{
    return pcb_get_registros_cpu(self)->registroAx;
}

uint32_t pcb_get_registro_bx_cpu(t_pcb* self)
{
    return pcb_get_registros_cpu(self)->registroBx;
}

uint32_t pcb_get_registro_cx_cpu(t_pcb* self)
{
    return pcb_get_registros_cpu(self)->registroCx;
}

uint32_t pcb_get_registro_dx_cpu(t_pcb* self)
{
    return pcb_get_registros_cpu(self)->registroDx;
}

void pcb_set_registro_ax_cpu(t_pcb* self, uint32_t registro)
{
    self->registrosCpu->registroAx = registro;
}
void pcb_set_registro_bx_cpu(t_pcb* self, uint32_t registro)
{
    self->registrosCpu->registroBx = registro;
}
void pcb_set_registro_cx_cpu(t_pcb* self, uint32_t registro)
{
    self->registrosCpu->registroCx = registro;
}
void pcb_set_registro_dx_cpu(t_pcb* self, uint32_t registro)
{
    self->registrosCpu->registroDx = registro;
}

*/ 