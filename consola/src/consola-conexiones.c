#include <consola-conexiones.h>

// Funciones publicas
int conectar_a_kernel(void)
{
    char *ipKernel = consola_config_get_ip_kernel(consolaConfig);
    char *puertoKernel = consola_config_get_puerto_kernel(consolaConfig);

    int socketKernel = conectar_a_servidor(ipKernel, puertoKernel);
    
    // Chequeo que la conexion con el kernel haya sido exitosa
    if (socketKernel == -1) {
        log_error(consolaLogger, "Error al intentar establecer conexión inicial con modulo Kernel");
        log_error(consolaDebuggingLogger, "Error al intentar establecer conexión inicial con modulo Kernel");
        consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger);
        exit(EXIT_FAILURE);
    }
    consola_config_set_socket_kernel(consolaConfig, socketKernel);

    log_info(consolaLogger, "Socket creado exitosamente con modulo Kernel");
    log_info(consolaDebuggingLogger, "Socket creado exitosamente con modulo Kernel");

    send_handshake_kernel(socketKernel, consolaLogger);
    receive_handshake_kernel(socketKernel, consolaConfig, consolaLogger, consolaDebuggingLogger);

    return socketKernel;
}

void send_handshake_kernel(const int socketKernel, t_log *consolaLogger)
{
    // Envio unicamente el handshake de la consola, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketKernel, HANDSHAKE_consola);
    log_info(consolaLogger, "Se ha enviado el handshake inicial al modulo kernel");
    log_info(consolaDebuggingLogger, "Se ha enviado el handshake inicial al modulo kernel");

    return;
}

void receive_handshake_kernel(const int socketKernel, t_consola_config *consolaConfig, t_log *consolaLogger, t_log *consolaDebuggingLogger)
{
    t_handshake kernelResponse = stream_recv_header(socketKernel);
    stream_recv_empty_buffer(socketKernel);
    
    // Chequeo que el kernel me de el OK para continuar ejecutando e interactuando
    if (kernelResponse != HANDSHAKE_ok_continue) {
        log_error(consolaLogger, "Error al intentar establecer Handshake inicial con modulo Kernel");
        log_error(consolaDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo Kernel");
        consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(consolaLogger, "Handshake inicial con modulo Kernel recibido exitosamente");
    log_info(consolaDebuggingLogger, "Handshake inicial con modulo Kernel recibido exitosamente");

    return;
}

void consola_enviar_instrucciones_a_kernel(const char *pathArchivoInstrucciones)
{
    t_buffer *bufferInstrucciones = buffer_create();
    
    int socketKernel = consola_config_get_socket_kernel(consolaConfig);
    if (!consola_parser_parse_instrucciones(bufferInstrucciones, pathArchivoInstrucciones)) {
        stream_send_empty_buffer(socketKernel, HEADER_error);
        log_error(consolaLogger, "Ocurrio un error en el parseo de las instrucciones. Finalizando consola...");
        log_error(consolaDebuggingLogger, "Ocurrio un error en el parseo de las instrucciones. Finalizando consola...");
        buffer_destroy(bufferInstrucciones);
        consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    stream_send_buffer(socketKernel, HEADER_lista_instrucciones, bufferInstrucciones);
    buffer_destroy(bufferInstrucciones);
    
    log_info(consolaLogger, "Se envía la lista de instrucciones al modulo Kernel");
    log_info(consolaDebuggingLogger, "Se envía la lista de instrucciones al modulo Kernel");

    return;
}

uint32_t receive_pid_kernel(void)
{
    int socketKernel = consola_config_get_socket_kernel(consolaConfig);
    t_header kernelResponse = stream_recv_header(socketKernel);
    
    if (kernelResponse != HEADER_pid) {
        log_error(consolaLogger, "Error al intentar recibir el PID");
        log_error(consolaDebuggingLogger, "Error al intentar recibir el PID");
        consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    uint32_t idProcesoTemp;
    
    t_buffer *bufferPID = buffer_create();
    stream_recv_buffer(socketKernel, bufferPID);
    buffer_unpack(bufferPID, &idProcesoTemp, sizeof(idProcesoTemp));
    buffer_destroy(bufferPID);
    
    consola_config_set_pid(consolaConfig, idProcesoTemp);

    return idProcesoTemp;
}

void wait_kernel_response()
{
    int socketKernel = consola_config_get_socket_kernel(consolaConfig);
    uint8_t kernelResponse = stream_recv_header(socketKernel);
    stream_recv_empty_buffer(socketKernel);

    uint32_t idProceso = consola_config_get_pid(consolaConfig);
    if(kernelResponse != HEADER_proceso_terminado) {
        log_error(consolaLogger, "Error al intentar finalizar consola de <ID %d>", idProceso);
        log_error(consolaDebuggingLogger, "Error al intentar finalizar consola de <ID %d>", idProceso);
        consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger);
        exit(EXIT_FAILURE);
    }
    
    log_info(consolaLogger, "Finalización de consola PID <%d> correctamente por instruccion EXIT", idProceso);
    log_info(consolaDebuggingLogger, "Finalización de consola PID <%d> correctamente por instruccion EXIT", idProceso);
    
    consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger);         
    exit(EXIT_SUCCESS);

    return;
}