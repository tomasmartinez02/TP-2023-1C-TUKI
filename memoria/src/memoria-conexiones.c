#include <memoria-conexiones.h>

// Funciones privadas

// Envia el handshake inicial e imprime los loggers
static void __handshake_iniciales(char* modulo, int clienteAceptado)
{
    stream_send_empty_buffer(clienteAceptado, HANDSHAKE_ok_continue);
}


// Funciones publicas
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
    
    return tempMemoriaSocketServerEscucha;
}

// Aceptar conexiones con CPU, filesystem y kernel

void aceptar_conexiones(int socketEscucha) 
{
    bool cpuSinAtender = true;
    bool kernelSinAtender = true;
    bool filesystemSinAtender = true;

    struct sockaddr cliente = {0};
    socklen_t len = sizeof(cliente);

    while (cpuSinAtender || kernelSinAtender || filesystemSinAtender) {

        const int clienteAceptado = accept(socketEscucha, &cliente, &len);
    
        if (clienteAceptado > -1) {

            t_handshake handshakeRecibido = stream_recv_header(clienteAceptado);
            stream_recv_empty_buffer(clienteAceptado);

            switch (handshakeRecibido) {
                case HANDSHAKE_cpu:
                    __handshake_iniciales("CPU", clienteAceptado);
                    memoria_config_set_socket_cpu(memoriaConfig, clienteAceptado);
                    cpuSinAtender = false;
                    break;
            
                case HANDSHAKE_kernel:
                    __handshake_iniciales("kernel", clienteAceptado);
                    memoria_config_set_socket_kernel(memoriaConfig, clienteAceptado);
                    kernelSinAtender = false;
                    break;

                case HANDSHAKE_filesystem:
                    __handshake_iniciales("filesystem", clienteAceptado);
                    memoria_config_set_socket_filesystem(memoriaConfig, clienteAceptado);
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

