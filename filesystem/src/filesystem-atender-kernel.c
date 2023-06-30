#include <filesystem-atender-kernel.h>

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
    uint32_t posicionAbsoluta, espacioDisponible, puntero, bytesLeidos, bytesQueFaltanPorLeer;
    bool respuestaMemoria;

    // Busco el fcb relacionado al archivo que quiero truncar
    t_fcb *fcbArchivo = dictionary_get(listaFcbs, nombreArchivo);
    if (fcbArchivo == NULL)
    {
        log_error(filesystemLogger, "No se encontró el fcb en la lista de fcbs.");
        log_error(filesystemDebuggingLogger, "No se encontró el fcb en la lista de fcbs.");
        return;
    }

    //cantidadBloquesALeer = redondear_hacia_arriba(cantidadBytes, tamanioBloques);
    puntero = punteroProceso;
    //bytesLeidos = 0;
    bytesQueFaltanPorLeer = cantidadBytes;
    char *informacion = malloc(cantidadBytes);
    char *buffer = malloc(tamanioBloques);

    // Obtengo la posicion desde la cual voy a empezar a leer informacion.
    posicionAbsoluta = obtener_posicion_absoluta(fcbArchivo, punteroProceso);

    espacioDisponible = espacio_disponible_en_bloque_desde_posicion(punteroProceso);

    archivoDeBloques = abrir_archivo_de_bloques();
    fseek(archivoDeBloques, posicionAbsoluta, SEEK_SET);

    // Si la cantidad de bytes a leer es menor que el espacio que queda en el 
    // bloque seleccionado, solamente se accede a ese bloque.
    if (cantidadBytes < espacioDisponible)
    {
        fread(buffer, sizeof(char), cantidadBytes, archivoDeBloques);
        bytesLeidos = cantidadBytes;
        // Ya se leyeron todos los bytes que habia que leer, no hay que hacer nada mas.
        // Pasar la data leida del buffer al char *informacion.
        memcpy(informacion, buffer, bytesLeidos);
        free(buffer);
    }

    // Si la cantidad de bytes a leer es mayor que el espacio que queda en el bloque
    // seleccionado, se leen todos los bytes que queden en el bloque antes de pasar al siguiente.
    else
    {
        fread(buffer, sizeof(char), espacioDisponible, archivoDeBloques);
        // Se mueve el puntero del archivo hasta el ultimo lugar que se leyo.
        puntero += espacioDisponible; 
        // Contador de bytes leidos.
        bytesLeidos = espacioDisponible;
        // Contador de bytes que faltan por leer.
        bytesQueFaltanPorLeer -= bytesLeidos;
        // Pasar la data leida del buffer al char *informacion.
        memcpy(informacion, buffer, bytesLeidos);
        free(buffer);
    }

    // Repetir hasta que se hayan leido todos los bytes.
    while(bytesQueFaltanPorLeer != 0)
    {   
        buffer = malloc(tamanioBloques);
        // Se busca la posicion del siguiente bloque
        // (Si todo esta bien esto deberia llevarnos al byte 0 del bloque) A CHECKEAR
        posicionAbsoluta = obtener_posicion_absoluta(fcbArchivo, puntero);
        // Si el desplazamiento en el bloque es 0 deberia ser lo mismo hacer 
        // posicionAbsoluta = obtener_bloque_absoluto(fcbArchivo, puntero);

        // Nos movemos a la posición desdeada 
        fseek(archivoDeBloques, posicionAbsoluta, SEEK_SET);
        if (bytesQueFaltanPorLeer < tamanioBloques)
        {
            fread(buffer, sizeof(char), bytesQueFaltanPorLeer, archivoDeBloques);
            // Pasar la data leida del buffer al char *informacion.
            memcpy(informacion+bytesLeidos, buffer, bytesQueFaltanPorLeer);
            //bytesLeidos += bytesQueFaltanPorLeer; --> creo que ya no hace falta
            bytesQueFaltanPorLeer = 0;
        }
        else
        {
            fread(buffer, sizeof(char), tamanioBloques, archivoDeBloques);
            memcpy(informacion+bytesLeidos, buffer, tamanioBloques);
            puntero += tamanioBloques;
            bytesLeidos += tamanioBloques;
            bytesQueFaltanPorLeer -= tamanioBloques;
        }
        free(buffer);
    }

    // Enviar información a memoria para ser escrita a partir de la dirección física 
    solicitar_escritura_memoria(direccionFisica, cantidadBytes, informacion);
    
    // Esperar su finalización para poder confirmar el éxito de la operación al Kernel.
    respuestaMemoria = recibir_buffer_confirmacion_escritura_memoria();
    if (respuestaMemoria)
    {
        enviar_confirmacion_fread_finalizado();
    }
    log_lectura_archivo(nombreArchivo, punteroProceso, direccionFisica, cantidadBytes);
    
    return;
}

// FWRITE

void escribir_archivo(char *nombreArchivo, uint32_t punteroProceso, uint32_t direccionFisica, uint32_t cantidadBytesAEscribir)
{
    uint32_t posicion, puntero;
    uint32_t bloqueActual, nuevoBloque, espacioDisponible;
    uint32_t bytesAEscribirEnBloque, bytesPorEscribir, bytesEscritos;

    bytesPorEscribir = cantidadBytesAEscribir;
    bytesEscritos = 0;

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

    // Escribir la información en los bloques correspondientes del archivo a partir del puntero recibido. 

    bloqueActual = obtener_bloque_absoluto(fcbArchivo, punteroProceso);
    posicion = obtener_posicion_absoluta(fcbArchivo, punteroProceso);
    espacioDisponible = espacio_disponible_en_bloque_desde_posicion(punteroProceso);

    // Si se tienen que escribir menos bytes de los que hay disponibles con escribir solo en este bloque alcanza.
    if (cantidadBytesAEscribir <= espacioDisponible)
    {
        escribir_en_bloque(posicion, cantidadBytesAEscribir, informacionAEscribir);
        bytesEscritos = cantidadBytesAEscribir;
    }

    /* Si se tienen que escribir más bytes de los que hay disponibles hay que escribir una parte en este bloque
    y el resto en el/los bloques siguientes */
    while (bytesEscritos < cantidadBytesAEscribir) {
        if (espacioDisponible == 0)
        {
            nuevoBloque = buscar_siguiente_bloque(bloqueActual,fcbArchivo);
            bloqueActual = nuevoBloque;
            /* el puntero ahora deberia apuntar al nuevo bloque: nuevo bloque * el tamaño de bloque */
            puntero = nuevoBloque * tamanioBloques;
            // en base al nuevo puntero calculo la nueva posicion
            posicion = obtener_posicion_absoluta(fcbArchivo, puntero);
            espacioDisponible = tamanioBloques;
        }

        //Cuántos bytes voy a poder escribir en el bloque
        bytesAEscribirEnBloque = espacioDisponible;

        if (bytesPorEscribir <= tamanioBloques)
        {
            escribir_en_bloque(posicion, bytesPorEscribir, informacionAEscribir);
            bytesEscritos += bytesPorEscribir;
        }
        else
        {  /* --> VER CÓMO "ACTUALIZAR" informacionAEscribir PARA QUE EN LA PROXIMA ITERACIÓN SIGA ESCRIBIENDO DESDE DONDE SE QUEDÓ */
            escribir_en_bloque(posicion, espacioDisponible, informacionAEscribir);
            bytesEscritos += espacioDisponible;
        }

        // Cantidad de bytes que falta escribir. 
        bytesPorEscribir = cantidadBytesAEscribir - bytesEscritos;
        espacioDisponible = 0;
    }

    log_escritura_archivo(nombreArchivo, punteroProceso, direccionFisica, cantidadBytesAEscribir);
    enviar_confirmacion_fwrite_finalizado();
}

void escribir_en_bloque(uint32_t posicion, uint32_t cantidadBytesAEscribir, char *informacionAEscribir)
{
    archivoDeBloques = abrir_archivo_de_bloques();
    fseek(archivoDeBloques,posicion,SEEK_SET);
    fwrite(informacionAEscribir,sizeof(char),cantidadBytesAEscribir,archivoDeBloques);
    fclose(archivoDeBloques);
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