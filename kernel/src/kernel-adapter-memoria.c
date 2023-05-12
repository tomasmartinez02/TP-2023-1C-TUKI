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

static void __enviar_info_segmento_a_crear (uint32_t idSegmento, uint32_t tamanio, uint32_t pid, uint32_t socketMemoria)
{
    t_buffer *bufferAEnviar = buffer_create();

    uint32_t id = idSegmento;
    buffer_pack(bufferAEnviar, &id, sizeof(id));

    uint32_t tamanioSegmento = tamanio;
    buffer_pack(bufferAEnviar, &id, sizeof(id));

    uint32_t pidProceso = pid;
    buffer_pack(bufferAEnviar, &pidProceso, sizeof(pidProceso));

    stream_send_buffer(socketMemoria, HEADER_crear_segmento, bufferAEnviar); // Este header hay que crearlo
    buffer_destroy(bufferAEnviar);

    return;
}

static void __enviar_pedido_compactacion(socketMemoria)
{
    stream_send_empty_buffer(socketMemoria, HEADER_necesita_compactacion); // No se si este header esta creado

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

            log_creacion_nuevo_segmento(pcb, idSegmento, tamanio);

            __enviar_pcb_a_cpu(pcb);

            pthread_mutex_unlock(&mutexSocketMemoria);

            break;
        }

        case HEADER_out_of_memory:
        {
            __pcb_pasar_a_exit(pcb, FINALIZACION_OUTOFMEMORY);
            pthread_mutex_unlock(&mutexSocketMemoria);
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

void adapter_memoria_pedir_compactacion(uint32_t socketMemoria) 
{
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    __enviar_pedido_compactacion(socketMemoria);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    if(respuestaMemoria == HEADER_memoria_compactada) // Este header hay que crearlo
    { 
        t_buffer *bufferTablaDeSegmentosActualizada;
        stream_recv_buffer(socketMemoria, bufferTablaDeSegmentosActualizada);

        actualizar_tabla_segmentos(bufferTablaDeSegmentosActualizada, estadoReady->listaProcesos);
        actualizar_tabla_segmentos(bufferTablaDeSegmentosActualizada, estadoBlocked->listaProcesos);
        actualizar_tabla_segmentos(bufferTablaDeSegmentosActualizada, estadoExecute->listaProcesos);
    } else {
        // error
    }

    return;
}

void adapter_memoria_pedir_eliminar_segmento(uint32_t idSegmento, t_pcb* pcb)
{
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    pthread_mutex_lock(&mutexSocketMemoria);
    __enviar_pedido_eliminar_segmento(socketMemoria);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    if(respuestaMemoria == HEADER_segmento_destruido) // Este header hay que crearlo
    { 
        t_buffer *bufferTablaDeSegmentosActualizada;
        stream_recv_buffer(socketMemoria, bufferTablaDeSegmentosActualizada);

        pcb_set_tabla_segmentos(pcb, bufferTablaDeSegmentosActualizada);
        
    } else {
        // error
    }

    return;   
}

void actualizarTablaSegmentos(void* elemento, t_buffer* nuevoValor) {
    t_pcb* pcb = (t_pcb*)elemento;
    // Realiza la actualización del campo tablaSegmentos en el pcb
    pcb->tablaSegmentos = nuevoValor;
}

void actualizarTablasSegmentosEnListaProcesos(t_estado* estado, t_buffer* nuevoValor) {
    list_iterate(estado->listaProcesos, actualizarTablaSegmentos, nuevoValor);
}

void funcion2(void* lista, t_buffer bufferTablaSegmentos) {
    // Realizar operaciones utilizando los parámetros recibidos
    // ...
}

void funcion1(auxiliarParaIterar* aux) {
    myClosure(aux->lista, aux->bufferTablaSegmentos);
}

void actualizar_tabla_segmentos(t_buffer *bufferTablaSegmentos, t_list *listaProcesos)
{   
    auxiliarParaIterar aux;

    aux.lista = listaProcesos;
    aux.bufferTablaSegmentos = bufferTablaSegmentos;

    list_iterate(listaProcesos, funcion1);





    t_list_iterator* aux = NULL;
    aux = list_iterator_create(t_list* list);
    
    while(aux != NULL){
        pcb_set_tabla_segmentos(aux->actual,bufferTablaSegmentos);
        aux = list_iterator_next(aux);
    }
}

void* funcion (t_pcb *pcb, )
{
    pcb_set_tabla_segmentos()
}

void actualizar_lista_segmentos_atomic(t_pcb *pcbATerminar, t_buffer *bufferTablaSegmentos)
{
    
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
