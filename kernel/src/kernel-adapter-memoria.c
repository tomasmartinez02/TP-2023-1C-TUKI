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

static lista_tablas* __desempaquetar_tabla_segmentos(t_buffer* bufferTablaDeSegmentosActualizada, uint32_t tamanioTablaDeSegmentosActualizada, uint32_t tamanioTablas)
{
    lista_tablas* tablaDeSegmentos = NULL;
    lista_tablas* aux = tablaDeSegmentos;    

    for(int i=0; i < tamanioTablaDeSegmentosActualizada; i++) {
        uint32_t pidProceso;
        t_info_segmentos** tablaProceso; 
        buffer_unpack(bufferTablaDeSegmentosActualizada, &pidProceso, sizeof(pidProceso));
        tablaProceso = desempaquetar_tabla_segmentos(bufferTablaDeSegmentosActualizada, tamanioTablas);

        lista_tablas* nuevoNodo = malloc(sizeof(lista_tablas));
        nuevoNodo->pidProceso = pidProceso;
        nuevoNodo->tablaSegmentos = tablaProceso;
        nuevoNodo->siguiente = NULL;

        if(aux == NULL) {
            aux = nuevoNodo;
            tablaDeSegmentos = nuevoNodo;
        } else {
            aux->siguiente = nuevoNodo;
            aux = aux->siguiente;
        }
    }

    return tablaDeSegmentos;
}
static t_info_segmentos** __buscar_tabla_correspondiente(uint32_t pid, lista_tablas* tablasDeSegmentosActualizadas)
{
    lista_tablas* aux = tablasDeSegmentosActualizadas;

    // while (aux->siguiente != NULL && aux->pidProceso != pid) {
    while (aux != NULL && aux->pidProceso != pid) {
        aux = aux->siguiente;
    }

    return aux->tablaSegmentos;
}

static void __actualizar_tablas_segmentos(lista_tablas* tablaDesempaquetada, t_pcb* pcbEnEjecucion)
{
    actualizar_tabla_segmentos(tablaDesempaquetada, estado_get_list(estadoReady));
    actualizar_tabla_segmentos(tablaDesempaquetada, estado_get_list(estadoBlocked));

    t_info_segmentos** tablaSeleccionada = __buscar_tabla_correspondiente(pcb_get_pid(pcbEnEjecucion), tablaDesempaquetada); 
    pcb_set_tabla_segmentos(pcbEnEjecucion, tablaSeleccionada);

    return;
}

static void __eliminar_tabla_general(lista_tablas* tablaADestruir, uint32_t tamanioTablas)
{
    while (tablaADestruir != NULL) {
        destruir_tabla_segmentos(tablaADestruir->tablaSegmentos, tamanioTablas);
        tablaADestruir = tablaADestruir->siguiente;
    }

    free(tablaADestruir);

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
            stream_recv_empty_buffer(socketMemoria);
            terminar_proceso(pcb, FINALIZACION_OUTOFMEMORY);
            pthread_mutex_unlock(&mutexSocketMemoria);
            log_info(kernelDebuggingLogger, "No hay memoria suficiente");
            sem_post(&dispatchPermitido);
            // Acá habria que terminar el proceso
            break;
        }
        
        case HEADER_necesita_compactacion:
        {
            stream_recv_empty_buffer(socketMemoria);
            if(fRead || fWrite) {
                log_info(kernelLogger, "Compactacion: <Esperando Fin de Operaciones de FS>");
            } 
            sem_wait(&semFRead);
            sem_wait(&semFWrite);
            log_info(kernelLogger, "Compactacion: <Se solicito compactacion>");
            adapter_memoria_pedir_compactacion(pcb);
            log_info(kernelLogger, "Se finalizo el proceso de compactacion");
            pthread_mutex_unlock(&mutexSocketMemoria);
            sem_post(&semFRead);
            sem_post(&semFWrite);
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

void adapter_memoria_pedir_compactacion(t_pcb *pcbEnEjecucion) 
{
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    __enviar_pedido_compactacion(socketMemoria);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    if(respuestaMemoria == HEADER_memoria_compactada)
    { 
        t_buffer *bufferTablaDeSegmentosActualizada = buffer_create();
        stream_recv_buffer(socketMemoria, bufferTablaDeSegmentosActualizada);

        uint32_t tamanioTablaDeSegmentosActualizada; 
        buffer_unpack(bufferTablaDeSegmentosActualizada, &tamanioTablaDeSegmentosActualizada, sizeof(tamanioTablaDeSegmentosActualizada));

        uint32_t tamanioTablas;
        buffer_unpack(bufferTablaDeSegmentosActualizada, &tamanioTablas, sizeof(tamanioTablas));

        lista_tablas* tablaDesempaquetada = __desempaquetar_tabla_segmentos(bufferTablaDeSegmentosActualizada, tamanioTablaDeSegmentosActualizada, tamanioTablas); 

        __actualizar_tablas_segmentos(tablaDesempaquetada, pcbEnEjecucion);

        free(tablaDesempaquetada);

        buffer_destroy(bufferTablaDeSegmentosActualizada);

    } else {
        log_error(kernelLogger, "No se pudo compactar la memoria");
    }

    return;
}

void adapter_memoria_pedir_eliminar_segmento(uint32_t idSegmento, t_pcb* pcb)
{
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    pthread_mutex_lock(&mutexSocketMemoria);
    __enviar_pedido_eliminar_segmento(socketMemoria, idSegmento, pcb_get_pid(pcb));

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    if(respuestaMemoria == HEADER_segmento_destruido)
    { 
        t_buffer *bufferTablaDeSegmentosActualizada = buffer_create();
        stream_recv_buffer(socketMemoria, bufferTablaDeSegmentosActualizada);
        
        t_info_segmentos ** nuevaTabla = desempaquetar_tabla_segmentos(bufferTablaDeSegmentosActualizada, pcb_get_tamanio_tabla_segmentos(pcb));

        pcb_set_tabla_segmentos(pcb, nuevaTabla); 

        buffer_destroy(bufferTablaDeSegmentosActualizada);
        
        seguir_ejecutando(pcb);
        
    } else {
        log_error(kernelLogger, "No se pudo destruir el segmento");
    }

    pthread_mutex_unlock(&mutexSocketMemoria);

    return ;   
}

void actualizar_tabla_segmentos(lista_tablas* tablasDeSegmentosActualizadas, t_list *listaProcesos)
{
    void __cambiar_tabla_segmentos(void* pcb)
    {
        t_info_segmentos** tablaDeSegmentosProceso = __buscar_tabla_correspondiente(pcb_get_pid(pcb), tablasDeSegmentosActualizadas);
        pcb_set_tabla_segmentos(pcb, tablaDeSegmentosProceso);
        return ;
    }

    list_iterate(listaProcesos, __cambiar_tabla_segmentos);

    return ;
}

void adapter_memoria_finalizar_proceso(t_pcb *pcbATerminar)
{   
    if (pcb_get_estado_finalizacion(pcbATerminar)) {
        
        pthread_mutex_lock(&mutexSocketMemoria);

        uint32_t pid = pcb_get_pid(pcbATerminar);
        uint32_t tamanioPid = sizeof(pid);
        int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

        t_buffer *bufferPidATerminar = buffer_create();
        buffer_pack(bufferPidATerminar, (void*) &pid, tamanioPid);
        stream_send_buffer(socketMemoria, HEADER_solicitud_finalizacion_proceso, bufferPidATerminar); 
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

        pthread_mutex_unlock(&mutexSocketMemoria);
    
    } else {
            log_error(kernelLogger, "El proceso con PID <%d> finalizado no pudo ser creado por falta de memoria", pcb_get_pid(pcbATerminar));
    }

   return;
}
