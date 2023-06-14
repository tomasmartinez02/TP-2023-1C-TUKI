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

void adapter_kernel_enviar_direccion_base(uint32_t socketKernel, uint32_t baseSegmento)
{
    //TODO
    return;
}
