#include <kernel-conexiones.h>

// Funciones para conectar con la CPU

int conectar_a_cpu(char *cpuIP, char *cpuPort, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger)
{
    int cpuSocket = conectar_a_servidor(cpuIP, cpuPort);
    
    // Chequeo que la conexion con la CPU haya sido exitosa
    if (cpuSocket == -1) {
        log_error(kernelLogger, "Error al intentar establecer conexión inicial con módulo cpu");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return cpuSocket;
}

void send_handshake_cpu(const int cpuSocket, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(cpuSocket, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo cpu");

    return;
}

void receive_handshake_cpu(const int cpuSocket, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake cpuResponse = stream_recv_header(cpuSocket);
    stream_recv_empty_buffer(cpuSocket);
    
    // Chequeo que el kernel me de el OK para continuar ejecutando e interactuando
    if (cpuResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con módulo cpu");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return;
}

// Funciones para conectar con la memoria

int conectar_a_memoria(char *memoriaIP, char *memoriaPort, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger)
{
    int memoriaSocket = conectar_a_servidor(memoriaIP, memoriaPort);
    
    // Chequeo que la conexion con la memoria haya sido exitosa
    if (memoriaSocket == -1) {
        log_error(kernelLogger, "Error al intentar establecer conexión inicial con módulo memoria");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return memoriaSocket;
}

void send_handshake_memoria(const int memoriaSocket, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(memoriaSocket, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo memoria");

    return;
}

void receive_handshake_memoria(const int memoriaSocket, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake memoriaResponse = stream_recv_header(memoriaSocket);
    stream_recv_empty_buffer(memoriaSocket);
    
    // Chequeo que la memoria me de el OK para continuar ejecutando e interactuando
    if (memoriaResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con módulo memoria");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return;
}

// Funciones para conectar con filesystem

int conectar_a_fileSystem(char *fileSystemIP, char *fileSystemPort, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger)
{
    int fileSystemSocket = conectar_a_servidor(fileSystemIP, fileSystemPort);
    
    // Chequeo que la conexion con el filesystem haya sido exitosa
    if (fileSystemSocket == -1) {
        log_error(kernelLogger, "Error al intentar establecer conexión inicial con módulo filesystem");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return fileSystemSocket;
}

void send_handshake_fileSystem(const int fileSystemSocket, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(fileSystemSocket, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo filesystem");

    return;
}

void receive_handshake_fileSystem(const int fileSystemSocket, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake fileSystemResponse = stream_recv_header(fileSystemSocket);
    stream_recv_empty_buffer(fileSystemSocket);
    
    // Chequeo que el kernel me de el OK para continuar ejecutando e interactuando
    if (fileSystemResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con módulo filesystem");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return;
}