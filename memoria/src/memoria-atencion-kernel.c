#include <memoria-atencion-kernel.h>

// Funciones privadas

// Funciones publicas

void atender_peticiones_kernel(void)
{

    for (;;) {
        int socketKernel = memoria_config_get_socket_kernel(memoriaConfig);
        t_header headerRecibido = stream_recv_header(socketKernel);
        t_buffer* bufferRecibido = buffer_create();
        
        switch (headerRecibido) {
            case HEADER_solicitud_inicializacion_proceso:
            {
                uint32_t pid = adapter_kernel_recibir_pid(socketKernel, bufferRecibido);
                t_info_segmentos** tablaCreada = crear_tabla_nuevo_proceso(pid);
                adapter_kernel_enviar_tabla(tablaCreada, HEADER_tabla_segmentos);
                log_info(memoriaLogger,  "Creacion de Proceso PID: <%d>", pid);
                break;
            }
            case HEADER_crear_segmento:
            {   
                t_info_segmentos *segmento = adapter_kernel_recibir_segmento_a_crear(socketKernel, bufferRecibido);
                uint32_t pid = adapter_kernel_recibir_pid(socketKernel, bufferRecibido);
                
                if (verificar_memoria_suficiente(segmento->tamanio)){
                    if (verificar_memoria_contigua(segmento->tamanio)) {
                        uint32_t baseSegmento = crear_segmento(segmento, pid);
                        adapter_kernel_enviar_direccion_base(socketKernel, baseSegmento); 
                    } else {
                        adapter_kernel_solicitar_compactacion(socketKernel);
                        free(segmento);
                    }
                } else {
                    adapter_kernel_error_out_of_memory(socketKernel);
                    free(segmento);
                }
                break;
            }
            case HEADER_eliminar_segmento: // Este header hay q agregarlo
            {
                uint32_t idSegmento = adapter_kernel_recibir_id_segmento_a_eliminar(socketKernel, bufferRecibido);
                uint32_t pid = adapter_kernel_recibir_pid(socketKernel, bufferRecibido);

                eliminar_segmento(idSegmento, pid);
                adapter_kernel_enviar_eliminacion_segmento(socketKernel, pid);
            }
            case HEADER_compactar: // Este header hay q agregarlo
            {

            } 
            // tambien deberíamos agregar para cuando se finaliza un proceso
            default:
            {   
                // ERROR
                break;
            }
        }
        buffer_destroy(bufferRecibido);
    }

    return;
}