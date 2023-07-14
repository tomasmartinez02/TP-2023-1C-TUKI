#include <memoria-adapter-kernel.h>

// funciones privadas

static void __empaquetar_tabla(t_info_segmentos** tablaSegmentos, uint32_t tamanioTabla, t_buffer* bufferAEnviar) 
{ 
    for(int i = 0; i < tamanioTabla; i++) {
        t_info_segmentos *infoSegmento = tablaSegmentos[i];
        
        uint32_t idSegmento = infoSegmento->idSegmento;
        buffer_pack(bufferAEnviar, &idSegmento, sizeof(idSegmento));

        uint32_t direccionBase = infoSegmento->direccionBase;
        buffer_pack(bufferAEnviar, &direccionBase, sizeof(direccionBase));

        uint32_t tamanio = infoSegmento->tamanio;
        buffer_pack(bufferAEnviar, &tamanio, sizeof(tamanio));
    }

    return;
}

static t_buffer *__empaquetar_tabla_segmentos_eliminacion(t_info_segmentos **tablaSegmentos, uint32_t tamanioTablaSegmentos)
{
    t_buffer *bufferTablaSegmentos = buffer_create();

    __empaquetar_tabla(tablaSegmentos, tamanioTablaSegmentos, bufferTablaSegmentos);
    /* 
    for (int i = 0; i < tamanioTablaSegmentos; i++) {
        t_info_segmentos *infoSegmento = tablaSegmentos[i];
        
        uint32_t idSegmento = infoSegmento->idSegmento;
        buffer_pack(bufferTablaSegmentos, &idSegmento, sizeof(idSegmento));

        uint32_t direccionBase = infoSegmento->direccionBase;
        buffer_pack(bufferTablaSegmentos, &direccionBase, sizeof(direccionBase));

        uint32_t tamanio = infoSegmento->tamanio;
        buffer_pack(bufferTablaSegmentos, &tamanio, sizeof(tamanio));
    }
    */

    return bufferTablaSegmentos;
}

static uint32_t __tamanio_tabla_de_segmentos()
{   
    lista_tablas *aux = tablasDeSegmentos;
    uint32_t contador = 0;

    while (aux != NULL) {
        contador = contador + 1;
        aux = aux->siguiente;
    }    

    return contador; 
}

static t_buffer* __empaquetar_tablas_de_segmentos(uint32_t tamanioTablas, t_buffer *bufferAEnviar) 
{
    // uint32_t tamanioTablaGeneral = __tamanio_tabla_de_segmentos();
    lista_tablas *aux = tablasDeSegmentos;

    while(aux != NULL) {
        // aca tengo que empaquetar cada pid y su respectiva tabla
        uint32_t pid = aux->pidProceso;
        buffer_pack(bufferAEnviar, &pid, sizeof(pid));
        __empaquetar_tabla(aux->tablaSegmentos, tamanioTablas, bufferAEnviar);
        aux = aux->siguiente;
    }

    return bufferAEnviar;
}

// Funciones publicas

void adapter_kernel_enviar_tabla(t_info_segmentos** tablaCreada, t_header headerAEnviar) 
{
    int socketKernel = memoria_config_get_socket_kernel(memoriaConfig);

    uint32_t cantidadSegmentos = memoria_config_get_cantidad_segmentos(memoriaConfig);
    
    t_buffer *bufferTabla = empaquetar_tabla_segmentos(tablaCreada, cantidadSegmentos);
    stream_send_buffer(socketKernel, headerAEnviar, bufferTabla);
    buffer_destroy(bufferTabla);

    return ;
}

uint32_t adapter_kernel_recibir_pid(t_buffer* bufferRecibido)
{
    uint32_t pidRecibido;

    //stream_recv_buffer(socketKernel,bufferRecibido);
    buffer_unpack(bufferRecibido, &pidRecibido, sizeof(pidRecibido));

    return pidRecibido;
}

t_info_segmentos* adapter_kernel_recibir_segmento_a_crear(uint32_t socketKernel, t_buffer* bufferRecibido)
{
    t_info_segmentos* segmentoRecibido = malloc(sizeof(t_info_segmentos));
    
    uint32_t idSegmento;
    uint32_t tamanioSegmento;
    
    stream_recv_buffer(socketKernel,bufferRecibido);
    buffer_unpack(bufferRecibido, &idSegmento, sizeof(idSegmento));
    buffer_unpack(bufferRecibido, &tamanioSegmento, sizeof(tamanioSegmento));
   
    segmentoRecibido->idSegmento = idSegmento;
    segmentoRecibido->tamanio = tamanioSegmento;
    segmentoRecibido->direccionBase = -1;

    return segmentoRecibido;
}

void adapter_kernel_enviar_direccion_base (uint32_t socketKernel, uint32_t baseSegmento)
{
    uint32_t baseSegmentoAEnviar = baseSegmento;
    t_buffer *bufferSegmento = buffer_create();
    
    buffer_pack(bufferSegmento, &baseSegmentoAEnviar, sizeof(baseSegmentoAEnviar));
    
    stream_send_buffer(socketKernel, HEADER_segmento_creado, bufferSegmento);
    buffer_destroy(bufferSegmento);
    
    return;
}

void adapter_kernel_solicitar_compactacion (uint32_t socketKernel)
{
    stream_send_empty_buffer(socketKernel, HEADER_necesita_compactacion);

    return;
}

void adapter_kernel_error_out_of_memory (uint32_t socketKernel)
{
    stream_send_empty_buffer(socketKernel, HEADER_out_of_memory);

    return;
}

uint32_t adapter_kernel_recibir_id_segmento_a_eliminar(uint32_t socketKernel, t_buffer* bufferRecibido)
{
    uint32_t pidAEliminar;

    stream_recv_buffer(socketKernel,bufferRecibido);
    buffer_unpack(bufferRecibido, &pidAEliminar, sizeof(pidAEliminar));

    return pidAEliminar;
}

void adapter_kernel_enviar_eliminacion_segmento(uint32_t socketKernel, uint32_t pid) 
{
    lista_tablas *aux = tablasDeSegmentos;
    uint32_t tamanioTabla = memoria_config_get_cantidad_segmentos(memoriaConfig);

    while (aux->pidProceso != pid) {
        aux = aux->siguiente;
    }
    
    t_buffer* bufferTabla = __empaquetar_tabla_segmentos_eliminacion(aux->tablaSegmentos, tamanioTabla);

    stream_send_buffer(socketKernel, HEADER_segmento_destruido, bufferTabla);

    buffer_destroy(bufferTabla);

    return;
}

void adapter_kernel_confirmar_finalizacion_proceso(uint32_t socketKernel, uint32_t pid)
{
    stream_send_empty_buffer(socketKernel, HANDSHAKE_ok_continue);

    return;
}

void adapter_kernel_confirmar_compactacion_memoria(uint32_t socketKernel)
{
    //esta funcion deberia avisarle a kernel que la memoria ya fue compactada y devolverle la tabla de segmentos actualizada
    
    t_buffer *bufferTablas = buffer_create();
    uint32_t tamanioTabla = memoria_config_get_cantidad_segmentos(memoriaConfig);
    uint32_t tamanioTotalTabla = __tamanio_tabla_de_segmentos();

    buffer_pack(bufferTablas, &tamanioTotalTabla, sizeof(tamanioTotalTabla));
    buffer_pack(bufferTablas, &tamanioTabla, sizeof(tamanioTabla));

    __empaquetar_tablas_de_segmentos(tamanioTabla,bufferTablas);

    stream_send_buffer(socketKernel, HEADER_memoria_compactada, bufferTablas);

    buffer_destroy(bufferTablas);

    return; 
}
