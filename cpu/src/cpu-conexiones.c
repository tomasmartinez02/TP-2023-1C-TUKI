#include <cpu-conexiones.h>

int conectar_a_memoria(char *memoriaIP, char *memoriaPuerto, t_cpu_config *cpuConfig ,t_log *cpuLogger, t_log *cpuDebuggingLogger)
{
    int memoriaSocket = conectar_a_servidor(memoriaIP, memoriaPuerto);

    if (memoriaSocket == -1 ) {
        log_error(cpuLogger, "Error al intentar establecer conexión inicial con el módulo Memoria");
        cpu_destroy(cpuConfig, cpuLogger, cpuDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return memoriaSocket;
}

void send_handshake_memoria(const int memoriaSocket, t_log *cpuLogger)
{
    // Envio unicamente el handshake del cpu, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(memoriaSocket, HANDSHAKE_cpu);
    log_info(cpuLogger, "Se ha enviado el handshake inicial al modulo memoria");

    return;
}

void receive_handshake_memoria(const int memoriaSocket, t_cpu_config* cpuConfig, t_log* cpuLogger, t_log* cpuDebuggingLogger)
{
    t_handshake respuestaMemoria = stream_recv_header(memoriaSocket);
    stream_recv_empty_buffer(memoriaSocket);
    
    // Chequeo que la memoria me de el OK para continuar ejecutando e interactuando
    if (respuestaMemoria != HANDSHAKE_ok_continue) {
        log_error(cpuLogger, "Error al intentar establecer Handshake inicial con módulo Kernel");
        cpu_destroy(cpuConfig, cpuLogger, cpuDebuggingLogger);
        exit(EXIT_FAILURE);
    }
}