#include <memoria-conexiones.h>

// Levantar servidor de instancias CPU
int inicializar_servidor_memoria_para_cpu(void)
{
    char *ipMemoria = memoria_config_get_ip_escucha(memoriaConfig);
    char *puertoMemoria = memoria_config_get_puerto_escucha(memoriaConfig);
    int tempMemoriaSocketServerEscuchaCPU = iniciar_servidor(ipMemoria, puertoMemoria);

    if (tempMemoriaSocketServerEscuchaCPU == -1) {
        
        log_error(memoriaLogger, "Error al intentar iniciar servidor de Memoria para la CPU");
        log_error(memoriaDebuggingLogger, "Error al intentar iniciar servidor de Memoria para la CPU");
        memoria_destroy(memoriaConfig, memoriaLogger, memoriaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(memoriaDebuggingLogger, "Se ha inicializado el servidor de escucha de Memoria para CPU correctamente");
    
    return tempMemoriaSocketServerEscuchaCPU;
}

void aceptar_conexiones_memoria_con_cpu(const int socketEscuchaCPU) 
{ 
    struct sockaddr cliente = {0};
    socklen_t len = sizeof(cliente);
    
    log_info(memoriaDebuggingLogger, "A la escucha de nuevas conexiones con cpu en puerto %d", socketEscuchaCPU);
    
    const int clienteAceptado = accept(socketEscuchaCPU, &cliente, &len);
        
    if (clienteAceptado > -1) {
        t_handshake handshakeCPU = stream_recv_header(clienteAceptado);

        if (handshakeCPU == HANDSHAKE_cpu) {
            log_info(memoriaDebuggingLogger, "Se recibio el handshake de la cpu correctamente");
            stream_send_empty_buffer(clienteAceptado, HANDSHAKE_ok_continue);
            log_info(memoriaDebuggingLogger, "Se ha enviado la respuesta al handshake inicial de la cpu con handshake ok continue");
        }
        else {
            log_error(memoriaLogger, "Error al intentar establecer conexión con la cpu mediante <socket %d>", clienteAceptado);
            log_error(memoriaDebuggingLogger, "Error al intentar establecer conexión con la cpu mediante <socket %d>", clienteAceptado);
        }
        } 
    else {
        log_error(memoriaLogger, "Error al aceptar conexión: %s", strerror(errno));
        log_error(memoriaDebuggingLogger, "Error al aceptar conexión: %s", strerror(errno));
    }
}

// Levantar servidor de instancias Filesystem
int inicializar_servidor_memoria_para_filesystem(void)
{
    char *ipFilesystem = mermoria_config_get_ip_escucha(memoriaConfig);
    char *puertoFilesystem = memoria_config_get_puerto_escucha(memoriaConfig);
    int tempMemoriaSocketServerEscuchaFilesystem = iniciar_servidor(ipFilesystem, puertoFilesystem);

    if (tempMemoriaSocketServerEscuchaFilesystem == -1) {
        
        log_error(memoriaLogger, "Error al intentar iniciar servidor de Memoria para filesystem");
        log_error(memoriaDebuggingLogger, "Error al intentar iniciar servidor de Memoria para filesystem");
        memoria_destroy(memoriaConfig, memoriaLogger, memoriaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(memoriaDebuggingLogger, "Se ha inicializado el servidor de escucha de Memoria para filesystem correctamente");
    
    return tempMemoriaSocketServerEscuchaFilesystem;
}

void aceptar_conexiones_memoria_con_filesystem(const int socketEscuchaFilesystem) 
{ 
    struct sockaddr cliente = {0};
    socklen_t len = sizeof(cliente);
    
    log_info(memoriaDebuggingLogger, "A la escucha de nuevas conexiones con filesystem en puerto %d", socketEscuchaFilesystem);
    
    const int clienteAceptado = accept(socketEscuchaFilesystem, &cliente, &len);
        
    if (clienteAceptado > -1) {
        t_handshake handshakeFilesystem = stream_recv_header(clienteAceptado);

        if (handshakeFilesystem == HANDSHAKE_filesystem) {
            log_info(memoriaDebuggingLogger, "Se recibio el handshake de la filesystem correctamente");
            stream_send_empty_buffer(clienteAceptado, HANDSHAKE_ok_continue);
            log_info(memoriaDebuggingLogger, "Se ha enviado la respuesta al handshake inicial del filesystem con handshake ok continue");
        }
        else {
            log_error(memoriaLogger, "Error al intentar establecer conexión con el filesystem mediante <socket %d>", clienteAceptado);
            log_error(memoriaDebuggingLogger, "Error al intentar establecer conexión con el filesystem mediante <socket %d>", clienteAceptado);
        }
        } 
    else {
        log_error(memoriaLogger, "Error al aceptar conexión: %s", strerror(errno));
        log_error(memoriaDebuggingLogger, "Error al aceptar conexión: %s", strerror(errno));
    }
}

// Levantar servidor de instancias Kernel
int inicializar_servidor_memoria_para_kernel(void)
{
    char *ipKernel = mermoria_config_get_ip_escucha(memoriaConfig);
    char *puertoKernel = memoria_config_get_puerto_escucha(memoriaConfig);
    int tempMemoriaSocketServerEscuchaKernel = iniciar_servidor(ipKernel, puertoKernel);

    if (tempMemoriaSocketServerEscuchaKernel == -1) {
        
        log_error(memoriaLogger, "Error al intentar iniciar servidor de Memoria para kernel");
        log_error(memoriaDebuggingLogger, "Error al intentar iniciar servidor de Memoria para kernel");
        memoria_destroy(memoriaConfig, memoriaLogger, memoriaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(memoriaDebuggingLogger, "Se ha inicializado el servidor de escucha de Memoria para kernel correctamente");
    
    return tempMemoriaSocketServerEscuchaKernel;
}

void aceptar_conexiones_memoria_con_kernel(const int socketEscuchaKernel) 
{ 
    struct sockaddr cliente = {0};
    socklen_t len = sizeof(cliente);
    
    log_info(memoriaDebuggingLogger, "A la escucha de nuevas conexiones con kernel en puerto %d", socketEscuchaKernel);
    
    const int clienteAceptado = accept(socketEscuchaKernel, &cliente, &len);
        
    if (clienteAceptado > -1) {
        t_handshake handshakeKernel = stream_recv_header(clienteAceptado);

        if (handshakeKernel == HANDSHAKE_kernel) {
            log_info(memoriaDebuggingLogger, "Se recibio el handshake del kernel correctamente");
            stream_send_empty_buffer(clienteAceptado, HANDSHAKE_ok_continue);
            log_info(memoriaDebuggingLogger, "Se ha enviado la respuesta al handshake inicial del kernel con handshake ok continue");
        }
        else {
            log_error(memoriaLogger, "Error al intentar establecer conexión con el kernel mediante <socket %d>", clienteAceptado);
            log_error(memoriaDebuggingLogger, "Error al intentar establecer conexión con el kernel mediante <socket %d>", clienteAceptado);
        }
        } 
    else {
        log_error(memoriaLogger, "Error al aceptar conexión: %s", strerror(errno));
        log_error(memoriaDebuggingLogger, "Error al aceptar conexión: %s", strerror(errno));
    }
}