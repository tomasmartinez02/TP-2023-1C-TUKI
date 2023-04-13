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