#include <kernel-adapter-filesystem.h>

void __enviar_consulta_existencia_archivo(int socketFilesystem, char* nombreArchivo)
{
    t_buffer *bufferConsulta = buffer_create();
    buffer_pack_string(bufferConsulta, nombreArchivo);
    stream_send_buffer(socketFilesystem, HEADER_consulta_existencia_archivo, bufferConsulta);
    buffer_destroy(bufferConsulta);
}

void __solicitar_creacion_archivo(int socketFilesystem, char* nombreArchivo)
{
    t_buffer *bufferSolicitud = buffer_create();
    buffer_pack_string(bufferSolicitud, nombreArchivo);
    stream_send_buffer(socketFilesystem, HEADER_solicitud_creacion_archivo, bufferSolicitud);
    buffer_destroy(bufferSolicitud);
} 
bool adapter_filesystem_existe_archivo(char *nombreArchivo)
{
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    __enviar_consulta_existencia_archivo(socketFilesystem, nombreArchivo);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    if (respuestaFilesystem == HEADER_archivo_existe_en_filesystem)
    {
        return true;
    }
    // El archivo no existe
    return false;
}

void adapter_filesystem_pedir_creacion_archivo(char *nombreArchivo)
{   
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    __solicitar_creacion_archivo(socketFilesystem, nombreArchivo);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    if (respuestaFilesystem != HEADER_archivo_creado)
    {
        // Siempre será posible crear un archivo y por lo tanto esta operación deberá devolver OK.
        //error
    }
    return ;
}