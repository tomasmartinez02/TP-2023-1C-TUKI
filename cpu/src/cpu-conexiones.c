#include <cpu-conexiones.h>
// Funciones privadas

// Envia el handshake inicial a memoria
static void __send_handshake_memoria(void)
{
    const int socketMemoria = cpu_config_get_socket_memoria(cpuConfig);

    // Envio unicamente el handshake del Filesystem, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketMemoria, HANDSHAKE_cpu);
    log_info(cpuLogger, "Se ha enviado el handshake inicial al modulo Memoria");
    return;
}

// Espera y recibe la respuesta al handshake inicial del modulo memoria
static void __receive_handshake_memoria(void)
{   
    int socketMemoria = cpu_config_get_socket_memoria(cpuConfig);

    // Aguardo respuesta del modulo Memoria
    t_handshake respuestaMemoria = stream_recv_header(socketMemoria);
    stream_recv_empty_buffer(socketMemoria);
    
    // Chequeo que la memoria me de el OK para continuar ejecutando e interactuando
    if (respuestaMemoria != HANDSHAKE_ok_continue) {
        log_error(cpuLogger, "Error al intentar establecer Handshake inicial con módulo Memoria");
        log_error(cpuDebuggingLogger, "Error al intentar establecer Handshake inicial con módulo Memoria");
        cpu_destroy(cpuConfig, cpuLogger, cpuDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(cpuDebuggingLogger, "Handshake inicial con modulo Memoria recibido exitosamente");

    return;
}

int conectar_a_memoria(void)
{   
    char *memoriaIP = cpu_config_get_ip_memoria;
    char *memoriaPuerto = cpu_config_get_puerto_memoria;
    int socketMemoria = conectar_a_servidor(memoriaIP, memoriaPuerto);

    if (socketMemoria == -1 ) {
        log_error(cpuLogger, "Error al intentar establecer conexión inicial con el módulo Memoria");
        cpu_destroy(cpuConfig, cpuLogger, cpuDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    cpu_config_set_socket_memoria(cpuConfig, socketMemoria);
    log_info(cpuDebuggingLogger, "Socket creado exitosamente con modulo Memoria");

    // Handshakes iniciales con módulo Memoria.
    __send_handshake_memoria();
    __receive_handshake_memoria();

    return socketMemoria;
}

int inicializar_servidor_cpu(void)
{
    return;
}

void aceptar_conexion_kernel(int socketEscucha)
{
    return;
}

