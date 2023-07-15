#include <kernel-pcb.h>

// Funciones privadas

// Calcula el valor del hrrn segun el tiempo actual pasado por parametro
static double __calcular_valor_hrrn(t_pcb *pcb, timestamp *tiempoActual)
{
    double estimadoProxRafaga = pcb_get_estimado_prox_rafaga(pcb);
    timestamp *tiempoLlegadaReady = pcb_get_tiempo_llegada_ready(pcb);
    double tiempoEnReady = obtener_diferencial_de_tiempo_en_milisegundos(tiempoActual, tiempoLlegadaReady);
    
    return ( 1.0 + (tiempoEnReady / estimadoProxRafaga) );
}

// Funciones publicas

t_pcb *crear_pcb(uint32_t pid)
{
    t_pcb *pcb = malloc(sizeof(*pcb));
    
    pcb->pid = pid;
    pcb->instrucciones = NULL;
    pcb->programCounter = 0;
    pcb->registrosCpu = registros_cpu_create();
    pcb->estimadoProxRafaga = kernel_config_get_estimacion_inicial(kernelConfig); // el valor inicial se saca del config y después se calcula
    pcb->tablaSegmentos = NULL;
    pcb->archivosAbiertos = dictionary_create();
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
    
    pcb->dispositivoIoEnUso = NULL;

    /*
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

    t_info_segmentos **tablaSegmentos = pcb->tablaSegmentos;
    if (tablaSegmentos != NULL) {
        destruir_tabla_segmentos(tablaSegmentos, pcb_get_tamanio_tabla_segmentos(pcb));
    }

    t_dictionary *archivosAbiertos = pcb->archivosAbiertos;
    if (archivosAbiertos != NULL) {
        destruir_diccionario_semaforos_recursos(archivosAbiertos);
    }

    char *dispositivoIO = pcb->dispositivoIoEnUso;
    if (dispositivoIO != NULL) {
        free(dispositivoIO);
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

t_dictionary *pcb_get_archivos_abiertos(t_pcb *pcb)
{
    return pcb->archivosAbiertos;
}

// Get y Set tabla segmentos

t_info_segmentos **pcb_get_tabla_segmentos(t_pcb *pcb)
{
    return pcb->tablaSegmentos;
}

void pcb_set_tabla_segmentos(t_pcb *pcb, t_info_segmentos **nuevaTablaSegmentos)
{
    if(pcb->tablaSegmentos != NULL){
    destruir_tabla_segmentos(pcb->tablaSegmentos, pcb->tamanioTablaSegmentos);
    }

    // Realizar una copia profunda de la tabla de segmentos
    pcb->tablaSegmentos = malloc(sizeof(t_info_segmentos *) * pcb->tamanioTablaSegmentos);
    for (int i = 0; i < pcb->tamanioTablaSegmentos; i++) {
        pcb->tablaSegmentos[i] = malloc(sizeof(t_info_segmentos));
        pcb->tablaSegmentos[i]->idSegmento = nuevaTablaSegmentos[i]->idSegmento;
        pcb->tablaSegmentos[i]->direccionBase = nuevaTablaSegmentos[i]->direccionBase;
        pcb->tablaSegmentos[i]->tamanio = nuevaTablaSegmentos[i]->tamanio;
    }

    // Liberar memoria para nuevaTablaSegmentos
    for (int i = 0; i < pcb->tamanioTablaSegmentos; i++) {
        free(nuevaTablaSegmentos[i]);
    }
    
    free(nuevaTablaSegmentos);

    return;
}


// Get y Set tamanio tabla segmentos

uint32_t pcb_get_tamanio_tabla_segmentos (t_pcb *pcb)
{
    return pcb->tamanioTablaSegmentos;
}

void pcb_set_tamanio_tabla_segmentos (t_pcb *pcb, uint32_t tamanioTablaSegmentos) 
{
    pcb->tamanioTablaSegmentos = tamanioTablaSegmentos;
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

// Funciones para estados y planificacion

// Funcion auxiliar para hayar el pcb con maximo hrrn en la cola de ready
void *comparar_pcb_segun_hrrn(void *pcbA, void *pcbB)
{
    t_pcb *pcb1 = (t_pcb *) pcbA;
    t_pcb *pcb2 = (t_pcb *) pcbB;

    timestamp *tiempoActual = malloc(sizeof(*tiempoActual));
    set_timespec(tiempoActual);

    double estimacionPcb1 = __calcular_valor_hrrn(pcb1, tiempoActual);
    double estimacionPcb2 = __calcular_valor_hrrn(pcb2, tiempoActual);

    free(tiempoActual);

    return estimacionPcb1 >= estimacionPcb2 ? pcbA : pcbB;
}

void pcb_estimar_proxima_rafaga(t_pcb *pcbEjecutado, double tiempoRealEjecutadoEnCpu)
{
    double alfaHrrn = kernel_config_get_hrrn_alfa(kernelConfig);

    double estimadoProxRafagaPcb = pcb_get_estimado_prox_rafaga(pcbEjecutado);
    double estimadoProxRafagaActualizado = alfaHrrn * tiempoRealEjecutadoEnCpu + (1.0 - alfaHrrn) * estimadoProxRafagaPcb;
    pcb_set_estimado_prox_rafaga(pcbEjecutado, estimadoProxRafagaActualizado);
    return;
}

// Set y Get dispositivo IO en uso
char *pcb_get_dispositivoIO(t_pcb* pcb) 
{
    return pcb->dispositivoIoEnUso;
}

void pcb_set_dispositivoIO(t_pcb* pcb, char* dispositivoIo) 
{
     pcb->dispositivoIoEnUso = strdup(dispositivoIo);
}
