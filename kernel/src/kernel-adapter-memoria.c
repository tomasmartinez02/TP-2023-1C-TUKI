#include <kernel-adapter-memoria.h>

pthread_mutex_t mutexSocketMemoria;

t_info_segmentos *memoria_adapter_pedir_inicializacion_proceso(t_pcb *pcbAMandar) // REVISAR ESTA FUNCION!!!!
{
    pthread_mutex_lock(&mutexSocketMemoria);
    
    uint32_t pid = pcb_get_pid(pcbAMandar);
    uint32_t tamanioPid = sizeof(pid);
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    t_buffer *bufferPidAProceso = buffer_create();
    buffer_pack(bufferPidAProceso, (void*) &pid, tamanioPid);
    stream_send_buffer(socketMemoria, HEADER_solicitud_inicializacion_proceso, bufferPidAProceso); // Ojo que hay que agregar este header!
    buffer_destroy(bufferPidAProceso);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    if (respuestaMemoria == HANDSHAKE_ok_continue) {
        t_buffer *bufferARecibir = buffer_create();
        stream_recv_buffer(socketMemoria, bufferARecibir);
        uint32_t tamanioTablaSegmentos = sizeof (t_info_segmentos);
        buffer_unpack(bufferARecibir, pcbAMandar->t_info_segmentos, tamanioTablaSegmentos); // Esto no se si esta bien
        buffer_destroy(bufferARecibir);
        log_info(kernelLogger, "Proceso: %d - Se recibe el array con las tablas de paginas de los segmentos", pid);
    }
    else if (respuestaMemoria == HEADER_error) { // En que caso nos mandaria eso?
        stream_recv_empty_buffer(socketMemoria);
        pthread_mutex_unlock(&mutexSocketMemoria);
        return NULL;
    }
    else {
        log_error(kernelLogger, "Error al recibir la tabla de segmentos");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_unlock(&mutexSocketMemoria);

}

static t_buffer __serializar_pcb_para_memoria (t_pcb *pcbAMandar) 
{
    t_buffer *bufferAEnviar = buffer_create();
    uint32_t pid = pcb_get_pid(pcbAMandar);
    uint32_t programCounter = pcb_get_program_counter(pcbAMandar);
    double estimadoProxRafaga = pcb_get_estimado_prox_rafaga(pcbAMandar);

    

    // Ver como mandar las instrucciones
    // Ver como mandar los registros de cpu
    // Ver como mandar la tablaSegmentos
    // Ver como mandar los archivos abiertos

    pcb->pid = pid;
    pcb->instrucciones = NULL;
    pcb->programCounter = 0;
    pcb->registrosCpu = NULL;
    pcb->estimadoProxRafaga = kernel_config_get_estimacion_inicial(kernelConfig); // el valor inicial se saca del config y después se calcula
    pcb->tablaSegmentos = NULL;
    pcb->archivosAbiertos = NULL;
    pcb->estadoActual = NEW;
    pcb->estadoDeFinalizacion = NEW;
    pcb->estadoAnterior = NEW;
    pcb->procesoBloqueadoOTerminado = false;
    pcb->socketProceso = -1;

}