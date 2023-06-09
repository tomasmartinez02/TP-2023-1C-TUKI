#include <memoria-adapter-kernel.h>

static t_buffer* __serializar_tabla_para_ejecucion(t_info_segmentos* tablaASerializar)
{
    t_buffer* bufferAEnviar = buffer_create();
    uint32_t cantidadSegmentos = memoria_config_get_cantidad_segmentos(memoriaConfig);
    uint32_t idSegmento, direccionBase, tamanio;
    
    for (int i = 1; i < cantidadSegmentos; i++) {
        idSegmento = tablaASerializar[i].idSegmento; 
        direccionBase = tablaASerializar[i].direccionBase;
        tamanio = tablaASerializar[i].tamanio;
        buffer_pack(bufferAEnviar, &idSegmento, sizeof(idSegmento));
        buffer_pack(bufferAEnviar, &direccionBase, sizeof(direccionBase));
        buffer_pack(bufferAEnviar, &tamanio, sizeof(tamanio));
    }

    return bufferAEnviar;
}

void adapter_kernel_enviar_tabla(t_info_segmentos* tablaCreada, t_header headerAEnviar) 
{
    int socketKernel = memoria_config_get_socket_kernel(memoriaConfig);
    
    t_buffer *bufferTabla = __serializar_tabla_para_ejecucion(tablaCreada);
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