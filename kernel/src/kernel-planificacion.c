#include <kernel-planificacion.h>

// Variables estaticas
static uint32_t pidActual;
static pthread_mutex_t mutexPid;

// Estados (estan aca porque en el adapter de memoria)
t_estado *estadoNew;
t_estado *estadoReady;
t_estado *estadoExecute;
t_estado *estadoBlocked; 
t_estado *estadoExit;

// Semaforos planificacion
static sem_t gradoMultiprogramacion;
sem_t dispatchPermitido;
pthread_mutex_t mutexSocketMemoria;
pthread_mutex_t mutexSocketFilesystem;
sem_t semFRead;
sem_t semFWrite;
bool fRead;
bool fWrite;

// Funciones privadas

// Funciones inicializacion estructuras kernel

// Inicializa pidActual y mutex del pidActual
static void __inicializar_estructuras_pid(void)
{
    pidActual = 0;
    pthread_mutex_init(&mutexPid, NULL);
    
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
    sem_init(&dispatchPermitido, 0, 1);
    pthread_mutex_init(&mutexSocketMemoria, NULL);
    pthread_mutex_init(&mutexSocketFilesystem, NULL);
    sem_init(&semFRead, 0, 1);
    sem_init(&semFWrite, 0, 1);
    fRead = false;
    fWrite = false;

    return;
}

// Funciones utilitarias para la planificacion

// Obtiene y actualiza el pidActual para asignarlo a un pcb de forma atomica
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

// Funcion para que siga ejecutando el mismo pcb
void seguir_ejecutando(t_pcb* pcbEnEjecucion)
{
    estado_encolar_pcb_atomic(estadoExecute, pcbEnEjecucion);
    return ;
}

// TRANSICIONES DE ESTADOS

// Encola, actualiza pcb y loggea transicion de estados
static void __pcb_pasar_de_estado(t_pcb* pcb, t_estado *nuevoEstado, char *stringEstadoViejo, char *stringEstadoNuevo)
{   
    t_nombre_estado nombreNuevoEstado = nuevoEstado->nombreEstado;
    
    pcb_set_estado_anterior(pcb, pcb_get_estado_actual(pcb));
    pcb_set_estado_actual(pcb, nombreNuevoEstado);

    log_transicion_estados(stringEstadoViejo, stringEstadoNuevo, pcb_get_pid(pcb));
    estado_encolar_pcb_atomic(nuevoEstado, pcb);

    return;
}

// Funcion generica para pasar un pcb a ready desde cualquier estado
static void __pcb_pasar_a_ready(t_pcb* pcbAReady, char *stringEstadoViejo)
{
    __pcb_pasar_de_estado(pcbAReady, estadoReady, stringEstadoViejo, ESTADO_READY);
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
    log_creacion_nuevo_proceso(pcbANew);
    __pcb_pasar_de_estado(pcbANew, estadoNew, ESTADO_NULL, ESTADO_NEW);

    return;
}

// Funcion generica para pasar un pcb a Exit desde cualquier estado
void __pcb_pasar_a_exit(t_pcb* pcbAExit, char *stringEstadoViejo) // Habria que borrar estructuras de memoria // hicimos que deje de ser ser estatica para poder usarla en el adapter de memori
{
    pcb_set_estado_finalizacion(pcbAExit, pcb_get_estado_actual(pcbAExit));
    pcb_set_proceso_bloqueado_o_terminado(pcbAExit, true);
    // log_finalizacion_proceso(pcbAExit, stringEstadoViejo);
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
void terminar_proceso(t_pcb* pcbFinalizado, char *motivoFinalizacion)
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
    log_finalizacion_proceso(pcbFinalizado, motivoFinalizacion);
}

void pcb_pasar_de_running_a_exit_public(t_pcb* pcbAExit)
{   
    __pcb_pasar_de_running_a_exit(pcbAExit);
    terminar_proceso(pcbAExit, FINALIZACION_RECURSOS);
    return;
}

void pcb_pasar_de_blocked_a_ready_public(t_pcb* pcbAReady)
{   
    __pcb_pasar_de_blocked_a_ready(pcbAReady);
    return;
}

void pcb_pasar_de_running_a_blocked_public(t_pcb* pcbABlocked)
{   
    __pcb_pasar_de_running_a_blocked(pcbABlocked);
    return;
}

// Planificadores
// Planificador de largo plazo

// Libera los pcbs que se encuentran en el estado EXIT
static void* __liberar_pcbs_en_exit(void* args)
{
    for (;;) {
        t_pcb *pcbALiberar = estado_desencolar_primer_pcb_atomic(estadoExit);
        
        adapter_memoria_finalizar_proceso(pcbALiberar);
        stream_send_empty_buffer(pcb_get_socket(pcbALiberar), HEADER_proceso_terminado);
        
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

        // Pido a la memoria que inicialice al pcb y me devuelva la tabla de segmentos
        t_info_segmentos **tablaSegmentos = adapter_memoria_pedir_inicializacion_proceso(pcbAReady);
        pcb_set_tabla_segmentos(pcbAReady, tablaSegmentos);

        if (tablaSegmentos == NULL) {
            terminar_proceso(pcbAReady, FINALIZACION_OUTOFMEMORY);
        }
        else {
           __pcb_pasar_de_new_a_ready(pcbAReady);
        } 
    }

    return NULL;
}

// Planificador de corto plazo

// Elige un pcb para dispatch segun fifo
static t_pcb *__elegir_pcb_segun_fifo(t_estado* estado)
{
    return estado_desencolar_primer_pcb_atomic(estado);
}

// Elige un pcb para dispatch segun hrrn
static t_pcb *__elegir_pcb_segun_hrrn(t_estado* estado)
{   
    t_pcb *pcbSeleccionado = estado_remover_pcb_segun_maximo_hrrn_atomic(estado);
    
    return pcbSeleccionado;
}

// Chequea si el algoritmo del kernel es Fifo
static bool __algoritmo_es_fifo()
{
    return strcmp(kernel_config_get_algoritmo_planificacion(kernelConfig), "FIFO") == 0;
}

// Elige el pcb segun el algoritmo que tengas
static t_pcb *__elegir_pcb(t_estado* estado)
{
    if (__algoritmo_es_fifo()) {
        return __elegir_pcb_segun_fifo(estado);
    } 
    else {
        return __elegir_pcb_segun_hrrn(estado);
    }
}

// Pone en ejecucion al pcb y recibe el pcb desalojado por la cpu
static void *__ejecucion_desalojo_pcb(void *args)
{
    for (;;) {
        t_pcb *pcbEnEjecucion = estado_desencolar_primer_pcb_atomic(estadoExecute);

        timestamp inicioEjecucionProceso;
        timestamp finEjecucionProceso;
        
        // Se manda el pcb a la cpu para que lo ejecute y se espera la respuesta para actualizar el pcb
        set_timespec(&inicioEjecucionProceso);
        ejecutar_proceso(pcbEnEjecucion); 
        recibir_proceso_desajolado(pcbEnEjecucion);
        set_timespec(&finEjecucionProceso);

        // Actualizo el estimado en el pcb segun el real ejecutado
        double tiempoRealEjecutadoEnCpu = obtener_diferencial_de_tiempo_en_milisegundos(&finEjecucionProceso, &inicioEjecucionProceso);
        pcb_estimar_proxima_rafaga(pcbEnEjecucion, tiempoRealEjecutadoEnCpu);

        // Recibe motivo de desalojo del proceso
        uint8_t headerPcbRecibido = recibir_motivo_desalojo(); 

        // Ejecuto la instruccion que ha producido el desalojo
        switch(headerPcbRecibido)
        {
            case HEADER_instruccion_yield:
            {   
                recibir_buffer_vacio_instruccion();
                __pcb_pasar_de_running_a_ready(pcbEnEjecucion);
                sem_post(&dispatchPermitido);
                break;
            }
            case HEADER_instruccion_exit:
            {
                recibir_buffer_vacio_instruccion();
                terminar_proceso(pcbEnEjecucion, FINALIZACION_SUCCESS);
                sem_post(&dispatchPermitido);
                break;
            }
            case HEADER_segmentation_fault:
            {
                recibir_buffer_vacio_instruccion();
                terminar_proceso(pcbEnEjecucion,FINALIZACION_SEGFAULT);
                sem_post(&dispatchPermitido);
                break;
            }
            case HEADER_instruccion_io:
            {
                uint32_t tiempoEjecucion = recibir_buffer_instruccion_io();
                ejecutar_instruccion_io(pcbEnEjecucion, tiempoEjecucion);
                sem_post(&dispatchPermitido);
                break;
            }
            case HEADER_instruccion_fopen:
            {   
                char *nombreArchivo = recibir_buffer_instruccion_fopen();
                ejecutar_instruccion_fopen(pcbEnEjecucion, nombreArchivo);
                free(nombreArchivo);
                break;
            }
            case HEADER_instruccion_fclose:
            {
                char *nombreArchivo = recibir_buffer_instruccion_fclose();
                ejecutar_instruccion_fclose(pcbEnEjecucion, nombreArchivo);
                free(nombreArchivo);
                break;
            }
            case HEADER_instruccion_fseek:
            {
                char *nombreArchivo = NULL;
                uint32_t ubicacionNueva;
                recibir_buffer_instruccion_fseek(&nombreArchivo, &ubicacionNueva);
                ejecutar_instruccion_fseek(pcbEnEjecucion, nombreArchivo, ubicacionNueva);
                free(nombreArchivo);
                break;
            }
            case HEADER_instruccion_fread:
            {
                sem_wait(&semFRead);
                fRead = true;
                char *nombreArchivo = NULL;
                uint32_t direccionLogica;
                uint32_t cantidadBytes;
                recibir_buffer_instruccion_fread(&nombreArchivo, &direccionLogica, &cantidadBytes);
                ejecutar_instruccion_fread(pcbEnEjecucion, nombreArchivo, direccionLogica, cantidadBytes);
                //free(nombreArchivo);
                sem_post(&dispatchPermitido); // esto se podria pasar a la funcion d pasar de running a otro estado
                break;
            }
            case HEADER_instruccion_fwrite:
            {
                sem_wait(&semFWrite);
                fWrite = true;
                char *nombreArchivo = NULL;
                uint32_t direccionLogica;
                uint32_t cantidadBytes;
                recibir_buffer_instruccion_fwrite(&nombreArchivo, &direccionLogica, &cantidadBytes);
                ejecutar_instruccion_fwrite(pcbEnEjecucion, nombreArchivo, direccionLogica, cantidadBytes);
                //free(nombreArchivo);
                sem_post(&dispatchPermitido);
                break;
            }
            case HEADER_instruccion_ftruncate:
            {
                char *nombreArchivo = NULL;
                uint32_t tamanio;
                recibir_buffer_instruccion_ftruncate(&nombreArchivo, &tamanio);
                ejecutar_instruccion_ftruncate(pcbEnEjecucion, nombreArchivo, tamanio);
                //free(nombreArchivo);
                sem_post(&dispatchPermitido);
                break;
            }
            case HEADER_instruccion_wait:
            {   
                char* nombreRecurso = recibir_buffer_instruccion_con_recurso();
                ejecutar_instruccion_wait(pcbEnEjecucion, nombreRecurso);
                free(nombreRecurso);
                break;
            }
            case HEADER_instruccion_signal:
            {
                char* nombreRecurso = recibir_buffer_instruccion_con_recurso();
                ejecutar_instruccion_signal(pcbEnEjecucion, nombreRecurso);
                free(nombreRecurso);
                break;
            }
            case HEADER_instruccion_create_segment:
            {   
                uint32_t idSegmento;
                uint32_t tamanio; 
                recibir_buffer_instruccion_create_segment(&idSegmento, &tamanio);
                adapter_memoria_pedir_creacion_segmento(idSegmento, tamanio, pcbEnEjecucion);
                break;
            }
            case HEADER_instruccion_delete_segment:
            {
                uint32_t idSegmento;
                idSegmento = recibir_buffer_instruccion_delete_segment();
                adapter_memoria_pedir_eliminar_segmento(idSegmento, pcbEnEjecucion);
                log_info(kernelLogger, "PID: <%d> - Eliminar Segmento - Id Segmento: <%d>",pcbEnEjecucion->pid, idSegmento);
                break;
            }
        } 

        // sem_post(&dispatchPermitido);
    }

    return NULL;
}

// Dispatcher de pcbs a execute
static void *__planificador_corto_plazo(void *args)
{   
    // Desalojo de PCBs
    pthread_t ejecucionDesalojoTh;
    pthread_create(&ejecucionDesalojoTh, NULL, __ejecucion_desalojo_pcb, NULL);
    pthread_detach(ejecucionDesalojoTh);

    // Dispatcher
    for (;;) {

        sem_wait(&dispatchPermitido);

        t_pcb *pcbAEjecutar = __elegir_pcb(estadoReady);
        __pcb_pasar_de_ready_a_running(pcbAEjecutar);
    }

    return NULL;
}

// Crea los hilos de planificacion principales
static void __crear_hilos_planificadores(void)
{
    // Planificador largo plazo
    pthread_t planificadorLargoPlazoTh;
    pthread_create(&planificadorLargoPlazoTh, NULL, __planificador_largo_plazo, NULL);
    pthread_detach(planificadorLargoPlazoTh);

    // Planificador corto plazo
    pthread_t planificadorCortoPlazoTh;
    pthread_create(&planificadorCortoPlazoTh, NULL, __planificador_corto_plazo, NULL);
    pthread_detach(planificadorCortoPlazoTh);

    return;
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
    stream_send_empty_buffer(socketConsola, HANDSHAKE_ok_continue);

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
    stream_recv_buffer(socketConsola, bufferInstrucciones);

    // Se crea el nuevo pcb
    t_pcb* nuevoPcb = __crear_nuevo_pcb(socketConsola, bufferInstrucciones);
    buffer_destroy(bufferInstrucciones);

    // Envio el pid a la consola
    uint32_t nuevoPid = pcb_get_pid(nuevoPcb);
    t_buffer* bufferPID = buffer_create();
    buffer_pack(bufferPID, &nuevoPid, sizeof(nuevoPid));
    stream_send_buffer(socketConsola, HEADER_pid, bufferPID);
    buffer_destroy(bufferPID);
    
    __pcb_pasar_de_null_a_new(nuevoPcb);
    
    return NULL;
}

void inicializar_estructuras(void) 
{
    __inicializar_estructuras_pid();
    __inicializar_estructuras_estados();
    __inicializar_semaforos();
    __crear_hilos_planificadores();
}


