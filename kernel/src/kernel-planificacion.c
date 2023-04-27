#include <kernel-planificacion.h>

// Variables estaticas
static uint32_t pidActual;
static pthread_mutex_t mutexPid;

// Estados
static t_estado *estadoNew;
static t_estado *estadoReady;
static t_estado *estadoExecute;
static t_estado *estadoBlocked; 
static t_estado *estadoExit;

// Variables de uso general
static sem_t hayPcbsParaAgregarAlSistema;
static sem_t gradoMultiprog;

// Funciones privadas

// Funciones inicializacion estructuras kernel

// Inicializa pidActual y mutex del pidActual
static void __inicializar_estructuras_pid(void)
{
    pidActual = 0;
    pthread_mutex_init(&mutexPid, NULL); // Este mutex habria que destruirlo en algun otro lado

    log_info(kernelDebuggingLogger, "Se inicializa a la estructura que actualiza el numero de pid");
    
    return;
}

// Inicializa los estados de planificacion del kernel
static void __inicializar_estructuras_estados(void)
{
    estadoNew = crear_estado(NEW);
    estadoReady = crear_estado(READY);
    estadoExecute = crear_estado(EXEC);
    estadoBlocked = crear_estado(BLOCKED);
    estadoExit = crear_estado(EXIT);

    log_info(kernelDebuggingLogger, "Se inicializan los estados de planificacion del kernel");

    return;
}

static void __inicializar_semaforos(void)
{
    int valorInicialGradoMultiprog = kernel_config_get_grado_max_multiprogramacion(kernelConfig);
    
    sem_init(&hayPcbsParaAgregarAlSistema, 0, 0);
    sem_init(&gradoMultiprog, 0, valorInicialGradoMultiprog);

    log_info(kernelDebuggingLogger, "Se inicializa el grado multiprogramación en %d", valorInicialGradoMultiprog);
    log_info(kernelDebuggingLogger, "Se inicializan los semaforos para la planificacion correctamente");

    return;
}

static void __crear_hilos_planificadores(void)
{
    // PLANIFICADOR A LARZO PLAZO
    // pthread_t largoPlazoTh;
    // pthread_create(&largoPlazoTh, NULL, __planificador_largo_plazo, NULL);
    // pthread_detach(largoPlazoTh);

    // PLANIFICADOR A CORTO PLAZO
    // pthread_t cortoPlazoTh;
    // pthread_create(&cortoPlazoTh, NULL, __planificador_corto_plazo, NULL);
    // pthread_detach(cortoPlazoTh);

    return;
}

// Funciones utilitarias para la planificacion

// Obtiene y actualiza el pidActual para asignarlo a un pcb
static uint32_t __obtener_siguiente_pid()
{
    pthread_mutex_lock(&mutexPid);
    uint32_t pid = pidActual++;
    pthread_mutex_unlock(&mutexPid);

    return pid;
}

// Crea el nuevo pcb que va a ser cargado en new
static t_pcb *__crear_nuevo_pcb(int socketConsola, t_buffer *bufferInstrucciones)
{
    uint32_t nuevoPid = __obtener_siguiente_pid();
    t_pcb *nuevoPcb = crear_pcb(nuevoPid);
    pcb_set_socket(nuevoPcb, socketConsola);

    t_buffer *copiaBufferInstrucciones = buffer_create_copy(bufferInstrucciones);
    pcb_set_instrucciones(nuevoPcb, copiaBufferInstrucciones);
    
    log_info(kernelDebuggingLogger, "Se crea un nuevo pcb con pid %d y sus instrucciones correctamente", nuevoPid);
    
    return nuevoPcb;
}

// Planificadores

void *__planificador_largo_plazo(void *args)
{
    for (;;) {
        // Aguarda a que haya pcbs en new y que el grado de multiprogramacion lo permita
        sem_wait(&hayPcbsParaAgregarAlSistema); 
        sem_wait(&gradoMultiprog); // Este semaforo solo va a hacer sem_post() cuando termine algun proceso, lo que significaria que uno nuevo puede entrar

        t_pcb *pcbAReady = estado_desencolar_primer_pcb_atomic(estadoNew);

        // Pido a la memoria que inicialice al pcb y me devuelca la tabla de segmentos
        t_info_segmentos *tablaSegmentos = adapter_memoria_pedir_inicializacion_proceso(pcbAReady);

        pcb_set_estado_anterior(pcbAReady, pcb_get_estado_actual(pcbAReady));
        pcb_set_estado_actual(pcbAReady, READY);

        estado_encolar_pcb_atomic(estadoReady, pcbAReady);
        log_transicion_estados(ESTADO_NEW, ESTADO_READY, pcb_get_pid(pcbAReady));
        log_ingreso_cola_ready(estadoReady);
    }

    return NULL;
}

// Funciones publicas

// Recepcion de nuevas consolas
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
    log_info(kernelDebuggingLogger, "Se reciben las instrucciones de la consola correctamente");

    // Se crea el nuevo pcb
    t_pcb* nuevoPcb = __crear_nuevo_pcb(socketConsola, bufferInstrucciones);
    buffer_destroy(bufferInstrucciones);

    // Envio el pid a la consola
    uint32_t nuevoPid = pcb_get_pid(nuevoPcb);
    t_buffer* bufferPID = buffer_create();
    buffer_pack(bufferPID, &nuevoPid, sizeof(nuevoPid));
    stream_send_buffer(socketConsola, HEADER_pid, bufferPID);
    buffer_destroy(bufferPID);
    log_info(kernelDebuggingLogger, "Se envia el pid %d del nuevo pcb creado a la consola", nuevoPid);

    // Log minimo kernel creacion proceso
    log_info(kernelLogger, "Creación de nuevo proceso con PID <%d> en NEW", nuevoPid);
    log_info(kernelDebuggingLogger, "Creación de nuevo con proceso PID <%d> en NEW", nuevoPid);

    estado_encolar_pcb_atomic(estadoNew, nuevoPcb);
    // Log minimo cambio de estado
    log_transicion_estados(ESTADO_NULL, ESTADO_NEW, nuevoPid);
    sem_post(&hayPcbsParaAgregarAlSistema);

    return NULL;
}

void finalizar_proceso()
{
    
}

void inicializar_estructuras(void) 
{
    __inicializar_estructuras_pid();
    __inicializar_estructuras_estados();
    __inicializar_semaforos();
    __crear_hilos_planificadores();
}

