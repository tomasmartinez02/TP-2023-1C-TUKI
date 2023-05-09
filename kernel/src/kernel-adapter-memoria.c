#include <kernel-adapter-memoria.h>

//Variables locales

static pthread_mutex_t mutexSocketMemoria;

// Funciones privadas

// Envia el pid del proceso que se quiere inicializar en memoria
void __enviar_pid_proceso_a_inicializar(t_pcb *pcbAInicializar, int socketMemoria)
{
    uint32_t pid = pcb_get_pid(pcbAInicializar);
    t_buffer *bufferPidAProceso = buffer_create();
    buffer_pack(bufferPidAProceso, (void*) &pid, sizeof(pid));
    stream_send_buffer(socketMemoria, HEADER_solicitud_inicializacion_proceso, bufferPidAProceso); // Ojo que hay que agregar este header! HEADER_solicitud_inicializacion_proceso
    buffer_destroy(bufferPidAProceso);

    log_info(kernelDebuggingLogger, "Se ha enviado el pid %d para que la memoria inicalice sus estructuras", pid);

    return;
}

// Recibe y desempaqueta la tabla de segmentos enviada por la memoria
static t_buffer *__recibir_tabla_segmentos(t_pcb *pcbInicializado, int socketMemoria)
{
    t_buffer *bufferTablaSegmentos = buffer_create();
    stream_recv_buffer(socketMemoria, bufferTablaSegmentos);

    log_info(kernelDebuggingLogger, "Proceso: %d - Se recibe el array con la tabla de los segmentos", pcb_get_pid(pcbInicializado));

    return bufferTablaSegmentos;
}

// Recibe la base y tamanio de un segmento y los agrega a la tabla de segmentos en su posicion correspondiente segun su id
static void __agregarSegmentoATabla(uint32_t base, uint32_t tamanio, uint32_t idSegmento, t_info_segmentos **tablaSegmentos)
{   
    t_info_segmentos *segmento = tablaSegmentos[idSegmento];
    info_segmentos_set_direccion_base(segmento, base);
    info_segmentos_set_tamanio(segmento, tamanio);

    return; 
}

// Funciones publicas

t_buffer *adapter_memoria_pedir_inicializacion_proceso(t_pcb *pcbAInicializar) // REVISAR ESTA FUNCION!!!!
{
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    pthread_mutex_lock(&mutexSocketMemoria);
    __enviar_pid_proceso_a_inicializar(pcbAInicializar, socketMemoria);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    if (respuestaMemoria != HEADER_tabla_segmentos) {
        if (respuestaMemoria == HEADER_error) { // En que caso nos mandaria eso?
            stream_recv_empty_buffer(socketMemoria);
            pthread_mutex_unlock(&mutexSocketMemoria);
            return NULL;
        }
        else {
            log_error(kernelLogger, "Error al recibir la tabla de segmentos");
            exit(EXIT_FAILURE);
        }
    }

    uint32_t tamanio_tabla_segmentos;
    buffer_unpack(tablaSegmentos, &tamanio_tabla_segmentos, sizeof(tamanio_tabla_segmentos));
    pcb_set_tamanio_tabla_segmentos(pcbAInicializar, tamanio_tabla_segmentos);

    t_buffer *tablaSegmentos = __recibir_tabla_segmentos(pcbAInicializar, socketMemoria);
    pthread_mutex_unlock(&mutexSocketMemoria);

    return tablaSegmentos;
}

void adapter_memoria_pedir_creacion_segmento(uint32_t idSegmento, uint32_t tamanio, t_pcb* pcb)
{   
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    pthread_mutex_lock(&mutexSocketMemoria);
    __enviar_info_segmento_a_crear(idSegmento, tamanio, pid, socketMemoria);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    switch(respuestaMemoria)
    {
        case HEADER_segmento_creado:
        {
            t_buffer *bufferBaseNuevoSegmento;
            stream_recv_buffer(socketMemoria, bufferBaseNuevoSegmento);
            uint32_t baseNuevoSegmento;
            buffer_unpack(bufferBaseNuevoSegmento, &baseNuevoSegmento, sizeof(baseNuevoSegmento));
            buffer_destroy(bufferBaseNuevoSegmento);     

            t_buffer *bufferTablaSegmentos = pcb_get_tabla_segmentos(pcb);       
            t_info_segmentos *tablaSegmentos= desempaquetar_tabla_segmentos(bufferTablaSegmentos, pcb_get_tamanio_tabla_segmentos(pcb)); 
            buffer_destroy(bufferTablaSegmentos);
            
            __agregarSegmentoATabla(baseNuevoSegmento, tamanio, tablaSegmentos); 

            t_buffer *bufferTablaSegmentosActualizada = empaquetar_tabla_segmentos(tablaSegmentos);
            pcb_set_tabla_segmentos(pcb, bufferTablaSegmentosActualizada);

            // Crear log obligatorio en utils
            log_info(kernelLogger, "PID: %d - Crear Segmento - Id: %d - Tamaño: %d", pcb_get_pid(pcb), idSegmento, tamanio);
            log_info(kernelDebuggingLogger, "PID: %d - Crear Segmento - Id: %d - Tamaño: %d", pcb_get_pid(pcb), idSegmento, tamanio);

            break;
        }

        case HEADER_out_of_memory:
        {
            // Acá hay que marcar un error
            break;
        }

        case HEADER_necesita_compactacion:
        {
            // Aca se deberia compactar la memoria y despues crear el segmento
            break;
        }
        
        default:
        {   
            log_error(kernelDebuggingLogger, "Error al intentar crear el segmento solicitado");
            exit(EXIT_FAILURE);
            // Aca deberiamos loguear un error
            break;
        }
    }

    pthread_mutex_unlock(&mutexSocketMemoria);

    return;
}


void adapter_memoria_finalizar_proceso(t_pcb *pcbATerminar)
{   
    /*
    if (pcb_get_estado_finalizacion(pcbATerminar)) {
        
        pthread_mutex_lock(&mutexSocketMemoria);


        uint32_t pid = pcb_get_pid(pcbATerminar);
        uint32_t tamanioPid = sizeof(pid);
        int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

        t_buffer *bufferPidATerminar = buffer_create();
        buffer_pack(bufferPidATerminar, (void*) &pid, tamanioPid);
        stream_send_buffer(socketMemoria, HEADER_proceso_terminado, bufferPidATerminar); // Ojo que hay que agregar este header!
        buffer_destroy(bufferPidATerminar);

        uint32_t respuestaMemoria = stream_recv_header(kernel_config_get_socket_memoria(kernelConfig));

        stream_recv_empty_buffer(kernel_config_get_socket_memoria(kernelConfig));
            
        if (respuestaMemoria == HANDSHAKE_ok_continue) {    
            log_info(kernelLogger, "Proceso %d finalizado correctamente en Memoria", pcb_get_pid(pcbATerminar)); // Despues ver bien este log
        } 
        else {   
            log_error(kernelLogger, "Error al finalizar proceso en Memoria");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&mutexSocketMemoria);
    
    } else {
            log_error(kernelLogger, "El proceso con PID <%d> finalizado no pudo ser creado por falta de memoria", pcb_get_pid(pcbATerminar));
    }
    */
   return;
}
