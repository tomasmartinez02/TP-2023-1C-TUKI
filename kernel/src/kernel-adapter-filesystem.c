#include <kernel-adapter-filesystem.h>

void __enviar_consulta_existencia_archivo(char* nombreArchivo)
{
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    t_buffer *bufferConsulta = buffer_create();
    buffer_pack_string(bufferConsulta, nombreArchivo);
    stream_send_buffer(socketFilesystem, HEADER_consulta_existencia_archivo, bufferConsulta);
    buffer_destroy(bufferConsulta);
}

void __solicitar_creacion_archivo(char* nombreArchivo)
{   
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    t_buffer *bufferSolicitud = buffer_create();
    buffer_pack_string(bufferSolicitud, nombreArchivo);
    stream_send_buffer(socketFilesystem, HEADER_solicitud_creacion_archivo, bufferSolicitud);
    buffer_destroy(bufferSolicitud);
} 

void __solicitar_modificacion_tamanio_archivo(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t tamanio)
{
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    t_buffer *bufferTruncar = buffer_create();
    buffer_pack_string(bufferTruncar, nombreArchivo);
    buffer_pack(bufferTruncar, &tamanio, sizeof(tamanio));
    stream_send_buffer(socketFilesystem, HEADER_solicitud_modificar_tamanio_archivo, bufferTruncar);
    buffer_destroy(bufferTruncar);
}

void __solicitar_escribir_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes)
{
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    t_buffer *bufferFwrite = buffer_create();
    buffer_pack_string(bufferFwrite, nombreArchivo);
    buffer_pack(bufferFwrite, &punteroArchivo, sizeof(punteroArchivo));
    buffer_pack(bufferFwrite, &direccionFisica, sizeof(direccionFisica));
    buffer_pack(bufferFwrite, &cantidadBytes, sizeof(cantidadBytes));
    stream_send_buffer(socketFilesystem, HEADER_solicitud_escribir_archivo, bufferFwrite);
    buffer_destroy(bufferFwrite);
}

void __solicitar_leer_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes)
{
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    t_buffer *bufferFread = buffer_create();
    buffer_pack_string(bufferFread, nombreArchivo);
    buffer_pack(bufferFread, &punteroArchivo, sizeof(punteroArchivo));
    buffer_pack(bufferFread, &direccionFisica, sizeof(direccionFisica));
    buffer_pack(bufferFread, &cantidadBytes, sizeof(cantidadBytes));
    stream_send_buffer(socketFilesystem, HEADER_solicitud_leer_archivo, bufferFread);
    buffer_destroy(bufferFread);
}

uint32_t adapter_filesystem_existe_archivo(char *nombreArchivo)
{   
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    pthread_mutex_lock(&mutexSocketFilesystem);
    __enviar_consulta_existencia_archivo(nombreArchivo);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    pthread_mutex_unlock(&mutexSocketFilesystem);
    
    if (respuestaFilesystem == HEADER_archivo_existe_en_filesystem)
    {
        log_info(kernelLogger, "El archivo '%s' existe", nombreArchivo);
        log_info(kernelDebuggingLogger, "El archivo '%s' existe", nombreArchivo);
        return 1;
    }
    if (respuestaFilesystem == HEADER_archivo_no_existe_en_filesystem)
    {   
        log_info(kernelLogger, "El archivo '%s' NO existe", nombreArchivo);
        log_info(kernelDebuggingLogger, "El archivo '%s' NO existe", nombreArchivo);
        return 0;
    }
    log_error(kernelLogger, "Error al verificar si existe el archivo");
    log_error(kernelDebuggingLogger, "Error al verificar si existe el archivo");
    return 2;
}

void adapter_filesystem_pedir_creacion_archivo(char *nombreArchivo)
{    
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    
    pthread_mutex_lock(&mutexSocketFilesystem);
    __solicitar_creacion_archivo(nombreArchivo);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    pthread_mutex_unlock(&mutexSocketFilesystem);

    if (respuestaFilesystem != HEADER_archivo_creado)
    {   
        // SIEMPRE SE DEBERIA PODER CREAR UN ARCHIVO
        log_error(kernelLogger, "Error en la creación del archivo");
        log_error(kernelDebuggingLogger, "Error en la creación del archivo");
    }
    log_info(kernelLogger, "Se creo el archivo <%s>", nombreArchivo);
    log_info(kernelDebuggingLogger, "Se creo el archivo <%s>", nombreArchivo);
    return ;
}

void *hiloTruncate(void* arg)
{      
    t_pcb* pcbEnEjecucion = (t_pcb*)arg;
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    pthread_mutex_unlock(&mutexSocketFilesystem);

    if (respuestaFilesystem == HEADER_tamanio_archivo_modificado)
    {
        log_info(kernelLogger, "El proceso con PID <%d> se desbloquea ya que el fs termino de truncar el archivo", pcb_get_pid(pcbEnEjecucion));
        pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);
    }
    return NULL;
}

void *hiloFread(void* arg)
{      
    t_pcb* pcbEnEjecucion = (t_pcb*)arg;
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    pthread_mutex_unlock(&mutexSocketFilesystem);

    if (respuestaFilesystem == HEADER_lectura_finalizada)
    {
        log_info(kernelDebuggingLogger, "El proceso con PID <%d> se desbloquea ya que el fs avisa que ya se leyo del archivo", pcb_get_pid(pcbEnEjecucion));
        pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);
    }
    return NULL;
}

void *hiloFwrite(void* arg)
{      
    t_pcb* pcbEnEjecucion = (t_pcb*)arg;
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    pthread_mutex_unlock(&mutexSocketFilesystem);

    if (respuestaFilesystem == HEADER_escritura_finalizada)
    {
        log_info(kernelDebuggingLogger, "El proceso con PID <%d> se desbloquea ya que el fs termino de escribir el archivo", pcb_get_pid(pcbEnEjecucion));
        pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);
    }
    return NULL;
}

void adapter_filesystem_pedir_truncar_archivo(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t tamanio)
{   
    pthread_t esperarFinalizacionTruncate;
    log_info(kernelDebuggingLogger, "PID <%d> solitica la modificación del tamanio de un archivo", pcb_get_pid(pcbEnEjecucion));
    pthread_mutex_lock(&mutexSocketFilesystem);
    __solicitar_modificacion_tamanio_archivo(pcbEnEjecucion, nombreArchivo, tamanio);
    pthread_create(&esperarFinalizacionTruncate, NULL, hiloTruncate, (void*)pcbEnEjecucion);
    pthread_detach(esperarFinalizacionTruncate);

    return;
}

 void adapter_filesystem_pedir_leer_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes)
 {  
    pthread_t esperarFinalizacionLectura;
    log_info(kernelDebuggingLogger, "PID <%d> solicita leer de un archivo", pcb_get_pid(pcbEnEjecucion));
    pthread_mutex_lock(&mutexSocketFilesystem);
    __solicitar_leer_archivo(pcbEnEjecucion, nombreArchivo, punteroArchivo, direccionFisica, cantidadBytes);
    pthread_create(&esperarFinalizacionLectura, NULL, hiloFread, (void*)pcbEnEjecucion);
    pthread_detach(esperarFinalizacionLectura);
 }

void adapter_filesystem_pedir_escribir_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes)
{
    pthread_t esperarFinalizacionEscritura;
    log_info(kernelDebuggingLogger, "PID <%d> solitica escribir en un archivo", pcb_get_pid(pcbEnEjecucion));
    pthread_mutex_lock(&mutexSocketFilesystem);
    __solicitar_escribir_archivo(pcbEnEjecucion, nombreArchivo, punteroArchivo, direccionFisica, cantidadBytes);
    pthread_create(&esperarFinalizacionEscritura, NULL, hiloFwrite, (void*)pcbEnEjecucion);
    pthread_detach(esperarFinalizacionEscritura);
}