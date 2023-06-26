#include <memoria-utils.h>

// Funciones privadas

// Adapta la funcion para destruir el config
static void __config_destroyer(void *moduleConfig)
{
    t_memoria_config *memoriaConfigTemp = (t_memoria_config *) moduleConfig;
    memoria_config_destroy(memoriaConfigTemp);

    return;
}

// Funciones publicas
void memoria_destroy(t_memoria_config *memoriaConfig, t_log *memoriaLogger, t_log *memoriaDebuggingLogger)
{
    module_destroy((void *) memoriaConfig, __config_destroyer, memoriaLogger, memoriaDebuggingLogger); 

    return;
}

void* obtener_valor_memoria(int desplazamiento, int tamanio)
{
    void* valor = malloc(tamanio);
    memcpy(valor, memoriaPrincipal + desplazamiento, tamanio);
    return valor;
}

void escribir_valor_en_memoria(void* ptr, const void* src, uint32_t size)  // Me hace ruido el casteo!!
{
    unsigned char* dest = (unsigned char*)ptr;
    const unsigned char* source = (const unsigned char*)src;

    for (uint32_t i = 0; i < size; i++) {
        *(unsigned char*)((unsigned char*)dest + i) = *(unsigned char*)((unsigned char*)source + i);
    }

    return;
}

t_buffer* serializar_bytes_leidos(int socketModulo) 
{
    t_buffer* bufferRecibido = buffer_create();
    stream_recv_buffer(socketModulo,bufferRecibido);

    uint32_t idSegmento;
    buffer_unpack(bufferRecibido, &idSegmento, sizeof(idSegmento));

    uint32_t desplazamiento;
    buffer_unpack(bufferRecibido, &desplazamiento, sizeof(desplazamiento));

    uint32_t bytesALeer;
    buffer_unpack(bufferRecibido, &bytesALeer, sizeof(bytesALeer));
    buffer_destroy(bufferRecibido);

    uint32_t base;
    base = obtener_base_segmento(idSegmento);

    void* bytesAEnviar = obtener_valor_memoria(base + desplazamiento, bytesALeer);

    t_buffer* bufferAEnviar = buffer_create();
    buffer_pack(bufferAEnviar, &bytesAEnviar, sizeof(bytesALeer));

    return bufferAEnviar;
}

uint32_t obtener_base_segmento(uint32_t idSegmento)
{
    uint32_t base = 0;
    //TODO
    return base;
}