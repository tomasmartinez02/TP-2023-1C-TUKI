#include <kernel-estados.h>

//Estados
t_estado* estadoNew;
t_estado* estadoReady;
t_estado* estadoExecute;
t_estado* estadoExit;
t_estado* estadoBlocked;

t_estado* crear_estado(t_nombre_estado nombre) // A medida que avancemos le vamos a agregar cosas
{
    t_estado* estado = malloc(sizeof(*estado));
    estado->listaProcesos = list_create();
    estado->nombreEstado = nombre;

    estado->mutexEstado = malloc(sizeof(*(estado->mutexEstado)));
    pthread_mutex_init(estado->mutexEstado, NULL);

    return estado;
}

void destruir_estado(t_estado* estado) 
{
    if (list_is_empty(estado->listaProcesos)) {
        list_destroy(estado->listaProcesos);
    } 
    else {
        list_destroy_and_destroy_elements(estado->listaProcesos, (void*)destruir_pcb); 
    }

    pthread_mutex_destroy(estado->mutexEstado);
    free(estado->mutexEstado);
    // free(estado->nombreEstado); nos da error, creemos que no hay que hacerlo por ser un enum
    free(estado);
}

void inicializar_estructuras_estados(void)
{
    estadoNew = crear_estado(NEW);
    estadoReady = crear_estado(READY);
    estadoExecute = crear_estado(EXEC);
    estadoExit = crear_estado(EXIT);
    estadoBlocked = crear_estado(BLOCKED);
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

void estado_encolar_pcb(t_estado* estadoDestino, t_pcb* pcbAAgregar)
{
    pthread_mutex_lock(estado_get_mutex(estadoDestino));
    list_add(estado_get_list(estadoDestino), pcbAAgregar);
    pthread_mutex_unlock(estado_get_mutex(estadoDestino));
}

t_list* estado_get_list(t_estado* estado) 
{
    return estado->listaProcesos;
}

pthread_mutex_t* estado_get_mutex(t_estado* estado) 
{
    return estado->mutexEstado;
}
