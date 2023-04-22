#include <kernel-pcb.h>

t_pcb *crear_pcb(uint32_t pid)
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

t_info_segmentos *crear_info_segmentos(void)
{
    t_info_segmentos *infoSegmentos = malloc(sizeof(*infoSegmentos));
    infoSegmentos->id = 0;
    infoSegmentos->direccionBase = 0;
    infoSegmentos->tamanio = 0;

    return infoSegmentos;
}

void destruir_info_segmentos(t_info_segmentos *infoSegmentos)
{
    free(infoSegmentos);
}

t_info_archivos *crear_info_archivos(void) 
{
    t_info_archivos *infoArchivos = malloc(sizeof(*infoArchivos));
    infoArchivos->nombreArchivo = NULL;
    infoArchivos->posicionPuntero = 0;

    return infoArchivos;
}

void destruir_info_archivos(t_info_archivos *infoArchivos)
{
    if (infoArchivos->nombreArchivo != NULL) {

        free(infoArchivos->nombreArchivo);
    }
    
    free(infoArchivos);
}

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

uint32_t pcb_get_estimado_prox_rafaga (t_pcb *pcb)
{
    return pcb->estimadoProxRafaga;
}

void pcb_set_estimado_prox_rafaga (t_pcb *pcb, uint32_t estimadoProxRafaga)
{
    pcb->estimadoProxRafaga = estimadoProxRafaga;
}

// Set y Get Tiempo de Llegada a Ready PCB

uint32_t pcb_get_tiempo_llegada_ready (t_pcb *pcb)
{
    return pcb->tiempoLlegadaReady;
}

void pcb_set_tiempo_llegada_ready (t_pcb *pcb, uint32_t tiempoLlegadaReady)
{
    pcb->tiempoLlegadaReady = tiempoLlegadaReady;
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

void pcb_set_estado_de_finalizacion(t_pcb* pcb, t_nombre_estado estadoDeFin)
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

// Set y Get ID info_segmentos

uint32_t info_segmentos_get_id(t_info_segmentos *infoSegmentos)
{
    return infoSegmentos->id;
}

void info_segmentos_set_id(t_info_segmentos *infoSegmentos, uint32_t id) 
{
    infoSegmentos->id = id;
}

// Set y Get Direccion Base info_segmentos

uint32_t info_segmentos_get_direccion_base(t_info_segmentos *infoSegmentos)
{
    return infoSegmentos->direccionBase;
}

void info_segmentos_set_direccion_base(t_info_segmentos *infoSegmentos, uint32_t direccionBase) 
{
    infoSegmentos->direccionBase = direccionBase;
}

// Set y Get tamanio info_segmentos

uint32_t info_segmentos_get_tamanio(t_info_segmentos *infoSegmentos)
{
    return infoSegmentos->tamanio;
}

void info_segmentos_set_tamanio(t_info_segmentos *infoSegmentos, uint32_t tamanio) 
{
    infoSegmentos->tamanio = tamanio;
}

// Set y Get Nombre info_archivos

char *info_archivos_get_nombre_archivo(t_info_archivos *infoArchivo)
{
    char *nombreArchivo = infoArchivo->nombreArchivo;
    return nombreArchivo != NULL ? string_duplicate(nombreArchivo) : nombreArchivo;
}

void info_archivos_set_nombre_archivo(t_info_archivos *infoArchivo, char *nombreArchivo)
{
    infoArchivo->nombreArchivo = nombreArchivo != NULL ? string_duplicate(nombreArchivo) : nombreArchivo;
}

// Set y Get Posicion del Puntero info_archivos

uint32_t info_archivos_get_posicion_puntero(t_info_archivos *infoArchivo)
{
    return infoArchivo->posicionPuntero;
}

void info_archivos_set_posicion_puntero(t_info_archivos *infoArchivo, uint32_t posicionPuntero)
{
    infoArchivo->posicionPuntero = posicionPuntero;
}