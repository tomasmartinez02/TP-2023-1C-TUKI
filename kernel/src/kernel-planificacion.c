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
static sem_t gradoMultiprogramacion;
static timestamp *tiempoActual;
static uint32_t alfaHRRN;

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

// Inicializa los semaforos para la planificacion
static void __inicializar_semaforos(void)
{
    int valorInicialGradoMultiprogramacion = kernel_config_get_grado_max_multiprogramacion(kernelConfig);
    
    sem_init(&gradoMultiprogramacion, 0, valorInicialGradoMultiprogramacion);

    log_info(kernelDebuggingLogger, "Se inicializa el grado multiprogramación en %d", valorInicialGradoMultiprogramacion);
    log_info(kernelDebuggingLogger, "Se inicializan los semaforos para la planificacion correctamente");

    return;
}

// Inicializa el valor de alfa para los calculos de HRRN
static void __inicializar_alfa_hrrn(void)
{
    alfaHRRN = kernel_config_get_hrrn_alfa(kernelConfig);
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


// TRANSICIONES DE ESTADOS

// Encola, actualiza pcb y loggea transicion de estados
static void __pcb_pasar_de_estado(t_pcb* pcb, t_estado *nuevoEstado, char *stringEstadoViejo, char *stringEstadoNuevo)
{   
    t_nombre_estado nombreNuevoEstado = nuevoEstado->nombreEstado;
    
    pcb_set_estado_anterior(pcb, pcb_get_estado_actual(pcb));
    pcb_set_estado_actual(pcb, nombreNuevoEstado);

    estado_encolar_pcb_atomic(nuevoEstado, pcb);
    log_transicion_estados(stringEstadoViejo, stringEstadoNuevo, pcb_get_pid(pcb));

    return;
}

// Funcion generica para pasar un pcb a ready desde cualquier estado
static void __pcb_pasar_a_ready(t_pcb* pcbAReady, char *stringEstadoViejo)
{
    __pcb_pasar_de_estado(pcbAReady, estadoReady, stringEstadoViejo, ESTADO_READY);
    pcb_set_tiempo_llegada_ready(pcbAReady);
    log_ingreso_cola_ready(estadoReady);

    return;
}

// Pasa un pcb de New a Ready
static void __pcb_pasar_de_new_a_ready(t_pcb* pcbAReady)
{
    __pcb_pasar_a_ready(pcbAReady, ESTADO_NEW);

    return;
}

// Pasa un pcb de Blocked a Ready
static void __pcb_pasar_de_blocked_a_ready(t_pcb* pcbAReady)
{   
    pcb_set_proceso_bloqueado_o_terminado(pcbAReady, false);
    __pcb_pasar_a_ready(pcbAReady, ESTADO_BLOCKED);

    return;
}

// Pasa un pcb de Running a Ready
static void __pcb_pasar_de_running_a_ready(t_pcb* pcbAReady)
{
    __pcb_pasar_a_ready(pcbAReady, ESTADO_EXECUTE);

    return;
}

// Pasa un pcb de Ready a Running
static void __pcb_pasar_de_ready_a_running(t_pcb* pcbARunning)
{   
    __pcb_pasar_de_estado(pcbARunning, estadoExecute, ESTADO_READY, ESTADO_EXECUTE);

    return;
}

// Pasa un pcb de Running a Blocked
static void __pcb_pasar_de_running_a_blocked(t_pcb* pcbABlocked)
{   
    pcb_set_proceso_bloqueado_o_terminado(pcbABlocked, true);
    __pcb_pasar_de_estado(pcbABlocked, estadoBlocked, ESTADO_EXECUTE, ESTADO_BLOCKED);

    return;
}

// Pasa un pcb de Null a New
static void __pcb_pasar_de_null_a_new(t_pcb* pcbANew)
{   
    uint32_t pid = pcb_get_pid(pcbANew);

    // Log minimo kernel creacion proceso
    log_info(kernelLogger, "Creación de nuevo proceso con PID <%d> en NEW", pid);
    log_info(kernelDebuggingLogger, "Creación de nuevo con proceso PID <%d> en NEW", pid);

    __pcb_pasar_de_estado(pcbANew, estadoNew, ESTADO_NULL, ESTADO_NEW);

    return;
}

// Funcion generica para pasar un pcb a Exit desde cualquier estado
static void __pcb_pasar_a_exit(t_pcb* pcbAExit, char *stringEstadoViejo)
{
    pcb_set_estado_finalizacion(pcbAExit, pcb_get_estado_actual(pcbAExit));
    pcb_set_proceso_bloqueado_o_terminado(pcbAExit, true);
    __pcb_pasar_de_estado(pcbAExit, estadoExit, stringEstadoViejo, ESTADO_EXIT);

    return;
}

// Pasa un pcb de New a Exit
static void __pcb_pasar_de_new_a_exit(t_pcb* pcbAExit)
{
    __pcb_pasar_a_exit(pcbAExit, ESTADO_NEW);

    return;
}

// Pasa un pcb de Running a Exit
static void __pcb_pasar_de_running_a_exit(t_pcb* pcbAExit)
{
    __pcb_pasar_a_exit(pcbAExit, ESTADO_EXECUTE);

    return;
}

// Pasa un pcb de Blocked a Exit
static void __pcb_pasar_de_blocked_a_exit(t_pcb* pcbAExit)
{
    __pcb_pasar_a_exit(pcbAExit, ESTADO_BLOCKED);

    return;
}

// Termina el proceso del cual se le pasa el PCB
static void __terminar_proceso(t_pcb* pcbFinalizado, char *motivoFinalizacion)
{
    switch (pcb_get_estado_actual(pcbFinalizado))
    {
        case NEW:
            __pcb_pasar_de_new_a_exit(pcbFinalizado);
            break;

        case EXEC: 
            __pcb_pasar_de_running_a_exit(pcbFinalizado);
            break;
            
        case BLOCKED:
            __pcb_pasar_de_blocked_a_exit(pcbFinalizado);
            break;

        default:
            //error
            break;
    }
    
    stream_send_empty_buffer(pcb_get_socket(pcbFinalizado), HEADER_proceso_terminado); // Da aviso a la consola de que finalizo la ejecucición
    log_finalizacion_proceso(pcbFinalizado, motivoFinalizacion);
}

// Planificadores
// Planificador de largo plazo

// Libera los pcbs que se encuentran en el estado EXIT
static void* __liberar_pcbs_en_exit(void* args)
{
    for (;;) {
        t_pcb *pcbALiberar = estado_desencolar_primer_pcb_atomic(estadoExit);
        adapter_memoria_finalizar_proceso(pcbALiberar);
        log_info(kernelDebuggingLogger, "Se liberan las estructuras del proceso PID <%d> en EXIT", pcb_get_pid(pcbALiberar));
        destruir_pcb(pcbALiberar);
        sem_post(&gradoMultiprogramacion);
    }

    return NULL;
}

// Instancia al liberador de pcbs en exit y pasa los pcbs de new a ready
static void *__planificador_largo_plazo(void *args)
{
    pthread_t liberarPcbsEnExitTh;
    pthread_create(&liberarPcbsEnExitTh, NULL, (void*) __liberar_pcbs_en_exit, NULL);
    pthread_detach(liberarPcbsEnExitTh);

    for (;;) {
        // Aguarda a que haya pcbs en new y que el grado de multiprogramacion lo permita
        t_pcb *pcbAReady = estado_desencolar_primer_pcb_atomic(estadoNew);
        sem_wait(&gradoMultiprogramacion); // Este semaforo solo va a hacer sem_post() cuando termine algun proceso, lo que significaria que uno nuevo puede entrar


        // Pido a la memoria que inicialice al pcb y me devuelca la tabla de segmentos
        t_buffer *tablaSegmentos = adapter_memoria_pedir_inicializacion_proceso(pcbAReady);
        pcb_set_tabla_segmentos(pcbAReady, tablaSegmentos);

        if (tablaSegmentos == NULL) {
            
        }
        else {
           __pcb_pasar_de_new_a_ready(pcbAReady);
        }
    }

    return NULL;
}

// Planificador de corto plazo

static bool __algoritmo_es_fifo()
{
    if (strcmp(kernel_config_get_algoritmo_planificacion(kernelConfig), "FIFO")  == 0 ){
        return true;
    } else return false;
}

static uint32_t __calcular_valor_hrrn(t_pcb *pcb)
{
    double tiempoEnReady;
    tiempoEnReady = obtener_diferencial_de_tiempo_en_milisegundos(tiempoActual, pcb->tiempoLlegadaReady);
    return (uint32_t) (tiempoEnReady + pcb->estimadoProxRafaga) / pcb->estimadoProxRafaga;
}

static t_pcb* _comparar_pcb_segun_hrrn(t_pcb *pcb1, t_pcb *pcb2)
{
    uint32_t estimacionPcb1 = 0;
    uint32_t estimacionPcb2 = 0;

    estimacionPcb1 = __calcular_valor_hrrn(pcb1);
    estimacionPcb2 = __calcular_valor_hrrn(pcb2);

    if (estimacionPcb1 >= estimacionPcb2) {
        return pcb1;
    } else return pcb2;
}

static t_pcb *__elegir_pcb_segun_fifo(t_estado* estado)
{
    return estado_desencolar_primer_pcb_atomic(estado);
}

static t_pcb *__elegir_pcb_segun_hrrn(t_estado* estado)
{   
    t_pcb *pcbSeleccionado;

    set_timespec(tiempoActual);
    pcbSeleccionado = (t_pcb*) list_get_maximum(estado->listaProcesos, (void*) _comparar_pcb_segun_hrrn); // Chequear si esto esta bien

    //Falta eliminarlo de la lista

    return pcbSeleccionado;
}

static void __estimar_proxima_rafaga(t_pcb *pcb, double tiempoEnCpu)
{
    double estimadoProxRafaga = alfaHRRN * tiempoEnCpu + (1 - alfaHRRN) * pcb->estimadoProxRafaga;
    pcb_set_estimado_prox_rafaga(pcb, estimadoProxRafaga);
}

static t_pcb *__elegir_pcb(t_estado* estado)
{
    if (__algoritmo_es_fifo()) {
        return __elegir_pcb_segun_fifo(estado);
    } else 
        return __elegir_pcb_segun_hrrn(estado);
}

static void *__planificador_corto_plazo()
{   
    t_pcb *pcbRecibido = NULL;
    t_header headerPcbRecibido;
    timestamp *inicioEjecucionProceso = NULL;
    timestamp *finEjecucionProceso = NULL;
    double tiempoEnCpu = 0;
    
    //for (;;) {
    t_pcb *pcbAEjecutar = __elegir_pcb(estadoReady);
    __pcb_pasar_de_ready_a_running(pcbAEjecutar);

    // Se manda el pcb a la cpu para que lo ejecute
    set_timespec(inicioEjecucionProceso);
    ejecutar_proceso(pcbAEjecutar); 
    
    recibir_proceso_desajolado(pcbAEjecutar);
    set_timespec(finEjecucionProceso);

    tiempoEnCpu = obtener_diferencial_de_tiempo_en_milisegundos(finEjecucionProceso, inicioEjecucionProceso);
    __estimar_proxima_rafaga(pcbAEjecutar, tiempoEnCpu); // Esta bien ese pcb?

    // Recibe pcb de la cpu
    headerPcbRecibido = recibir_motivo_desalojo(); 

    switch(headerPcbRecibido)
    {
        case HEADER_instruccion_yield:
        {   
            __pcb_pasar_de_running_a_ready(pcbRecibido);
            break;
        }
        case HEADER_instruccion_exit:
        {
            __terminar_proceso(pcbRecibido,FINALIZACION_SUCCESS);
            break;
        }
        case HEADER_segmentation_fault:
        {
            __terminar_proceso(pcbRecibido,FINALIZACION_SEGFAULT);
            break;
        }
        case HEADER_instruccion_io:
        {
            break;
        }
        case HEADER_instruccion_fopen:
        {
            break;
        }
        case HEADER_instruccion_fclose:
        {
            break;
        }
        case HEADER_instruccion_fseek:
        {
            break;
        }
        case HEADER_instruccion_fread:
        {
            break;
        }
        case HEADER_instruccion_fwrite:
        {
            break;
        }
        case HEADER_instruccion_ftruncate:
        {
            break;
        }
        case HEADER_instruccion_wait:
        {
            break;
        }
        case HEADER_instruccion_signal:
        {
            break;
        }
        case HEADER_instruccion_create_segment:
        {
            break;
        }
        case HEADER_instruccion_delete_segment:
        {
            break;
        }
    } 
    //}

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

    
    __pcb_pasar_de_null_a_new(nuevoPcb);
    
    return NULL;
}

void inicializar_estructuras(void) 
{
    __inicializar_estructuras_pid();
    __inicializar_estructuras_estados();
    __inicializar_semaforos();
    __inicializar_alfa_hrrn();
    __crear_hilos_planificadores();
}

