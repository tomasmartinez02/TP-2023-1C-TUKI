#include <filesystem-atender-kernel.h>

void verificar_existencia_archivo(char *nombreArchivo) // para abrir archivo
{   
    //Verificar que exista el FCB correspondiente al archivo
    //Si existe devolver OK
    enviar_confirmacion_existencia_archivo();
    log_existe_archivo(nombreArchivo);
    //Si no existe informar que no existe --> no se si hace falta un HEADER_archivo_inexistente o solo con mandar un error basta
}

void crear_archivo(char *nombreArchivo)
{   
    //  crear un archivo FCB correspondiente al nuevo archivo, con tamaño 0 y sin bloques asociados.
    // Siempre será posible crear un archivo y por lo tanto esta operación deberá devolver OK.
    enviar_confirmacion_archivo_creado();
    log_crear_archivo(nombreArchivo);
}

void truncar_archivo(char *nombreArchivo, uint32_t tamanioNuevo)
{   
    // AMPLIAR TAMAÑO
    //Actualizar el tamaño del archivo en el FCB, se le deberán asignar tantos bloques como sea necesario para 
    //poder direccionar el nuevo tamaño.

    // REDUCIR TAMAÑO
    //Asignar el nuevo tamaño del archivo en el FCB y se deberán marcar como libres todos los bloques que ya
    // no sean necesarios para direccionar el tamaño del archivo (descartando desde el final del archivo hacia el principio).
    enviar_confirmacion_tamanio_archivo_modificado();
    log_truncar_archivo(nombreArchivo, tamanioNuevo);
}

void leer_archivo(char *nombreArchivo, uint32_t puntero, uint32_t direccionFisica, uint32_t cantidadBytes)
{   
    // Leer la información correspondiente de los bloques a partir del puntero y el tamaño recibido
    // Enviar información a memoria para ser escrita a partir de la dirección física 
    // Esperar su finalización para poder confirmar el éxito de la operación al Kernel.
    enviar_confirmacion_lectura_finalizada();
    log_lectura_archivo(nombreArchivo, puntero, direccionFisica, cantidadBytes);
    
}

void escribir_archivo(char *nombreArchivo, uint32_t puntero, uint32_t direccionFisica, uint32_t cantidadBytes)
{
    // Solicitar a la Memoria la información que se encuentra a partir de la dirección física y escribirlo en 
    //los bloques correspondientes del archivo a partir del puntero recibido.
    //El tamaño de la información a leer de la memoria y a escribir en los bloques se recibe desde el Kernel (cantidadBytes)

    enviar_confirmacion_escritura_finalizada();
    log_escritura_archivo(nombreArchivo, puntero, direccionFisica, cantidadBytes);
}

void atender_peticiones_kernel()
{
    uint8_t headerPeticionRecibida = recibir_header_peticion_de_kernel();
    // Ejecuto el pedido que hizo kernel
    switch(headerPeticionRecibida)
    {
        case HEADER_solicitud_creacion_archivo:
        {      
            char* nombreArchivo = recibir_buffer_nombre_archivo();
            //crear_archivo(nombreArchivo);
            free(nombreArchivo);
            break;
        }
        case HEADER_solicitud_escribir_archivo:
        {   
            char *nombreArchivo = NULL;
            uint32_t direccionFisica;
            uint32_t cantidadBytes;
            uint32_t puntero;
            recibir_buffer_escritura_archivo(&nombreArchivo, &puntero, &direccionFisica, &cantidadBytes);
            //escribir_archivo(nombreArchivo, puntero, direccionFisica, cantidadBytes);
            free(nombreArchivo);
            break;
        }
        case HEADER_solicitud_leer_archivo:
        {
            char *nombreArchivo = NULL;
            uint32_t direccionFisica;
            uint32_t cantidadBytes;
            uint32_t puntero;
            recibir_buffer_lectura_archivo(&nombreArchivo, &puntero, &direccionFisica, &cantidadBytes);
            //leer_archivo(nombreArchivo, puntero, direccionFisica, cantidadBytes);
            free(nombreArchivo);
            break;
        }
        case HEADER_solicitud_modificar_tamanio_archivo:
        {   
            char *nombreArchivo = NULL;
            uint32_t tamanioNuevo;
            recibir_buffer_truncate_archivo(&nombreArchivo, &tamanioNuevo);
            //truncar_archivo(nombreArchivo, tamanioNuevo);
            free(nombreArchivo);
            break;
        }
        case HEADER_consulta_existencia_archivo:
        {   
            char* nombreArchivo = recibir_buffer_nombre_archivo();
            //verificar_existencia_archivo(nombreArchivo);
            free(nombreArchivo);
            break;
        }
    }
}