#include <filesystem-conexiones.h>

// Funciones privadas

// Envia el handshake inicial a memoria
static void __send_handshake_memoria(void)
{
    const int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);

    // Envio unicamente el handshake del Filesystem, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketMemoria, HANDSHAKE_filesystem);
    log_info(filesystemDebuggingLogger, "Se ha enviado el handshake inicial al modulo Memoria");

    return;
}

// Espera y recibe la respuesta al handshake inicial del modulo memoria
static void __receive_handshake_memoria(void)
{
    const int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);

    // Aguardo respuesta del modulo Memoria
    t_handshake memoriaResponse = stream_recv_header(socketMemoria);
    stream_recv_empty_buffer(socketMemoria);
    
    // Chequeo que la memoria me de el OK para continuar ejecutando e interactuando
    if (memoriaResponse != HANDSHAKE_ok_continue) {
        log_error(filesystemLogger, "Error al intentar establecer Handshake inicial con modulo Memoria");
        log_error(filesystemDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo Memoria");
        filesystem_destroy(filesystemConfig, filesystemLogger, filesystemDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(filesystemDebuggingLogger, "Handshake inicial con modulo Memoria recibido exitosamente");

    return;
}

// Funciones publicas

int conectar_a_memoria(void)
{
    char *ipMemoria = filesystem_config_get_memory_ip(filesystemConfig);
    char *puertoMemoria = filesystem_config_get_puerto_memoria(filesystemConfig);

    int socketMemoria = conectar_a_servidor(ipMemoria, puertoMemoria);
    
    // Chequeo que la conexion con la Memoria haya sido exitosa
    if (socketMemoria == -1) {
        log_error(filesystemLogger, "Error al intentar establecer conexión inicial con modulo Memoria");
        log_error(filesystemDebuggingLogger, "Error al intentar establecer conexión inicial con modulo Memoria");
        filesystem_destroy(filesystemConfig, filesystemLogger, filesystemDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    filesystem_config_set_socket_memoria(filesystemConfig, socketMemoria);
    log_info(filesystemDebuggingLogger, "Socket creado exitosamente con modulo Memoria");

    // Handshakes iniciales con modulo Memoria
    __send_handshake_memoria();
    __receive_handshake_memoria();

    log_info(filesystemDebuggingLogger, "Conexión con modulo Memoria establecida exitosamente");

    return socketMemoria;
}

// Levantar servidor Filesystem

int inicializar_servidor_filesystem(void)
{
    char* ipFilesystem = filesystem_config_get_ip_escucha(filesystemConfig);
    char *puertoFilesystem = filesystem_config_get_puerto_escucha(filesystemConfig);
    int tempFilesystemSocketServerEscucha = iniciar_servidor(ipFilesystem, puertoFilesystem);

    // Chequeo que se haya inciado el servidor del Filesystem correctamente
    if (tempFilesystemSocketServerEscucha == -1) {
        log_error(filesystemLogger, "Error al intentar iniciar servidor del Filesystem");
        log_error(filesystemDebuggingLogger, "Error al intentar iniciar servidor del Filesystem");
        filesystem_destroy(filesystemConfig, filesystemLogger, filesystemDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(filesystemDebuggingLogger, "Se ha inicializado el servidor de escucha de Kernel correctamente");
    
    return tempFilesystemSocketServerEscucha;
}

void aceptar_conexion_kernel(const int socketEscucha)
{
    struct sockaddr cliente = {0};
    socklen_t len = sizeof(cliente);
    
    log_info(filesystemDebuggingLogger, "A la escucha de conexion del modulo Kernel en puerto %d", socketEscucha);
    
    //for (;;) {
        
        const int socketKernel = accept(socketEscucha, &cliente, &len);
        
        if (socketKernel > -1) {
            
            // Recibo handshake
            t_handshake handshakeKernel = stream_recv_header(socketKernel);

            if (handshakeKernel == HANDSHAKE_kernel) {
                filesystem_config_set_socket_kernel(filesystemConfig, socketKernel);
                log_info(filesystemDebuggingLogger, "Se recibio el handshake del kernel correctamente");
                
                // Respondo handshake ok
                stream_send_empty_buffer(socketKernel, HANDSHAKE_ok_continue);
                log_info(filesystemDebuggingLogger, "Se ha enviado la respuesta al handshake inicial del Kernel con handshake ok continue");
            }
            else {
                log_error(filesystemLogger, "Error al intentar establecer conexión con Kernel mediante <socket %d>", socketKernel);
                log_error(filesystemDebuggingLogger, "Error al intentar establecer conexión con Kernel mediante <socket %d>", socketKernel);
            }
        } 
        else {
            log_error(filesystemLogger, "Error al aceptar conexión: %s", strerror(errno));
            log_error(filesystemDebuggingLogger, "Error al aceptar conexión: %s", strerror(errno));
        }
    //}
}