#include <kernel-conexiones.h>

// Funciones para conectar con el CPU

int conectar_a_cpu(char *ipCpu, char *puertoCpu, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger)
{
    int socketCpu = conectar_a_servidor(ipCpu, puertoCpu);
    
    // Chequeo que la conexion con la CPU haya sido exitosa
    if (socketCpu == -1) {
        log_error(kernelLogger, "Error al intentar establecer conexion inicial con modulo cpu");
        log_error(kernelDebuggingLogger, "Error al intentar establecer conexion inicial con modulo cpu");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Conexion exitosa con modulo cpu");

    return socketCpu;
}

void send_handshake_cpu(const int socketCpu, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketCpu, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo cpu");

    return;
}

void receive_handshake_cpu(const int socketCpu, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake cpuResponse = stream_recv_header(socketCpu);
    stream_recv_empty_buffer(socketCpu);
    
    // Chequeo que el kernel me de el OK para continuar ejecutando e interactuando
    if (cpuResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con modulo cpu");
        log_error(kernelDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo cpu");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Handshake inicial con modulo cpu recibido exitosamente");

    return;
}

// Funciones para conectar con la memoria

int conectar_a_memoria(char *ipMemoria, char *puertoMemoria, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger)
{
    int socketMemoria = conectar_a_servidor(ipMemoria, puertoMemoria);
    
    // Chequeo que la conexion con la memoria haya sido exitosa
    if (socketMemoria == -1) {
        log_error(kernelLogger, "Error al intentar establecer conexion inicial con modulo memoria");
        log_error(kernelDebuggingLogger, "Error al intentar establecer conexion inicial con modulo memoria");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Conexion exitosa con modulo memoria");

    return socketMemoria;
}

void send_handshake_memoria(const int socketMemoria, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketMemoria, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo memoria");

    return;
}

void receive_handshake_memoria(const int socketMemoria, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake memoriaResponse = stream_recv_header(socketMemoria);
    stream_recv_empty_buffer(socketMemoria);
    
    // Chequeo que la memoria me de el OK para continuar ejecutando e interactuando
    if (memoriaResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con modulo memoria");
        log_error(kernelDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo memoria");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Handshake inicial con modulo memoria recibido exitosamente");

    return;
}

// Funciones para conectar con filesystem

int conectar_a_filesystem(char *ipFilesystem, char *puertoFilesystem, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger)
{
    int socketFilesystem = conectar_a_servidor(ipFilesystem, puertoFilesystem);
    
    // Chequeo que la conexion con el filesystem haya sido exitosa
    if (socketFilesystem == -1) {
        log_error(kernelLogger, "Error al intentar establecer conexion inicial con modulo filesystem");
        log_error(kernelDebuggingLogger, "Error al intentar establecer conexion inicial con modulo filesystem");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Conexion exitosa con modulo filesystem");

    return socketFilesystem;
}

void send_handshake_filesystem(const int socketFilesystem, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketFilesystem, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo filesystem");

    return;
}

void receive_handshake_filesystem(const int socketFilesystem, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake fileSystemResponse = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    
    // Chequeo que el kernel me de el OK para continuar ejecutando e interactuando
    if (fileSystemResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con modulo filesystem");
        log_error(kernelDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo filesystem");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Handshake inicial con modulo filesystem recibido exitosamente");

    return;
}