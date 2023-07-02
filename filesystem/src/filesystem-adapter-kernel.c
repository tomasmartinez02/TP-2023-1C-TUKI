#include <filesystem-adapter-kernel.h>
t_header recibir_header_peticion_de_kernel(void)
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);
   
    t_header headerPeticion = stream_recv_header(socketKernel);

    return headerPeticion;
}

char *recibir_buffer_nombre_archivo()
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);

    t_buffer *bufferArchivo = buffer_create();
    stream_recv_buffer(socketKernel, bufferArchivo);
    char *nombreArchivo = buffer_unpack_string(bufferArchivo);
    buffer_destroy(bufferArchivo);

    return nombreArchivo;
}

void recibir_buffer_truncate_archivo(char **nombreArchivo, uint32_t *tamanioNuevo)
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);

    t_buffer *bufferFtruncate = buffer_create();
    stream_recv_buffer(socketKernel, bufferFtruncate);

    char *nombreArchivoFtruncate = buffer_unpack_string(bufferFtruncate);
    *nombreArchivo = strdup(nombreArchivoFtruncate);
    free(nombreArchivoFtruncate);
    
    uint32_t tamanio;
    buffer_unpack(bufferFtruncate, &tamanio, sizeof(tamanio));
    *tamanioNuevo = tamanio;

    buffer_destroy(bufferFtruncate);

    return;
}

void recibir_buffer_lectura_archivo(char **nombreArchivo, uint32_t *puntero, uint32_t *direccionFisica, uint32_t *cantidadBytes, uint32_t *pid)
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);

    t_buffer *bufferLectura = buffer_create();
    stream_recv_buffer(socketKernel, bufferLectura);

    char *nombreArchivoLectura = buffer_unpack_string(bufferLectura);
    *nombreArchivo = strdup(nombreArchivoLectura);
    free(nombreArchivoLectura);

    uint32_t punteroBuffer;
    buffer_unpack(bufferLectura, &punteroBuffer, sizeof(punteroBuffer));
    *puntero = punteroBuffer;

    uint32_t direccion;
    buffer_unpack(bufferLectura, &direccion, sizeof(direccion));
    *direccionFisica = direccion;

    uint32_t bytes;
    buffer_unpack(bufferLectura, &bytes, sizeof(bytes));
    *cantidadBytes = bytes;

    uint32_t pidProceso;
    buffer_unpack(bufferLectura, &pidProceso, sizeof(pidProceso));
    *pid = pidProceso;

    buffer_destroy(bufferLectura);
    return;
}

void recibir_buffer_escritura_archivo(char **nombreArchivo, uint32_t *puntero, uint32_t *direccionFisica, uint32_t *cantidadBytes, uint32_t* pid)
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);

    t_buffer *bufferLectura = buffer_create();
    stream_recv_buffer(socketKernel, bufferLectura);

    char *nombreArchivoLectura = buffer_unpack_string(bufferLectura);
    *nombreArchivo = strdup(nombreArchivoLectura);
    free(nombreArchivoLectura);

    uint32_t punteroBuffer;
    buffer_unpack(bufferLectura, &punteroBuffer, sizeof(punteroBuffer));
    *puntero = punteroBuffer;

    uint32_t direccion;
    buffer_unpack(bufferLectura, &direccion, sizeof(direccion));
    *direccionFisica = direccion;

    uint32_t bytes;
    buffer_unpack(bufferLectura, &bytes, sizeof(bytes));
    *cantidadBytes = bytes;

    uint32_t pidProceso;
    buffer_unpack(bufferLectura, &pidProceso, sizeof(pidProceso));
    *pid = pidProceso;

    buffer_destroy(bufferLectura);
    return;
}

void enviar_confirmacion_existencia_archivo()
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);
    stream_send_empty_buffer(socketKernel, HEADER_archivo_existe_en_filesystem);
    return;
}

void enviar_confirmacion_no_existencia_archivo()
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);
    stream_send_empty_buffer(socketKernel, HEADER_archivo_no_existe_en_filesystem);
    return;
}

void enviar_confirmacion_archivo_creado()
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);
    stream_send_empty_buffer(socketKernel, HEADER_archivo_creado);
    return;
}

void enviar_confirmacion_fread_finalizado()
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);
    stream_send_empty_buffer(socketKernel, HEADER_lectura_finalizada);
    return;
}

void enviar_confirmacion_fwrite_finalizado()
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);
    stream_send_empty_buffer(socketKernel, HEADER_escritura_finalizada);
    return;
}

void enviar_confirmacion_tamanio_archivo_modificado()
{
    int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);
    stream_send_empty_buffer(socketKernel, HEADER_tamanio_archivo_modificado);
    return;
}