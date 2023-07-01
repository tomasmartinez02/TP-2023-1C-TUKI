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

void* obtener_valor_memoria(uint32_t dirFisica, uint32_t tamanio) // VER DESPUES CUALES HACER PRIVADAS
{
    void* valor = malloc(tamanio);
    memcpy(valor, memoriaPrincipal + dirFisica, tamanio);
    return valor;
}

void escribir_valor_en_memoria(uint32_t dirFisica, void* bytesRecibidos, uint32_t tamanio)
{
    memcpy(memoriaPrincipal + dirFisica, bytesRecibidos, tamanio); // cambiar

    return;
}

t_buffer* serializar_bytes_leidos(int socketModulo, uint32_t dirFisica, uint32_t bytesALeer) 
{
    void* bytesAEnviar = obtener_valor_memoria(dirFisica, bytesALeer);

    t_buffer* bufferAEnviar = buffer_create();
    buffer_pack(bufferAEnviar, &bytesAEnviar, sizeof(bytesALeer));
    free(bytesAEnviar);

    return bufferAEnviar;
}

t_buffer* memoria_recibir_buffer_solicitud(int socketModulo, uint32_t *dirFisica, uint32_t *tamanio)
{
    t_buffer* bufferRecibido = buffer_create();
    stream_recv_buffer(socketModulo,bufferRecibido);

    buffer_unpack(bufferRecibido, dirFisica, sizeof(dirFisica));
    buffer_unpack(bufferRecibido, tamanio, sizeof(tamanio));

    return bufferRecibido;
}


void* atender_modulo(void* args)
{

    for (;;) {
        int socketModulo = *(int*)args;
        t_header headerRecibido = stream_recv_header(socketModulo);
        uint32_t dirFisica;
        uint32_t tamanio;
        t_buffer* bufferRecibido = memoria_recibir_buffer_solicitud(socketModulo, &dirFisica, &tamanio);
        
        switch (headerRecibido) {
            case HEADER_solicitud_memoria_escritura:
            {
                void* bytesRecibidos = malloc(tamanio);
                buffer_unpack(bufferRecibido, bytesRecibidos, tamanio);
                escribir_valor_en_memoria(dirFisica, bytesRecibidos, tamanio);
                free(bytesRecibidos);
                stream_send_empty_buffer(socketModulo, HEADER_memoria_confirmacion_escritura);
                break;
            }
            case HEADER_solicitud_memoria_lectura:
            {
                t_buffer* bufferAEnviar = serializar_bytes_leidos(socketModulo, dirFisica, tamanio);
                stream_send_buffer(socketModulo, HEADER_memoria_confirmacion_lectura, bufferAEnviar); // LOS HEADERS HACERLOS GENERICOS
                buffer_destroy(bufferAEnviar);
                break;
            }
            default:
            {
                log_error(memoriaLogger, "Se recibio un header no esperado: %d", headerRecibido);
                break;
            }
                break;
        }
        buffer_destroy(bufferRecibido);
    }

    return NULL;
}
