#include <filesystem-atender-kernel.h>

void ampliar_archivo(t_fcb *fcbArchivo, uint32_t tamanioNuevo)
{
    uint32_t cantidadBloquesAsignadosActual = fcb_get_cantidad_bloques_asignados(fcbArchivo);
    uint32_t tamanioNuevoEnBloques = redondear_hacia_arriba(tamanioNuevo, tamanioBloques);

    if(cantidadBloquesAsignadosActual == 0) {
        log_info(filesystemLogger, "El archivo no tiene ningun bloque asignado actualmente.");
        asignar_bloques_archivo_vacio(fcbArchivo,tamanioNuevo);
    }
    else {
        /* Si ya tiene bloques, solo se agregan punteros al bloque de punteros */
        log_info(filesystemLogger, "El archivo ya tenía algún bloque asignado previamente.");
        asignar_bloques_archivo_no_vacio(fcbArchivo, tamanioNuevo);
    }
    fcb_set_tamanio_archivo(fcbArchivo, tamanioNuevo);
    fcb_set_cantidad_bloques_asignados(fcbArchivo, tamanioNuevoEnBloques);
}

void reducir_archivo(t_fcb *fcbArchivo, uint32_t tamanioNuevo)
{
    uint32_t cantidadBloquesDesasignar, cantidadBloquesAsignadosActual, tamanioNuevoEnBloques;
    if (tamanioNuevo == 0)
    {
        vaciar_archivo(fcbArchivo);
        return;
    }
    cantidadBloquesAsignadosActual = fcb_get_cantidad_bloques_asignados(fcbArchivo);
    tamanioNuevoEnBloques = redondear_hacia_arriba(tamanioNuevo, tamanioBloques);
    cantidadBloquesDesasignar = cantidadBloquesAsignadosActual - tamanioNuevoEnBloques;
    desasignar_bloques(fcbArchivo, cantidadBloquesDesasignar);
    return;
}

//Verificar que exista el FCB correspondiente al archivo
void verificar_existencia_archivo(char *nombreArchivo) // para abrir archivo
{   
    log_abrir_archivo(nombreArchivo);
    //Si existe devolver OK
    if (dictionary_has_key(listaFcbs, nombreArchivo))
    {
        enviar_confirmacion_existencia_archivo();
        log_existe_archivo(nombreArchivo);
    }
    // Si no existe
    else
    {
        enviar_confirmacion_no_existencia_archivo();
        log_no_existe_archivo(nombreArchivo);
    }
    return;
}

t_fcb *crear_archivo(char *nombreArchivo)
{   
    bool archivoCreado;
    // Crear un archivo FCB correspondiente al nuevo archivo, con tamaño 0 y sin bloques asociados.
    // Siempre será posible crear un archivo y por lo tanto esta operación deberá devolver OK.
    t_fcb* nuevoFcb = crear_nuevo_fcb(nombreArchivo);
    archivoCreado = crear_archivo_nuevo_fcb(nuevoFcb);
    if (archivoCreado)
    {   
        enviar_confirmacion_archivo_creado();
        log_crear_archivo(nombreArchivo);
    }
    return nuevoFcb;
}

// FTRUNCATE

void truncar_archivo(char *nombreArchivo, uint32_t tamanioNuevo)
{   
    uint32_t bloquesAsignados, bloquesNuevos;
    // Busco el fcb relacionado al archivo que quiero truncar
    t_fcb *fcbArchivo = dictionary_get(listaFcbs, nombreArchivo);
    if (fcbArchivo == NULL)
    {
        log_error(filesystemLogger, "No se encontró el fcb en la lista de fcbs.");
        log_error(filesystemDebuggingLogger, "No se encontró el fcb en la lista de fcbs.");
        return;
    }

    bloquesAsignados = fcb_get_cantidad_bloques_asignados(fcbArchivo);
    bloquesNuevos = redondear_hacia_arriba(tamanioNuevo, tamanioBloques);
    log_info(filesystemLogger, "Bloques asignados actuales: %u", bloquesAsignados);

    // AMPLIAR TAMAÑO
    if (bloquesAsignados < bloquesNuevos)
    {   
        log_info(filesystemLogger, "El archivo %s se va a ampliar.", nombreArchivo);
        ampliar_archivo(fcbArchivo, tamanioNuevo);
    }
    // REDUCIR TAMAÑO
    if (bloquesAsignados > bloquesNuevos)
    {   
        log_info(filesystemLogger, "El archivo %s se va a reducir.", nombreArchivo);
        reducir_archivo(fcbArchivo, tamanioNuevo);
    }
    // SI TAMANIO ACTUAL == TAMANIO NUEVO --> NO SE HACE NADA 
    fcb_set_tamanio_archivo(fcbArchivo, tamanioNuevo);
    persistir_fcb(fcbArchivo);

    log_truncar_archivo(nombreArchivo, tamanioNuevo);
    fcb_mostrar_por_pantalla(fcbArchivo);
    return;
}

// FREAD

// Leer la información correspondiente de los bloques a partir del puntero y el tamaño recibido
void leer_archivo(char *nombreArchivo, uint32_t punteroProceso, uint32_t direccionFisica, uint32_t cantidadBytes)
{   
    uint32_t posicionAbsoluta;
    bool respuestaMemoria;

    // Busco el fcb relacionado al archivo que quiero truncar
    t_fcb *fcbArchivo = dictionary_get(listaFcbs, nombreArchivo);
    if (fcbArchivo == NULL)
    {
        log_error(filesystemLogger, "No se encontró el fcb en la lista de fcbs.");
        log_error(filesystemDebuggingLogger, "No se encontró el fcb en la lista de fcbs.");
        return;
    }

    // Obtengo la posicion desde la cual voy a empezar a leer informacion.
    posicionAbsoluta = obtener_posicion_absoluta(fcbArchivo, punteroProceso);

    char* informacion = "aca voy a tener la info q voy a leer";

    // Enviar información a memoria para ser escrita a partir de la dirección física 
    solicitar_escritura_memoria(direccionFisica, cantidadBytes, informacion);
    
    // Esperar su finalización para poder confirmar el éxito de la operación al Kernel.
    respuestaMemoria = recibir_buffer_confirmacion_escritura_memoria();
    if (respuestaMemoria)
    {
        enviar_confirmacion_fread_finalizado();
    }
    log_lectura_archivo(nombreArchivo, punteroProceso, direccionFisica, cantidadBytes);
    
}

// FWRITE

void escribir_archivo(char *nombreArchivo, uint32_t punteroProceso, uint32_t direccionFisica, uint32_t cantidadBytesAEscribir)
{
    uint32_t bloqueActual, espacioDisponible, bytesAEscribirEnBloque, bytesPorEscribir;
    uint32_t bytesEscritos = 0;

    // Busco el fcb relacionado al archivo en el que se quiere escribir
    t_fcb *fcbArchivo = dictionary_get(listaFcbs, nombreArchivo);
    if (fcbArchivo == NULL)
    {
        log_error(filesystemLogger, "No se encontró el fcb en la lista de fcbs.");
        log_error(filesystemDebuggingLogger, "No se encontró el fcb en la lista de fcbs.");
        return;
    }

    // Solicitar a la Memoria la información que se encuentra a partir de la dirección física
    solicitar_informacion_memoria(direccionFisica, cantidadBytesAEscribir);

    char *informacionAEscribir = recibir_buffer_informacion_memoria(cantidadBytesAEscribir);

    // Escribir la información en los bloques correspondientes del archivo a partir del puntero recibido:
   
    // obtener bloqueActual;
    //espacioDisponible = espacio_disponible_en_bloque(bloqueActual);

    // Si se tienen que escribir menos bytes de los que hay disponibles con escribir solo en este bloque alcanza
    /*
    if (cantidadBytesAEscribir <= espacioDisponible)
    {
        escribir_en_bloque(bloqueActual,cantidadBytesAEscribir);
        return;
    }*/

    /* Si se tienen que escribir más bytes de los que hay disponibles hay que escribir una parte en este bloque
    y el resto en el/los bloques siguientes */
    /*
    if (cantidadBytesAEscribir > espacioDisponible)
    {
        while (bytesEscritos < cantidadBytesAEscribir) {

            if (espacioDisponible == 0)
            {
                //bloqueActual = buscar_siguiente_bloque(bloqueActual);
                //espacioDisponible = espacio_disponible_en_bloque(bloqueActual);
            }

            // cuántos bytes voy a poder escribir en el bloque actual
            bytesAEscribirEnBloque = cantidadBytesAEscribir - espacioDisponible;

            //escribir_en_bloque(bloqueActual,bytesAEscribirEnBloque);

            // cuántos bytes van a faltar escribir
            bytesPorEscribir = cantidadBytesAEscribir - bytesAEscribirEnBloque;

            bytesEscritos += bytesAEscribirEnBloque;
        }
    }
    */

    log_escritura_archivo(nombreArchivo, punteroProceso, direccionFisica, cantidadBytesAEscribir);
    enviar_confirmacion_fwrite_finalizado();
}

void atender_peticiones_kernel()
{   
    // Loop infinito para que este atento a los pedidos de kernel
    for (;;)
    {    
        uint8_t headerPeticionRecibida = recibir_header_peticion_de_kernel();
        
        // Ejecuto el pedido que hizo kernel
        switch(headerPeticionRecibida)
        {
            case HEADER_solicitud_creacion_archivo:
            {     
                char* nombreArchivo = recibir_buffer_nombre_archivo();
                // PARA PROBAR //
                log_info(filesystemLogger, "FS recibe la solicitud de crear el archivo %s", nombreArchivo);
                crear_archivo(nombreArchivo);
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
                escribir_archivo(nombreArchivo, puntero, direccionFisica, cantidadBytes);

                // PARA PROBAR //
                log_info(filesystemLogger, "FS recibe la solicitud de escribir archivo %s, %d cantidad de bytes, en el puntero %d, direccion fisica%d", nombreArchivo, cantidadBytes, puntero, direccionFisica);
                
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
                leer_archivo(nombreArchivo, puntero, direccionFisica, cantidadBytes);

                // PARA PROBAR //
                log_info(filesystemLogger, "FS recibe la solicitud de leer archivo %s, %d cantidad de bytes, en el puntero %d, direccion fisica%d", nombreArchivo, cantidadBytes, puntero, direccionFisica);
                free(nombreArchivo);
                break;
            }
            case HEADER_solicitud_modificar_tamanio_archivo:
            {   
                char *nombreArchivo = NULL;
                uint32_t tamanioNuevo;
                recibir_buffer_truncate_archivo(&nombreArchivo, &tamanioNuevo);
                truncar_archivo(nombreArchivo, tamanioNuevo);
                // PARA PROBAR //
                log_info(filesystemLogger, "FS recibe la solicitud de cambiarle el tamanio (%d) al archivo %s", tamanioNuevo, nombreArchivo );
                enviar_confirmacion_tamanio_archivo_modificado();

                free(nombreArchivo);
                break;
            }
            case HEADER_consulta_existencia_archivo:
            {   
                char* nombreArchivo = recibir_buffer_nombre_archivo();
                // PARA PROBAR //
                log_info(filesystemLogger, "FS recibe la consulta de existencia del archivo %s", nombreArchivo);
                verificar_existencia_archivo(nombreArchivo);

                free(nombreArchivo);
                break;
            }
        }
    }
    return;
}