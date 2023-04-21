#include <kernel-planificacion.h>

// Variables estaticas
static uint32_t pidActual;
static pthread_mutex_t mutexPid;

// Funciones privadas

// Inicializa pidActual y mutex del pidActual
static void __inicializar_estructuras_pid()
{
    pidActual = 0;
    pthread_mutex_init(&mutexPid, NULL); // Este mutex habria que destruirlo en algun otro lado

    return;
}

// Obtiene y actualiza el pidActual para asignarlo a un pcb
static uint32_t __obtener_siguiente_pid()
{
    pthread_mutex_lock(&mutexPid);
    uint32_t pid = pidActual++;
    pthread_mutex_unlock(&mutexPid);

    return pid;
}

void *encolar_en_new_a_nuevo_pcb_entrante(void *socketCliente) 
{
    int socketConsola = *((int *) socketCliente);
    free(socketCliente);

    // Recibo el handshake inicial de la consola
    t_handshake handshakeConsola = stream_recv_header(socketConsola);
    stream_recv_empty_buffer(socketConsola);
    if (handshakeConsola != HANDSHAKE_consola) {

        log_error(kernelLogger, "Error al intentar establecer conexion con consola mediante <socket %d>", socketConsola);
        log_error(kernelDebuggingLogger, "Error al intentar establecer conexion con consola mediante <socket %d>", socketConsola);

        stream_send_empty_buffer(socketConsola, HEADER_error);
        close(socketConsola);

        return NULL;
    }
    
    // Si recibo el handshake correcto, envio a la consola para que continue
    log_info(kernelDebuggingLogger, "Se recibio el handshake de la consola correctamente");
    stream_send_empty_buffer(socketConsola, HANDSHAKE_ok_continue);
    log_info(kernelDebuggingLogger, "Se ha enviado la respuesta al handshake inicial de la consola con handshake ok continue");

    // Recibo las instrucciones de la consola
    t_header consolaResponse = stream_recv_header(socketConsola);
    if (consolaResponse != HEADER_lista_instrucciones) {

        log_error(kernelLogger, "No se han podido recibir las instrucciones de la consola correctamente");
        log_error(kernelDebuggingLogger, "No se han podido recibir las instrucciones de la consola correctamente");

        stream_send_empty_buffer(socketConsola, HEADER_error);
        close(socketConsola);

        return NULL;
    }

    // Si esta todo ok, desempaqueto las instrucciones
    t_buffer *bufferInstrucciones = buffer_create();
    stream_recv_buffer(socketConsola, bufferInstrucciones); // Aca tenemos dudas acerca de de donde viene este buffer. De la consola?

    uint32_t nuevoPid = __obtener_siguiente_pid();
    t_pcb *nuevoPcb = crear_pcb(nuevoPid);
    pcb_set_socket(nuevoPcb, socketConsola);
    pcb_set_instrucciones(nuevoPcb, bufferInstrucciones);

    // Log minimo kernel numero 1
    log_info(kernelLogger, "Creación de nuevo proceso con PID <%d> en NEW", pcb_get_pid(nuevoPcb));
    log_info(kernelDebuggingLogger, "Creación de nuevo con proceso PID <%d> en NEW", pcb_get_pid(nuevoPcb));

    // Envio el pid a la consola
    t_buffer* bufferPID = buffer_create();
    buffer_pack(bufferPID, &nuevoPid, sizeof(nuevoPid));
    stream_send_buffer(socketConsola, HEADER_pid, bufferPID);
    buffer_destroy(bufferPID);

    estado_encolar_pcb(estadoNew, nuevoPcb);
    log_transicion("NULL", "NEW", pcb_get_pid(nuevoPcb));

    // AGREGAR PARTE SEMAFOROS ACA CUANDO SEA NECESARIO

    return NULL;
}