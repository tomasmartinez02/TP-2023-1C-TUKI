#include <kernel-adapter-memoria.h>

//Variables locales

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
static t_info_segmentos **__recibir_tabla_segmentos(t_pcb *pcbInicializado, int socketMemoria)
{
    t_buffer *bufferTablaSegmentos = buffer_create();
    stream_recv_buffer(socketMemoria, bufferTablaSegmentos);

    uint32_t tamanioTablaSegmentos;
    buffer_unpack(bufferTablaSegmentos, &tamanioTablaSegmentos, sizeof(tamanioTablaSegmentos));

    pcb_set_tamanio_tabla_segmentos(pcbInicializado, tamanioTablaSegmentos);

    t_info_segmentos **tablaSegmentos;
    tablaSegmentos = desempaquetar_tabla_segmentos(bufferTablaSegmentos, pcb_get_tamanio_tabla_segmentos(pcbInicializado));

    log_info(kernelDebuggingLogger, "Proceso: %d - Se recibe el array con la tabla de los segmentos", pcb_get_pid(pcbInicializado));

    buffer_destroy(bufferTablaSegmentos);

    return tablaSegmentos;
}

// Recibe la base y tamanio de un segmento y los agrega a la tabla de segmentos en su posicion correspondiente segun su id
static void __agregarSegmentoATabla(uint32_t baseSegmentoCreado, uint32_t tamanioSegmentoCreado, uint32_t idSegmentoCreado, t_info_segmentos **tablaSegmentos)
{   
    uint32_t indice = 0;

    while (tablaSegmentos[indice]->idSegmento != -1) {
        indice = indice + 1;
    }

    tablaSegmentos[indice]->direccionBase = baseSegmentoCreado;
    tablaSegmentos[indice]->idSegmento = idSegmentoCreado;
    tablaSegmentos[indice]->tamanio = tamanioSegmentoCreado;

    return; 
}

static void __enviar_info_segmento_a_crear (uint32_t idSegmento, uint32_t tamanio, uint32_t pid, uint32_t socketMemoria)
{
    t_buffer *bufferAEnviar = buffer_create();

    uint32_t id = idSegmento;
    buffer_pack(bufferAEnviar, &id, sizeof(id));

    uint32_t tamanioSegmento = tamanio;
    buffer_pack(bufferAEnviar, &tamanioSegmento, sizeof(tamanioSegmento));

    uint32_t pidProceso = pid;
    buffer_pack(bufferAEnviar, &pidProceso, sizeof(pidProceso));

    stream_send_buffer(socketMemoria, HEADER_crear_segmento, bufferAEnviar); 
    buffer_destroy(bufferAEnviar);

    return;
}

static void __enviar_pedido_compactacion(int socketMemoria)
{
    stream_send_empty_buffer(socketMemoria, HEADER_necesita_compactacion); // No se si este header esta creado

    return;
}

static void __enviar_pedido_eliminar_segmento(int socketMemoria, uint32_t idSegmento, uint32_t pid)
{
    t_buffer *bufferAEnviar = buffer_create();

    uint32_t id = idSegmento;
    buffer_pack(bufferAEnviar, &id, sizeof(id));

    uint32_t pidProceso = pid;
    buffer_pack(bufferAEnviar, &pidProceso, sizeof(pidProceso));

    stream_send_buffer(socketMemoria, HEADER_eliminar_segmento, bufferAEnviar); 
    buffer_destroy(bufferAEnviar);

    return;
}


// Funciones publicas

t_info_segmentos **adapter_memoria_pedir_inicializacion_proceso(t_pcb *pcbAInicializar) // REVISAR ESTA FUNCION!!!!
{
    t_info_segmentos **tablaSegmentos;
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
        free(tablaSegmentos);
    }else {
        tablaSegmentos = __recibir_tabla_segmentos(pcbAInicializar, socketMemoria);
    }

    pthread_mutex_unlock(&mutexSocketMemoria);

    return tablaSegmentos;
}

void adapter_memoria_pedir_creacion_segmento(uint32_t idSegmento, uint32_t tamanio, t_pcb* pcb)
{   
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    pthread_mutex_lock(&mutexSocketMemoria);
    __enviar_info_segmento_a_crear(idSegmento, tamanio, pcb_get_pid(pcb), socketMemoria);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    switch(respuestaMemoria)
    {
        case HEADER_segmento_creado:
        {
            t_buffer *bufferBaseNuevoSegmento = buffer_create();
            stream_recv_buffer(socketMemoria, bufferBaseNuevoSegmento);
            uint32_t baseNuevoSegmento;
            buffer_unpack(bufferBaseNuevoSegmento, &baseNuevoSegmento, sizeof(baseNuevoSegmento));
            buffer_destroy(bufferBaseNuevoSegmento);     

            //uint32_t tamanioTablaSegmentos = pcb_get_tamanio_tabla_segmentos(pcb);

            t_info_segmentos **tablaSegmentos = pcb_get_tabla_segmentos(pcb);
            
            __agregarSegmentoATabla(baseNuevoSegmento, tamanio, idSegmento, tablaSegmentos); 

            log_creacion_nuevo_segmento(pcb, idSegmento, tamanio);

            // enviar_pcb_a_cpu(pcb);

            pthread_mutex_unlock(&mutexSocketMemoria);

            seguir_ejecutando(pcb);

            break;
        }

        case HEADER_out_of_memory:
        {
            __pcb_pasar_a_exit(pcb, FINALIZACION_OUTOFMEMORY);
            pthread_mutex_unlock(&mutexSocketMemoria);
            log_error(kernelDebuggingLogger, "No hay memoria suficiente");
            // Acá habria que terminar el proceso
            break;
        }
        
        case HEADER_necesita_compactacion:
        {
            adapter_memoria_pedir_compactacion(socketMemoria);
            pthread_mutex_unlock(&mutexSocketMemoria); //liberamos el socket porque como llamamos 
            adapter_memoria_pedir_creacion_segmento(idSegmento, tamanio, pcb);
            break;
        }
        
        default:
        {   
            log_error(kernelDebuggingLogger, "Error al intentar crear el segmento solicitado");
            pthread_mutex_unlock(&mutexSocketMemoria);
            exit(EXIT_FAILURE);
            // Aca deberiamos loguear un error
            break;
        }
    }

    return;
}

void adapter_memoria_pedir_compactacion() 
{
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    __enviar_pedido_compactacion(socketMemoria);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    if(respuestaMemoria == HEADER_memoria_compactada)
    { 
        t_buffer *bufferTablaDeSegmentosActualizada = buffer_create();
        stream_recv_buffer(socketMemoria, bufferTablaDeSegmentosActualizada);

        uint32_t tamanioTablaDeSegmentosActualizada; // FINJO DEMENCIA Y SIGO PERO LO VAMOS A CAMBIAR
        buffer_unpack(bufferTablaDeSegmentosActualizada, &tamanioTablaDeSegmentosActualizada, sizeof(tamanioTablaDeSegmentosActualizada));

        t_info_segmentos **tablaNueva = desempaquetar_tabla_segmentos(bufferTablaDeSegmentosActualizada, tamanioTablaDeSegmentosActualizada);

        actualizar_tabla_segmentos(tablaNueva, estado_get_list(estadoReady));
        actualizar_tabla_segmentos(tablaNueva, estado_get_list(estadoBlocked));
        actualizar_tabla_segmentos(tablaNueva, estado_get_list(estadoExecute));
    } else {
        // error
    }

    return;
}

void adapter_memoria_pedir_eliminar_segmento(uint32_t idSegmento, t_pcb* pcb)
{
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    pthread_mutex_lock(&mutexSocketMemoria);
    log_info(kernelLogger, "puede usar el socket");
    __enviar_pedido_eliminar_segmento(socketMemoria, idSegmento, pcb_get_pid(pcb));
    log_info(kernelLogger, "pide la eliminacion del segmento");

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);
    log_info(kernelLogger, "recibe el header");

    if(respuestaMemoria == HEADER_segmento_destruido)
    { 
        t_buffer *bufferTablaDeSegmentosActualizada = buffer_create();
        log_info(kernelLogger, "crea el buffer");
        stream_recv_buffer(socketMemoria, bufferTablaDeSegmentosActualizada);
        log_info(kernelLogger, "recibe el buffer case");
        
        t_info_segmentos ** nuevaTabla = desempaquetar_tabla_segmentos(bufferTablaDeSegmentosActualizada, pcb_get_tamanio_tabla_segmentos(pcb));
        log_info(kernelLogger, "desempaqueta la tabla de segmentos");

        pcb_set_tabla_segmentos(pcb, nuevaTabla); // Ver, porque hay que liberar algo de memoria si o si
        log_info(kernelLogger, "setea la nueva tabla de segmentos");

        seguir_ejecutando(pcb);
        
    } else {
        // error
    }

    pthread_mutex_unlock(&mutexSocketMemoria);

    return ;   
}

void actualizar_tabla_segmentos(t_info_segmentos **tablaDeSegmentosActualizada, t_list *listaProcesos)
{
    void __cambiar_tabla_segmentos(void* pcb)
    {
        pcb_set_tabla_segmentos(pcb, tablaDeSegmentosActualizada);
        return ;
    }

    list_iterate(listaProcesos, __cambiar_tabla_segmentos);

    return ;
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
