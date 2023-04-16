#include <memoria-conexiones.h>

// Funciones privadas

// Envia el handshake inicial e imprime los loggers
static void __handshake_iniciales(char* modulo, int clienteAceptado)
{
    log_info(memoriaDebuggingLogger, "Se recibio el handshake de %s correctamente", modulo);
    stream_send_empty_buffer(clienteAceptado, HANDSHAKE_ok_continue);
    log_info(memoriaDebuggingLogger, "Se ha enviado la respuesta al handshake inicial de %s con handshake ok continue", modulo);
}

// Levantar servidor de escucha

int inicializar_servidor_memoria(void)
{
    char *ipMemoria = memoria_config_get_ip_escucha(memoriaConfig);
    char *puertoMemoria = memoria_config_get_puerto_escucha(memoriaConfig);
    int tempMemoriaSocketServerEscucha = iniciar_servidor(ipMemoria, puertoMemoria);

    if (tempMemoriaSocketServerEscucha == -1) {
        
        log_error(memoriaLogger, "Error al intentar iniciar servidor de Memoria");
        log_error(memoriaDebuggingLogger, "Error al intentar iniciar servidor de Memoria");
        memoria_destroy(memoriaConfig, memoriaLogger, memoriaDebuggingLogger);
        exit(EXIT_FAILURE);
    }

    log_info(memoriaDebuggingLogger, "Se ha inicializado el servidor de escucha de kernel, filesystem y cpu correctamente");
    
    return tempMemoriaSocketServerEscucha;
}

// Aceptar conexiones con CPU, filesystem y kernel

void aceptar_conexiones(int socketEscucha) {


    bool cpuSinAtender = true;
    bool kernelSinAtender = true;
    bool filesystemSinAtender = true;

    struct sockaddr cliente = {0};
    socklen_t len = sizeof(cliente);
    
    log_info(memoriaDebuggingLogger, "A la escucha de nuevas conexiones en puerto %d", socketEscucha);

    while (cpuSinAtender || kernelSinAtender || filesystemSinAtender) {

        const int clienteAceptado = accept(socketEscucha, &cliente, &len);
    
        if (clienteAceptado > -1) {

            t_handshake handshakeRecibido = stream_recv_header(clienteAceptado);

            switch (handshakeRecibido) {
                case HANDSHAKE_cpu:
                    __handshake_iniciales("CPU", clienteAceptado);
                    cpuSinAtender = false;
                    break;
            
                case HANDSHAKE_kernel:
                    __handshake_iniciales("kernel", clienteAceptado);
                    kernelSinAtender = false;
                    break;

                case HANDSHAKE_filesystem:
                    __handshake_iniciales("filesystem", clienteAceptado);
                    filesystemSinAtender = false;
                    break;

                default:
                log_error(memoriaLogger, "Error al aceptar conexión: %s", strerror(errno));
                log_error(memoriaDebuggingLogger, "Error al aceptar conexión: %s", strerror(errno));

                break;
            }
        } 
        else {
            log_error(memoriaLogger, "Error al aceptar conexión: %s", strerror(errno));
            log_error(memoriaDebuggingLogger, "Error al aceptar conexión: %s", strerror(errno));
        }
    }
    
    return;
}

