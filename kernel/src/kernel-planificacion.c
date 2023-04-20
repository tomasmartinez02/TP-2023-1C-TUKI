#include <kernel-planificacion.h>

void inicializar_pid()
{
    pidAnterior = -1;
    pthread_mutex_init(&mutexPid, NULL); // Este mutex habria que destruirlo en algun otro lado
}

uint32_t obtener_siguiente_pid()
{
    pthread_mutex_lock(&mutexPid);
    uint32_t pid = pidAnterior++;
    pthread_mutex_unlock(&mutexPid);

    return pid;
}

void* encolar_en_new_a_nuevo_pcb_entrante(void* socket) 
{
    int socketProceso = *((int*)socket); // Aca tendriamos que usar el uint32_t ??
    free(socket);

    t_buffer* instructionsBuffer = buffer_create();
    stream_recv_buffer(socketProceso, instructionsBuffer); // Aca tenemos dudas acerca de de donde viene este buffer. De la consola?
    t_buffer* instructionsBufferCopy = buffer_create_copy(instructionsBuffer);
    buffer_destroy(instructionsBuffer);

    uint32_t nuevoPid = obtener_siguiente_pid();
    t_pcb* nuevoPcb = crear_pcb(nuevoPid);
    pcb_set_socket(nuevoPcb, socketProceso);
    pcb_set_instrucciones(nuevoPcb, instructionsBufferCopy);

    log_info(kernelLogger, "Creación de nuevo proceso ID %d mediante <socket %d>", pcb_get_pid(nuevoPcb), socketProceso);
    log_info(kernelDebuggingLogger, "Creación de nuevo proceso ID %d mediante <socket %d>", pcb_get_pid(nuevoPcb), socketProceso);

    t_buffer* bufferPID = buffer_create();
    buffer_pack(bufferPID, &nuevoPid, sizeof(nuevoPid));
    stream_send_buffer(socketProceso, HEADER_pid, bufferPID);
    buffer_destroy(bufferPID);

    estado_encolar_pcb(estadoNew, nuevoPcb);
    log_transicion("NULL", "NEW", pcb_get_pid(nuevoPcb));

    // AGREGAR PARTE SEMAFOROS ACA CUANDO SEA NECESARIO

    return NULL;
}