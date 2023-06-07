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
            case HEADER_crear_segmento:
            {
                break;
            }
            /* case HEADER_eliminar_segmento: // Este header hay q agregarlo
            {

            }
            case HEADER_compactar: // Este header hay q agregarlo
            {

            } */
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