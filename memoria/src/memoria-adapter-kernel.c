#include <memoria-adapter-kernel.h>

void adapter_kernel_enviar_tabla(t_info_segmentos** tablaCreada, t_header headerAEnviar) 
{
    int socketKernel = memoria_config_get_socket_kernel(memoriaConfig);

    uint32_t cantidadSegmentos = memoria_config_get_cantidad_segmentos(memoriaConfig);
    
    t_buffer *bufferTabla = empaquetar_tabla_segmentos(tablaCreada, cantidadSegmentos);
    stream_send_buffer(socketKernel, headerAEnviar, bufferTabla);
    buffer_destroy(bufferTabla);

    return ;
}

uint32_t adapter_kernel_recibir_pid(uint32_t socketKernel, t_buffer* bufferRecibido)
{
    uint32_t pidRecibido;

    stream_recv_buffer(socketKernel,bufferRecibido);
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
    t_buffer *bufferTabla = buffer_create();
    lista_tablas *aux = tablasDeSegmentos;
    uint32_t tamanioTabla = memoria_config_get_cantidad_segmentos(memoriaConfig);
    
    while (aux->pidProceso != pid) {
        aux = aux->siguiente;
    }
    
    buffer_pack(bufferTabla, &tamanioTabla, sizeof(tamanioTabla));
    bufferTabla = empaquetar_tabla_segmentos(aux->tablaSegmentos, tamanioTabla);

    stream_send_buffer(socketKernel, HEADER_segmento_destruido, bufferTabla);

    buffer_destroy(bufferTabla);

    return;
}