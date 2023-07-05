#include <memoria-atencion-kernel.h>

// Funciones privadas

// Funciones publicas

void *atender_peticiones_kernel()
{

    for (;;) {
        int socketKernel = memoria_config_get_socket_kernel(memoriaConfig);
        pthread_mutex_lock(&mutexSocketKernel);
        t_header headerRecibido = stream_recv_header(socketKernel);
        t_buffer* bufferRecibido = buffer_create();
        
        switch (headerRecibido) {
            case HEADER_solicitud_inicializacion_proceso:
            {
                stream_recv_buffer(socketKernel,bufferRecibido);
                uint32_t pid = adapter_kernel_recibir_pid(bufferRecibido);
                t_info_segmentos** tablaCreada = crear_tabla_nuevo_proceso(pid);
                adapter_kernel_enviar_tabla(tablaCreada, HEADER_tabla_segmentos);
                log_info(memoriaLogger,  "Creacion de Proceso PID: <%d>", pid);
                break;
            }
            case HEADER_crear_segmento:
            {   
                log_info(memoriaLogger,  "Entra a crear segmento");
                t_info_segmentos *segmento = adapter_kernel_recibir_segmento_a_crear(socketKernel, bufferRecibido);
                log_info(memoriaLogger,  "Recibe segmento a crear");
                uint32_t pid = adapter_kernel_recibir_pid(bufferRecibido);
                log_info(memoriaLogger,  "Recibe pid");
                
                if (verificar_memoria_suficiente(segmento->tamanio)){
                    log_info(memoriaLogger,  "Entra a verificar memoria suficiente");
                    if (verificar_memoria_contigua(segmento->tamanio)) {
                        log_info(memoriaLogger,  "Tiene memoria contigua");
                        uint32_t baseSegmento = crear_segmento(segmento, pid);
                        adapter_kernel_enviar_direccion_base(socketKernel, baseSegmento);
                        log_info(memoriaLogger,  "PID: <%d> - Crear Segmento: <%d> - Base: <%d> - TAMAÑO: <%d>", pid, segmento->idSegmento, baseSegmento, segmento->tamanio); 
                        // No hay que hacer free del puntero?
                    } else {
                        adapter_kernel_solicitar_compactacion(socketKernel);
                        log_info(memoriaLogger,  "Se solicita la compactacion");
                        free(segmento);
                    }
                } else {
                    log_error(memoriaLogger,  "PID: <%d> - Crear Segmento: <%d> - ERROR: No hay memoria suficiente", pid, segmento->idSegmento);
                    adapter_kernel_error_out_of_memory(socketKernel);
                    free(segmento);
                }
                break;
            }
            case HEADER_eliminar_segmento: 
            {
                uint32_t idSegmento = adapter_kernel_recibir_id_segmento_a_eliminar(socketKernel, bufferRecibido);
                uint32_t pid = adapter_kernel_recibir_pid(bufferRecibido);

                eliminar_segmento(idSegmento, pid); // El log esta adentro
                adapter_kernel_enviar_eliminacion_segmento(socketKernel, pid);
                break;
            }
            case HEADER_compactar:
            {
                compactar_memoria();
                adapter_kernel_confirmar_compactacion_memoria(socketKernel);
                break;
            }
            case HEADER_solicitud_finalizacion_proceso:
            {
                stream_recv_buffer(socketKernel,bufferRecibido);
                uint32_t pid = adapter_kernel_recibir_pid(bufferRecibido);
                eliminar_estructuras_proceso(pid);
                log_info(memoriaLogger,  "Eliminación de Proceso PID: <%d>", pid);
                adapter_kernel_confirmar_finalizacion_proceso(socketKernel, pid);
                break;
            }
            default:
            {   
                log_error(memoriaLogger, "Se recibio un header de kernel NO valido");
                break;
            }
        }
        pthread_mutex_unlock(&mutexSocketKernel);
        buffer_destroy(bufferRecibido);
    }

    return NULL;
}
