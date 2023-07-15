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

void __solicitar_escribir_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pidProceso)
{
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    t_buffer *bufferFwrite = buffer_create();
    buffer_pack_string(bufferFwrite, nombreArchivo);
    buffer_pack(bufferFwrite, &punteroArchivo, sizeof(punteroArchivo));
    buffer_pack(bufferFwrite, &direccionFisica, sizeof(direccionFisica));
    buffer_pack(bufferFwrite, &cantidadBytes, sizeof(cantidadBytes));
    buffer_pack(bufferFwrite, &pidProceso, sizeof(uint32_t));
    stream_send_buffer(socketFilesystem, HEADER_solicitud_escribir_archivo, bufferFwrite);
    buffer_destroy(bufferFwrite);
}

void __solicitar_leer_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pidProceso)
{
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
    t_buffer *bufferFread = buffer_create();
    buffer_pack_string(bufferFread, nombreArchivo);
    buffer_pack(bufferFread, &punteroArchivo, sizeof(punteroArchivo));
    buffer_pack(bufferFread, &direccionFisica, sizeof(direccionFisica));
    buffer_pack(bufferFread, &cantidadBytes, sizeof(cantidadBytes));
    buffer_pack(bufferFread, &pidProceso, sizeof(uint32_t));
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

// Estructuras para hilos
typedef struct ParametrosHiloTruncate {
    t_pcb* pcb;
    char *nombreArchivo;
    uint32_t tamanio;
} t_parametros_hilo_truncate;

typedef struct ParametrosHiloIO {
    t_pcb* pcb;
    char *nombreArchivo;
    uint32_t punteroArchivo;
    uint32_t direccionFisica;
    uint32_t cantidadBytes;
    uint32_t pidProceso;
} t_parametros_hilo_IO;


void *hiloTruncate(void* arg)
{   
    t_parametros_hilo_truncate *parametrosHilo = (t_parametros_hilo_truncate*)arg;
    t_pcb* pcbEnEjecucion = parametrosHilo->pcb;
    char *nombre = parametrosHilo->nombreArchivo;
    uint32_t tamanio = parametrosHilo->tamanio;
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);

    pthread_mutex_lock(&mutexSocketFilesystem);
    __solicitar_modificacion_tamanio_archivo(pcbEnEjecucion, nombre, tamanio);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    pthread_mutex_unlock(&mutexSocketFilesystem);

    if (respuestaFilesystem == HEADER_tamanio_archivo_modificado)
    {
        log_info(kernelLogger, "El proceso con PID <%d> se desbloquea --> El FS termino de truncar el archivo.", pcb_get_pid(pcbEnEjecucion));
        t_pcb* pcb = estado_remover_pcb_de_cola_atomic(estadoBlocked, pcbEnEjecucion); // chequear
        pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);
    }
    free(nombre);
    free(parametrosHilo);
    return NULL;
}

void *hiloFread(void* arg)
{      
    t_parametros_hilo_IO *parametrosHilo = (t_parametros_hilo_IO*)arg;
    t_pcb* pcbEnEjecucion = parametrosHilo->pcb;
    char *nombreArchivo = parametrosHilo->nombreArchivo;
    uint32_t punteroArchivo = parametrosHilo->punteroArchivo;
    uint32_t direccionFisica = parametrosHilo->direccionFisica;
    uint32_t cantidadBytes = parametrosHilo->cantidadBytes;
    uint32_t pidProceso = parametrosHilo->pidProceso;
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);

     pthread_mutex_lock(&mutexSocketFilesystem);
    __solicitar_leer_archivo(pcbEnEjecucion, nombreArchivo, punteroArchivo, direccionFisica, cantidadBytes, pidProceso);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    pthread_mutex_unlock(&mutexSocketFilesystem);

    if (respuestaFilesystem == HEADER_lectura_finalizada)
    {
        log_info(kernelDebuggingLogger, "El proceso con PID <%d> se desbloquea --> El FS termino de leer.", pcb_get_pid(pcbEnEjecucion));
        t_pcb* pcb = estado_remover_pcb_de_cola_atomic(estadoBlocked, pcbEnEjecucion);
        pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);
        fRead = false;
        sem_post(&semFRead);
    }
    free(nombreArchivo);
    free(parametrosHilo);
    return NULL;
}

void *hiloFwrite(void* arg)
{      
    t_parametros_hilo_IO *parametrosHilo = (t_parametros_hilo_IO*)arg;
    t_pcb* pcbEnEjecucion = parametrosHilo->pcb;
    char *nombreArchivo = parametrosHilo->nombreArchivo;
    uint32_t punteroArchivo = parametrosHilo->punteroArchivo;
    uint32_t direccionFisica = parametrosHilo->direccionFisica;
    uint32_t cantidadBytes = parametrosHilo->cantidadBytes;
    uint32_t pidProceso = parametrosHilo->pidProceso;
    int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);

    pthread_mutex_lock(&mutexSocketFilesystem);
    __solicitar_escribir_archivo(pcbEnEjecucion, nombreArchivo, punteroArchivo, direccionFisica, cantidadBytes, pidProceso);
    uint8_t respuestaFilesystem = stream_recv_header(socketFilesystem);
    log_info(kernelLogger, "FS recibe el header %u:", respuestaFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    pthread_mutex_unlock(&mutexSocketFilesystem);

    if (respuestaFilesystem == HEADER_escritura_finalizada)
    {
        log_info(kernelDebuggingLogger, "El proceso con PID <%d> se desbloquea --> El FS termino de escribir.", pcb_get_pid(pcbEnEjecucion));
        t_pcb* pcb = estado_remover_pcb_de_cola_atomic(estadoBlocked, pcbEnEjecucion);
        pcb_pasar_de_blocked_a_ready_public(pcbEnEjecucion);
        fWrite = false;
        sem_post(&semFWrite);
    }
    free(nombreArchivo);
    free(parametrosHilo);
    return NULL;
}

void adapter_filesystem_pedir_truncar_archivo(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t tamanio)
{   
    pthread_t esperarFinalizacionTruncate;
    log_info(kernelLogger, "PID <%d> solicita la modificación del tamanio de un archivo.", pcb_get_pid(pcbEnEjecucion));
    // Estructura temporal para pasarle los datos necesarios al hilo.

    t_parametros_hilo_truncate *parametrosHilo = (t_parametros_hilo_truncate*)malloc(sizeof(t_parametros_hilo_truncate));
    parametrosHilo->pcb = pcbEnEjecucion;
    parametrosHilo->nombreArchivo = strdup(nombreArchivo);
    parametrosHilo->tamanio = tamanio;

    // Hilo que envia la solicitud de truncado y se bloquea hasta que reciba la confirmación de su finalización
    pthread_create(&esperarFinalizacionTruncate, NULL, hiloTruncate, (void*)parametrosHilo);
    pthread_detach(esperarFinalizacionTruncate);

    return;
}

void adapter_filesystem_pedir_leer_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pidProceso)
{  
    pthread_t esperarFinalizacionLectura;
    log_info(kernelLogger, "PID <%d> solicita leer de un archivo.", pcb_get_pid(pcbEnEjecucion));
    // Estructura temporal para pasarle los datos necesarios al hilo.
    t_parametros_hilo_IO *parametrosHilo = (t_parametros_hilo_IO *)malloc(sizeof(t_parametros_hilo_IO));
    parametrosHilo->pcb = pcbEnEjecucion;
    parametrosHilo->nombreArchivo = strdup(nombreArchivo);
    parametrosHilo->punteroArchivo = punteroArchivo;
    parametrosHilo->direccionFisica = direccionFisica;
    parametrosHilo->cantidadBytes = cantidadBytes;
    parametrosHilo->pidProceso = pidProceso;

    // Hilo que envia la solicitud de leida y se bloquea hasta que reciba la confirmación de su finalización
    pthread_create(&esperarFinalizacionLectura, NULL, hiloFread, (void*)parametrosHilo);
    pthread_detach(esperarFinalizacionLectura);
}


void adapter_filesystem_pedir_escribir_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pidProceso)
{
    pthread_t esperarFinalizacionEscritura;
    log_info(kernelLogger, "PID <%d> solicita escribir en un archivo.", pcb_get_pid(pcbEnEjecucion));
    // Estructura temporal para pasarle los datos necesarios al hilo.
    t_parametros_hilo_IO *parametrosHilo = (t_parametros_hilo_IO*)malloc(sizeof(t_parametros_hilo_IO));
    parametrosHilo->pcb = pcbEnEjecucion;
    parametrosHilo->nombreArchivo = nombreArchivo;
    parametrosHilo->punteroArchivo = punteroArchivo;
    parametrosHilo->direccionFisica = direccionFisica;
    parametrosHilo->cantidadBytes = cantidadBytes;
    parametrosHilo->pidProceso = pidProceso;
    
    // Hilo que envia la solicitud de escritura y se bloquea hasta que reciba la confirmación de su finalización
    pthread_create(&esperarFinalizacionEscritura, NULL, hiloFwrite, (void*)parametrosHilo);
    pthread_detach(esperarFinalizacionEscritura);
}

