#include <filesystem-adapter-memoria.h>

// F_WRITE
// Solicitar a la Memoria la información que se encuentra a partir de la dirección física.
void solicitar_lectura_memoria(uint32_t direccionFisica)
{
    int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);
    t_buffer *bufferLectura = buffer_create();
    buffer_pack(bufferLectura, &direccionFisica, sizeof(uint32_t));
    stream_send_buffer(socketMemoria, HEADER_fs_solicitud_memoria_lectura, bufferLectura);
    return;
}
// Recibir información de Memoria para escribir en los bloques.
char* recibir_buffer_informacion_memoria()
{
    int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);
    t_buffer *bufferInformacion = buffer_create();
    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);
    stream_recv_buffer(socketMemoria, bufferInformacion);

    if (respuestaMemoria != HEADER_memoria_confirmacion_fs_lectura)
    {
        log_info(filesystemLogger, "Error al recibir la información de memoria.");
        return  NULL;
    }
    char *informacion = buffer_unpack_string(bufferInformacion);

    buffer_destroy(bufferInformacion);

    return informacion;
}

// F_READ
// Enviar información a Memoria para que la escriba en la dirección física. 
 void solicitar_escritura_memoria(uint32_t direccionFisica, char* informacion)
{
    int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);
    t_buffer *bufferLectura = buffer_create();
    buffer_pack(bufferLectura, &direccionFisica, sizeof(uint32_t));
    buffer_pack_string(bufferLectura, informacion);
    stream_send_buffer(socketMemoria, HEADER_fs_solicitud_memoria_escritura, bufferLectura);
    return;
}
// Recibir información de Memoria para escribir en los bloques.
bool recibir_buffer_confirmacion_escritura_memoria()
{
    int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);
    t_buffer *bufferConfirmacion = buffer_create();
    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);
    stream_recv_buffer(socketMemoria, bufferConfirmacion);

    if (respuestaMemoria != HEADER_memoria_confirmacion_fs_escritura)
    {
        log_info(filesystemLogger, "Error al recibir la confirmación de escritura de memoria.");
        return false;
    }

    buffer_destroy(bufferConfirmacion);
    return true;
}
