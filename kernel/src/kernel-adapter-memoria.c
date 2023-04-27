#include <kernel-adapter-memoria.h>

pthread_mutex_t mutexSocketMemoria;

t_info_segmentos *adapter_memoria_pedir_inicializacion_proceso (t_pcb *pcbAMandar) // REVISAR ESTA FUNCION!!!!
{
    pthread_mutex_lock(&mutexSocketMemoria);
    
    t_info_segmentos *tablaSegmentos = NULL;

    uint32_t pid = pcb_get_pid(pcbAMandar);
    uint32_t tamanioPid = sizeof(pid);
    int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

    t_buffer *bufferPidAProceso = buffer_create();
    buffer_pack(bufferPidAProceso, (void*) &pid, tamanioPid);
    stream_send_buffer(socketMemoria, 1, bufferPidAProceso); // Ojo que hay que agregar este header! HEADER_solicitud_inicializacion_proceso
    buffer_destroy(bufferPidAProceso);

    uint8_t respuestaMemoria = stream_recv_header(socketMemoria);

    if (respuestaMemoria == HANDSHAKE_ok_continue) {
        t_buffer *bufferARecibir = buffer_create();
        stream_recv_buffer(socketMemoria, bufferARecibir);
        uint32_t tamanioTablaSegmentos = sizeof (t_info_segmentos);
        buffer_unpack(bufferARecibir, tablaSegmentos, tamanioTablaSegmentos); // Esto no se si esta bien
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

    return tablaSegmentos;
}

void adapter_memoria_finalizar_proceso (t_pcb *pcbATerminar)
{   
    if (pcb_get_estado_finalizacion(pcbATerminar)) {
        
        pthread_mutex_lock(&mutexSocketMemoria);


        uint32_t pid = pcb_get_pid(pcbATerminar);
        uint32_t tamanioPid = sizeof(pid);
        int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);

        t_buffer *bufferPidATerminar = buffer_create();
        buffer_pack(bufferPidATerminar, (void*) &pid, tamanioPid);
        stream_send_buffer(socketMemoria, HEADER_proceso_terminado, bufferPidATerminar); // Ojo que hay que agregar este header!
        buffer_destroy(bufferPidATerminar);

        uint32_t respuestaMemoria = stream_recv_header(kernel_config_get_socket_memoria(kernelConfig));

        stream_recv_empty_buffer(kernel_config_get_socket_memoria(kernelConfig));
            
        if (respuestaMemoria == HANDSHAKE_ok_continue) {    
            log_info(kernelLogger, "Proceso %d finalizado correctamente en Memoria", pcb_get_pid(pcbATerminar)); // Despues ver bien este log
        } 
        else {   
            log_error(kernelLogger, "Error al finalizar proceso en Memoria");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&mutexSocketMemoria);
    
    } else {
            log_error(kernelLogger, "El proceso con PID <%d> finalizado no pudo ser creado por falta de memoria", pcb_get_pid(pcbATerminar));
    }
}
