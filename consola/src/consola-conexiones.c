#include <consola-conexiones.h>

// Funciones publicas
int conectar_a_kernel(char *kernelIP, char *kernelPort, t_consola_config *consolaConfig, t_log *consolaLogger, t_log *consolaDebuggingLogger)
{
    int kernelSocket = conectar_a_servidor(kernelIP, kernelPort);
    
    // Chequeo que la conexion con el kernel haya sido exitosa
    if (kernelSocket == -1) {
        log_error(consolaLogger, "Error al intentar establecer conexión inicial con módulo Kernel");
        consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return kernelSocket;
}

void send_handshake_kernel(const int kernelSocket, t_log *consolaLogger)
{
    // Envio unicamente el handshake de la consola, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(kernelSocket, HANDSHAKE_consola);
    log_info(consolaLogger, "Se ha enviado el handshake inicial al modulo kernel");

    return;
}

void receive_handshake_kernel(const int kernelSocket, t_consola_config *consolaConfig, t_log *consolaLogger, t_log *consolaDebuggingLogger)
{
    t_handshake kernelResponse = stream_recv_header(kernelSocket);
    stream_recv_empty_buffer(kernelSocket);
    
    // Chequeo que el kernel me de el OK para continuar ejecutando e interactuando
    if (kernelResponse != HANDSHAKE_ok_continue) {
        log_error(consolaLogger, "Error al intentar establecer Handshake inicial con módulo Kernel");
        consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return;
}