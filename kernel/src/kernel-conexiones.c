#include <kernel-conexiones.h>

// Funciones privadas

// Abstrae la logica para conectarse a los distintos modulos
static int __conectar_a_modulo(char *nombreModulo, char *(*get_ip_modulo)(t_kernel_config *), char *(*get_puerto_modulo)(t_kernel_config *), 
void (*set_socket_modulo)(t_kernel_config *, int), void (*send_handshake_modulo)(int, t_log *), void (*receive_handshake_modulo)(const int, t_kernel_config *, t_log *, t_log *))
{
    char *ipModulo = get_ip_modulo(kernelConfig);
    char *puertoModulo = get_puerto_modulo(kernelConfig);

    const int socketModulo = conectar_a_servidor(ipModulo, puertoModulo);
    
    // Chequeo que la conexion con la CPU haya sido exitosa
    if (socketModulo == -1) {
        log_error(kernelLogger, "Error al intentar establecer conexion inicial con modulo %s", nombreModulo);
        log_error(kernelDebuggingLogger, "Error al intentar establecer conexion inicial con modulo %s", nombreModulo);
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    set_socket_modulo(kernelConfig, socketModulo);
    log_info(kernelDebuggingLogger, "Socket creado exitosamente con modulo %s", nombreModulo);

    // Handshakes iniciales con modulo Cpu
    send_handshake_modulo(socketModulo, kernelDebuggingLogger);
    receive_handshake_modulo(socketModulo, kernelConfig, kernelLogger, kernelDebuggingLogger);

    log_info(kernelDebuggingLogger, "Conexion con modulo %s establecida exitosamente", nombreModulo);

    return socketModulo;
}

// Crea los hilos para manejar las conexiones de las consolas
static void __crear_hilo_handler_conexion_entrante(int *socket) 
{
    pthread_t threadSuscripcion;
    pthread_create(&threadSuscripcion, NULL, encolar_en_new_a_nuevo_pcb_entrante, (void*)socket);
    pthread_detach(threadSuscripcion);
    return;
}

// Funciones publicas

// Funciones para conectar con el CPU

int conectar_a_cpu(void)
{   
    return _conectar_a_modulo("Cpu", kernel_config_get_ip_cpu, kernel_config_get_puerto_cpu, kernel_config_set_socket_cpu, send_handshake_cpu, receive_handshake_cpu);__conectar_a_modulo("Cpu", kernel_config_get_ip_cpu, kernel_config_get_puerto_cpu, kernel_config_set_socket_cpu, send_handshake_cpu, receive_handshake_cpu);
}

void send_handshake_cpu(const int socketCpu, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketCpu, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo Cpu");

    return;
}

void receive_handshake_cpu(const int socketCpu, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake cpuResponse = stream_recv_header(socketCpu);
    stream_recv_empty_buffer(socketCpu);
    
    // Chequeo que el kernel me de el OK para continuar ejecutando e interactuando
    if (cpuResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con modulo Cpu");
        log_error(kernelDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo Cpu");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Handshake inicial con modulo Cpu recibido exitosamente");

    return;
}

// Funciones para conectar con la memoria

int conectar_a_memoria()
{   
    return __conectar_a_modulo("Memoria", kernel_config_get_ip_memoria, kernel_config_get_puerto_memoria, kernel_config_set_socket_memoria, send_handshake_memoria, receive_handshake_memoria);
}

void send_handshake_memoria(const int socketMemoria, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketMemoria, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo Memoria");

    return;
}

void receive_handshake_memoria(const int socketMemoria, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake memoriaResponse = stream_recv_header(socketMemoria);
    stream_recv_empty_buffer(socketMemoria);
    
    // Chequeo que la memoria me de el OK para continuar ejecutando e interactuando
    if (memoriaResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con modulo Memoria");
        log_error(kernelDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo Memoria");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Handshake inicial con modulo Memoria recibido exitosamente");

    return;
}

// Funciones para conectar con filesystem
int conectar_a_filesystem()
{   
    return __conectar_a_modulo("Filesystem", kernel_config_get_ip_filesystem, kernel_config_get_puerto_filesystem, kernel_config_set_socket_filesystem, send_handshake_filesystem, receive_handshake_filesystem);
}

void send_handshake_filesystem(const int socketFilesystem, t_log *kernelLogger)
{
    // Envio unicamente el handshake del kernel, sin ningun buffer e informacion adicional
    stream_send_empty_buffer(socketFilesystem, HANDSHAKE_kernel);
    log_info(kernelLogger, "Se ha enviado el handshake inicial al modulo Filesystem");

    return;
}

void receive_handshake_filesystem(const int socketFilesystem, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger)
{
    t_handshake fileSystemResponse = stream_recv_header(socketFilesystem);
    stream_recv_empty_buffer(socketFilesystem);
    
    // Chequeo que el kernel me de el OK para continuar ejecutando e interactuando
    if (fileSystemResponse != HANDSHAKE_ok_continue) {
        log_error(kernelLogger, "Error al intentar establecer Handshake inicial con modulo Filesystem");
        log_error(kernelDebuggingLogger, "Error al intentar establecer Handshake inicial con modulo Filesystem");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Handshake inicial con modulo Filesystem recibido exitosamente");

    return;
}

// Levantar servidor de instancias Consola
int inicializar_servidor_kernel(void)
{
    char *ipKernel = kernel_config_get_ip_escucha(kernelConfig);
    char *puertoKernel = kernel_config_get_puerto_escucha(kernelConfig);
    int tempKernelSocketServerEscucha = iniciar_servidor(ipKernel, puertoKernel);

    if (tempKernelSocketServerEscucha == -1) {
        
        log_error(kernelLogger, "Error al intentar iniciar servidor del Kernel");
        log_error(kernelDebuggingLogger, "Error al intentar iniciar servidor del Kernel");
        kernel_destroy(kernelConfig, kernelLogger, kernelDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(kernelDebuggingLogger, "Se ha inicializado el servidor de escucha de consolas correctamente");
    
    return tempKernelSocketServerEscucha;
}


void aceptar_conexiones_kernel(const int socketEscucha) 
{ 
    struct sockaddr cliente = {0};
    socklen_t len = sizeof(cliente);
    
    log_info(kernelDebuggingLogger, "A la escucha de nuevas conexiones en puerto %d", socketEscucha);
    
    for (;;) {
        
        const int clienteAceptado = accept(socketEscucha, &cliente, &len);
        
        if (clienteAceptado > -1) {
            
            int* socketCliente = malloc(sizeof(*socketCliente)); // Ojo con manejo de enteros
            *socketCliente = clienteAceptado;
            __crear_hilo_handler_conexion_entrante(socketCliente);

            t_handshake handshakeConsola = stream_recv_header(clienteAceptado);

            if (handshakeConsola == HANDSHAKE_consola) {
                log_info(kernelDebuggingLogger, "Se recibio el handshake de la consola correctamente");
                stream_send_empty_buffer(clienteAceptado, HANDSHAKE_ok_continue);
                log_info(kernelDebuggingLogger, "Se ha enviado la respuesta al handshake inicialde  la consola con handshake ok continue");
            }
            else {
                log_error(kernelLogger, "Error al intentar establecer conexion con consola mediante <socket %d>", clienteAceptado);
                log_error(kernelDebuggingLogger, "Error al intentar establecer conexion con consola mediante <socket %d>", clienteAceptado);
            }
        } 
        else {
            log_error(kernelLogger, "Error al aceptar conexion: %s", strerror(errno));
            log_error(kernelDebuggingLogger, "Error al aceptar conexion: %s", strerror(errno));
        }
    }
}