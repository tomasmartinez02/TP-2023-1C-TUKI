#include <cpu-conexiones.h>
// Funciones privadas

// Envia el handshake inicial a memoria
static void __send_handshake_memoria(void)
{
    const int socketMemoria = cpu_config_get_socket_memoria(cpuConfig);

    // Envio unicamente el handshake del Cpu, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketMemoria, HANDSHAKE_cpu);
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
        log_error(cpuLogger, "Error al intentar establecer Handshake inicial con modulo Memoria");
        log_error(cpuDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo Memoria");
        cpu_destroy(cpuConfig, cpuLogger, cpuDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return;
}

int conectar_a_memoria(void)
{   
    char *ipMemoria = cpu_config_get_ip_memoria(cpuConfig);
    char *puertoMemoria = cpu_config_get_puerto_memoria(cpuConfig);
    int socketMemoria = conectar_a_servidor(ipMemoria, puertoMemoria);

    if (socketMemoria == -1 ) {
        log_error(cpuLogger, "Error al intentar establecer conexión inicial con el modulo Memoria");
        log_error(cpuDebuggingLogger, "Error al intentar establecer conexión inicial con el modulo Memoria");
        cpu_destroy(cpuConfig, cpuLogger, cpuDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    cpu_config_set_socket_memoria(cpuConfig, socketMemoria);

    // Handshakes iniciales con módulo Memoria.
    __send_handshake_memoria();
    __receive_handshake_memoria();

    return socketMemoria;
}

int inicializar_servidor_cpu(void)
{      
    char *ipCpu = cpu_config_get_ip_escucha(cpuConfig);
    char *puertoCpu= cpu_config_get_puerto_escucha(cpuConfig);

    int socketServidorCpu = iniciar_servidor(ipCpu, puertoCpu);

    // Checkeo que el servidor se haya levantado correctamente
    if (socketServidorCpu == -1)
    {
        log_error(cpuLogger, "Error al intentar iniciar servidor del Cpu");
        log_error(cpuDebuggingLogger, "Error al intentar iniciar servidor del Cpu");
        cpu_destroy(cpuConfig, cpuLogger, cpuDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    return socketServidorCpu;
}

void aceptar_conexion_kernel(int socketEscucha)
{   
    struct sockaddr cliente = {0};
    socklen_t len = sizeof(cliente);
        
    const int socketKernel = accept(socketEscucha, &cliente, &len);
        
    if (socketKernel > -1) {

        // Recibo handshake
        t_handshake handshakeKernel = stream_recv_header(socketKernel);
        if (handshakeKernel == HANDSHAKE_kernel) {
            stream_recv_empty_buffer(socketKernel);
            cpu_config_set_socket_kernel(cpuConfig, socketKernel);
                
            // Respondo handshake ok
            stream_send_empty_buffer(socketKernel, HANDSHAKE_ok_continue);
        }
        else {
            log_error(cpuLogger, "Error al intentar establecer conexión con Kernel mediante <socket %d>", socketKernel);
            log_error(cpuDebuggingLogger, "Error al intentar establecer conexión con Kernel mediante <socket %d>", socketKernel);
        }

    } else {

        log_error(cpuLogger, "Error al aceptar conexión: %s", strerror(errno));
        log_error(cpuDebuggingLogger, "Error al aceptar conexión: %s", strerror(errno));
    }
    
    return;
}

