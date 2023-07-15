#include <filesystem-adapter-memoria.h>

// F_WRITE

// Solicitar a la Memoria la información que se encuentra a partir de la dirección física.
void solicitar_informacion_memoria(uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pid)
{
    int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);
    t_buffer *bufferLectura = buffer_create();
    buffer_pack(bufferLectura, &direccionFisica, sizeof(uint32_t));
    buffer_pack(bufferLectura, &cantidadBytes, sizeof(uint32_t));
    buffer_pack(bufferLectura, &pid, sizeof(uint32_t));
    stream_send_buffer(socketMemoria, HEADER_solicitud_memoria_lectura, bufferLectura);
    buffer_destroy(bufferLectura);
    return;
}

// Recibir información de Memoria para escribir en los bloques.
void* recibir_buffer_informacion_memoria(uint32_t cantidadBytes)
{   
    void* informacionRecibida = malloc(cantidadBytes);
    int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);

    t_buffer *bufferInformacion = buffer_create();
    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);
    stream_recv_buffer(socketMemoria, bufferInformacion);
    buffer_unpack(bufferInformacion, informacionRecibida, cantidadBytes);

    if (respuestaMemoria != HEADER_memoria_confirmacion_lectura)
    {
        log_info(filesystemLogger, "Error al recibir la información de memoria.");
        return  NULL;
    }
    buffer_destroy(bufferInformacion);
    return informacionRecibida;
}

// F_READ

// Enviar información a Memoria para que la escriba en la dirección física. 
void solicitar_escritura_memoria(uint32_t direccionFisica, uint32_t cantidadBytes, char* informacion, uint32_t pid)
{
    int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);
    void *informacionParaBuffer = (void*)informacion;
    t_buffer *bufferLectura = buffer_create();
    buffer_pack(bufferLectura, &direccionFisica, sizeof(uint32_t));
    buffer_pack(bufferLectura, &cantidadBytes, sizeof(uint32_t));
    buffer_pack(bufferLectura, &pid, sizeof(uint32_t));
    buffer_pack(bufferLectura, informacionParaBuffer, cantidadBytes);
    
    stream_send_buffer(socketMemoria, HEADER_solicitud_memoria_escritura, bufferLectura);
    buffer_destroy(bufferLectura);
    return;
}

// Recibir información de Memoria para escribir en los bloques.
bool recibir_buffer_confirmacion_escritura_memoria()
{
    int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);
    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);
    stream_recv_empty_buffer(socketMemoria);

    if (respuestaMemoria != HEADER_memoria_confirmacion_escritura)
    {
        log_info(filesystemLogger, "Error al recibir la confirmación de escritura de memoria.");
        return false;
    }

    return true;
}
