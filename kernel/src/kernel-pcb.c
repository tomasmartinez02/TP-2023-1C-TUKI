#include <kernel-pcb.h>

// Funciones publicas

t_pcb *crear_pcb(uint32_t pid)
{
    t_pcb *pcb = malloc(sizeof(*pcb));
    
    pcb->pid = pid;
    pcb->instrucciones = NULL;
    pcb->programCounter = 0;
    pcb->registrosCpu = NULL;
    pcb->estimadoProxRafaga = kernel_config_get_estimacion_inicial(kernelConfig); // el valor inicial se saca del config y después se calcula
    pcb->tablaSegmentos = NULL;
    pcb->archivosAbiertos = list_create();
    pcb->estadoActual = NEW;
    pcb->estadoDeFinalizacion = NEW;
    pcb->estadoAnterior = NEW;
    pcb->procesoBloqueadoOTerminado = false;
    pcb->socketProceso = -1;

    timestamp *tiempoLlegadaReady = malloc(sizeof(*(tiempoLlegadaReady)));
    pcb->tiempoLlegadaReady = tiempoLlegadaReady;

    pthread_mutex_t *mutex = malloc(sizeof(*(pcb->mutex)));
    pthread_mutex_init(mutex, NULL);
    pcb->mutex = mutex;
    
    /*
    pcb->dispositivoIoEnUso = NULL;
    pcb->cantidadUnidadesTiemposIo = 0;
    pcb->registroUsadoEnIo = REGISTRO_null;
    
    */
    return pcb;
}

void destruir_pcb(t_pcb* pcb) // Ir viendo que agregar o sacar a medida que termienmos la estrucura del PCB
{
    pthread_mutex_lock(pcb_get_mutex(pcb));
    
    t_buffer *instrucciones = pcb->instrucciones;
    if (instrucciones != NULL) {
        buffer_destroy(instrucciones);
    }

    t_registros_cpu *registrosCpu = pcb->registrosCpu;
    if (registrosCpu != NULL) {
        registros_cpu_destroy(registrosCpu);
    }

    t_buffer *tablaSegmentos = pcb->tablaSegmentos;
    if (tablaSegmentos != NULL) {
        buffer_destroy(tablaSegmentos);
    }

    t_list *archivosAbiertos = pcb->archivosAbiertos;
    if (archivosAbiertos != NULL) {
        destruir_lista_archivos_abiertos(archivosAbiertos);
    }

    free(pcb->tiempoLlegadaReady);

    close(pcb->socketProceso);
    pthread_mutex_unlock(pcb_get_mutex(pcb));

    pthread_mutex_destroy(pcb->mutex);
    free(pcb->mutex);

    free(pcb);
}

// Setters y getters pcb

uint32_t pcb_get_pid(t_pcb* pcb) 
{
    return pcb->pid;
}

// Set y Get Instrucciones PCB

t_buffer* pcb_get_instrucciones(t_pcb* pcb) 
{
    return pcb->instrucciones;
}

void pcb_set_instrucciones(t_pcb* pcb, t_buffer* instructionsBuffer) 
{
    pcb->instrucciones = instructionsBuffer;
}

// Set y Get Program Counter PCB

uint32_t pcb_get_program_counter(t_pcb* pcb) 
{
    return pcb->programCounter;
}

void pcb_set_program_counter(t_pcb* pcb, uint32_t programCounter) 
{
    pcb->programCounter = programCounter;
}

// Set y Get Estimado de la Proxima Rafaga PCB

double pcb_get_estimado_prox_rafaga(t_pcb *pcb)
{
    return pcb->estimadoProxRafaga;
}

void pcb_set_estimado_prox_rafaga(t_pcb *pcb, double estimadoProxRafaga)
{
    pcb->estimadoProxRafaga = estimadoProxRafaga;
}

// Getter y setter archivosAbiertos

t_list *pcb_get_archivos_abiertos(t_pcb *pcb)
{
    return pcb->archivosAbiertos;
}

// Get y Set tabla segmentos

t_buffer *pcb_get_tabla_segmentos(t_pcb *pcb)
{
    return pcb->tablaSegmentos;
}

void pcb_set_tabla_segmentos(t_pcb *pcb, t_buffer *tablaSegmentos)
{
    t_buffer *tablaSegmentosActual = pcb->tablaSegmentos;
    if (tablaSegmentosActual != NULL) {
        buffer_destroy(tablaSegmentos);
    }

    pcb->tablaSegmentos = tablaSegmentos;

    return;
}

// Set y Get Tiempo de Llegada a Ready PCB

timestamp *pcb_get_tiempo_llegada_ready(t_pcb *pcb)
{
    return pcb->tiempoLlegadaReady;
}

void pcb_set_tiempo_llegada_ready(t_pcb *pcb)
{
    set_timespec(pcb->tiempoLlegadaReady);
}

// Set y Get Socket PCB

void pcb_set_socket(t_pcb *pcb, uint32_t socket)
{
    pcb->socketProceso = socket;
}

uint32_t pcb_get_socket(t_pcb *pcb)
{
    return pcb->socketProceso;
}

// Set y Get Estado Actual PCB

t_nombre_estado pcb_get_estado_actual(t_pcb* pcb) 
{
    return pcb->estadoActual;
}

void pcb_set_estado_actual(t_pcb* pcb, t_nombre_estado estadoActual)
{
    pcb->estadoActual = estadoActual;
}

// Set y Get Estado Anterior PCB

void pcb_set_estado_anterior(t_pcb* pcb, t_nombre_estado estadoAnterior)
{
    pcb->estadoAnterior = estadoAnterior;
}

t_nombre_estado pcb_get_estado_anterior(t_pcb* pcb) 
{
    return pcb->estadoAnterior;
}

// Set y Get Estado de Finalizacion PCB

void pcb_set_estado_finalizacion(t_pcb* pcb, t_nombre_estado estadoDeFin)
{
    pcb->estadoDeFinalizacion = estadoDeFin;
}

t_nombre_estado pcb_get_estado_finalizacion(t_pcb* pcb) 
{
    return pcb->estadoDeFinalizacion;
}

bool pcb_es_este_pcb_por_pid(void *unPcb, void *otroPcb)
{
    return pcb_get_pid((t_pcb *) unPcb) == pcb_get_pid((t_pcb *) otroPcb);
}

bool pcb_es_proceso_bloqueado_o_terminado(t_pcb* pcb)
{   
    return pcb->procesoBloqueadoOTerminado;
}

void pcb_set_proceso_bloqueado_o_terminado(t_pcb* pcb, bool procesoBloqueadoOTerminado)
{
    pcb->procesoBloqueadoOTerminado = procesoBloqueadoOTerminado;
}

// Set y Get Registros PCB

t_registros_cpu* pcb_get_registros_cpu(t_pcb* pcb)
{
    return pcb->registrosCpu;
}

void pcb_set_registros_cpu(t_pcb *pcb, t_registros_cpu *registrosCpu)
{
    pcb->registrosCpu = registrosCpu;
}

// Get mutex PCB
pthread_mutex_t* pcb_get_mutex(t_pcb* pcb)
{
    return pcb->mutex;
}

