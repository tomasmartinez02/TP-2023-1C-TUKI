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

void sleep_y_log_obligatorio(t_fcb *fcbArchivo, uint32_t punteroProceso)
{
    uint32_t numeroBloqueArchivo, numeroBloqueFS;
    char* nombreArchivo = fcb_get_nombre_archivo(fcbArchivo);
    //sleep(tiempoRetardo);
    numeroBloqueFS = obtener_bloque_absoluto(fcbArchivo, punteroProceso);
    numeroBloqueArchivo = obtener_bloque_relativo(fcbArchivo, punteroProceso);
    log_acceso_bloque(nombreArchivo, numeroBloqueArchivo, numeroBloqueFS);
    return;
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
    sleep(tiempoRetardo);
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

// F_READ

// Leer la información correspondiente de los bloques a partir del puntero y el tamaño recibido
void leer_archivo(char *nombreArchivo, uint32_t punteroProceso, uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pidProceso)
{   
    log_info(filesystemLogger,"Entra a leer archivo"); // LOG A SACAR
    uint32_t posicionAbsoluta, espacioDisponible, puntero, bytesLeidos, bytesQueFaltanPorLeer;
    size_t rtaLectura;
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
    log_info(filesystemLogger,"Va a empezar a leer desde la posicion <%u>.", posicionAbsoluta); // LOG A SACAR

    espacioDisponible = espacio_disponible_en_bloque_desde_posicion(punteroProceso);

    archivoDeBloques = abrir_archivo_de_bloques();
    fseek(archivoDeBloques, posicionAbsoluta, SEEK_SET);
    log_info(filesystemLogger, "Posicion desde la que vamos a leer: %ld", ftell(archivoDeBloques)); // LOG A SACAR

    /* Si la cantidad de bytes a leer es menor que el espacio que queda en el  bloque seleccionado,
    solamente se accede a ese bloque. */
    if (cantidadBytes < espacioDisponible)
    {   
        log_info(filesystemLogger,"La cantidad de bytes a leer <%u> es menor al espacio disponible <%u> del bloque", cantidadBytes, espacioDisponible); // LOG A SACAR

        rtaLectura = fread(buffer, sizeof(char), cantidadBytes, archivoDeBloques);
        if (rtaLectura!=cantidadBytes || ferror(archivoDeBloques))
        {
            log_error(filesystemLogger, "El archivo de bloques no se leyo correctamente.");
        }
        // Se mueve el puntero del archivo hasta el ultimo lugar que se leyo.
        puntero = puntero + espacioDisponible; 
        // Contador de bytes leidos.
        bytesLeidos = cantidadBytes;
        // Contador de bytes que faltan por leer.
        bytesQueFaltanPorLeer = bytesQueFaltanPorLeer - bytesLeidos;
        
        // Ya se leyeron todos los bytes que habia que leer, no hay que leer nada mas.
        // Pasar la data leida del buffer al char *informacion.
        memcpy(informacion, buffer, bytesLeidos);
        log_info(filesystemLogger,"Leyo <%s>", informacion); // LOG A SACAR

        free(buffer);
        fclose(archivoDeBloques);
    }

    /* Si la cantidad de bytes a leer es mayor que el espacio que queda en el bloque eleccionado, se leen todos los bytes
    que queden en el bloque antes de pasar al siguiente. */
    else
    {
        rtaLectura = fread(buffer, sizeof(char), espacioDisponible, archivoDeBloques);
        if (rtaLectura!=espacioDisponible || ferror(archivoDeBloques))
        {
            log_error(filesystemLogger, "El archivo de bloques no se leyo correctamente.");
        }

        log_info(filesystemLogger,"Leyo <%lu> cant bytes", rtaLectura); // LOG A SACAR
        // Se mueve el puntero del archivo hasta el ultimo lugar que se leyo.
        puntero = puntero + espacioDisponible; 
        log_info(filesystemLogger,"Ahora el puntero del proceso apunta a <%u> ", puntero); // LOG A SACAR
        // Contador de bytes leidos.
        bytesLeidos = espacioDisponible;
        // Contador de bytes que faltan por leer.
        bytesQueFaltanPorLeer = bytesQueFaltanPorLeer - bytesLeidos;

        // Pasar la data leida del buffer al char *informacion.
        memcpy(informacion, buffer, bytesLeidos);

        void* bufferLeido = (void*)buffer;
        char* infoLeida = agregarCaracterNulo(bufferLeido);

        log_info(filesystemLogger,"Leyo <%s>", infoLeida); // LOG A SACAR
        free(buffer);
        fclose(archivoDeBloques);
    }

    // Repetir hasta que se hayan leido todos los bytes.
    while(bytesQueFaltanPorLeer != 0)
    {   
        log_info(filesystemLogger,"Faltan leer <%u> bytes de los <%u> totales", bytesQueFaltanPorLeer, cantidadBytes); // LOG A SACAR
        buffer = malloc(tamanioBloques);

        // Se busca la posicion del siguiente bloque
        posicionAbsoluta = obtener_posicion_absoluta(fcbArchivo, puntero);
        log_info(filesystemLogger,"Se posiciona en la posicion absoluta: <%u>.", posicionAbsoluta); // LOG A SACAR
        // Nos movemos a la posición desdeada 
        archivoDeBloques = abrir_archivo_de_bloques();
        fseek(archivoDeBloques, posicionAbsoluta, SEEK_SET);
        log_info(filesystemLogger, "Posicion desde la que se va a leer: %ld", ftell(archivoDeBloques)); // LOG A SACAR
        if (bytesQueFaltanPorLeer < tamanioBloques)
        {
            rtaLectura = fread(buffer, sizeof(char), bytesQueFaltanPorLeer, archivoDeBloques);
            if (rtaLectura!=bytesQueFaltanPorLeer || ferror(archivoDeBloques))
            {
                log_error(filesystemLogger, "El archivo de bloques no se leyo correctamente.");
            }
            
            // Pasar la data leida del buffer al char *informacion.
            memcpy(informacion+bytesLeidos, buffer, bytesQueFaltanPorLeer);

            bytesLeidos = bytesLeidos + bytesQueFaltanPorLeer; 
            bytesQueFaltanPorLeer = 0;

            log_info(filesystemLogger,"Hasta ahora se leyeron <%u> bytes.", bytesLeidos); // LOG A SACAR
            log_info(filesystemLogger,"Leyo <%s>", buffer); // LOG A SACAR
            log_info(filesystemLogger,"Hasta ahora leyo <%s>", informacion); // LOG A SACAR
            log_info(filesystemLogger,"Faltan leer <%u> bytes.", bytesQueFaltanPorLeer); // LOG A SACAR
        }
        else
        {
            rtaLectura = fread(buffer, sizeof(char), tamanioBloques, archivoDeBloques);
            if (rtaLectura!=tamanioBloques || ferror(archivoDeBloques))
            {
                log_error(filesystemLogger, "El archivo de bloques no se leyo correctamente.");
            }
            memcpy(informacion+bytesLeidos, buffer, tamanioBloques);
            puntero = puntero + tamanioBloques;
            bytesLeidos = bytesLeidos + tamanioBloques;
            bytesQueFaltanPorLeer = bytesQueFaltanPorLeer - tamanioBloques;

            log_info(filesystemLogger,"Hasta ahora se leyeron <%u> bytes.", bytesLeidos); // LOG A SACAR
            log_info(filesystemLogger,"Faltan leer <%u> bytes.", bytesQueFaltanPorLeer); // LOG A SACAR
            log_info(filesystemLogger,"Leyo <%s>", buffer); // LOG A SACAR
            log_info(filesystemLogger,"Hasta ahora leyo <%s>", informacion); // LOG A SACAR
        }
        free(buffer);
        fclose(archivoDeBloques);
    }

    log_info(filesystemLogger,"FS le manda <%s> a memoria.", informacion); // LOG A SACAR
    // Enviar información a memoria para ser escrita a partir de la dirección física 
    solicitar_escritura_memoria(direccionFisica, cantidadBytes, informacion, pidProceso);
    free(informacion);

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
void escribir_archivo(char *nombreArchivo, uint32_t punteroProceso, uint32_t direccionFisica, uint32_t cantidadBytesAEscribir, uint32_t pidProceso)
{
    log_info(filesystemLogger,"Entra a escribir archivo"); // LOG A SACAR
    uint32_t posicion;
    // el 'bloqueActual' es el absoluto
    uint32_t bloqueActual, bloqueRelativo, nuevoBloque, espacioDisponible;
    uint32_t bytesPorEscribir, bytesEscritos;
    bytesPorEscribir = cantidadBytesAEscribir;
    bytesEscritos = 0;

    log_info(filesystemLogger,"Bytes por escribir es <%u>",bytesPorEscribir); // LOG A SACAR

    // Busco el fcb relacionado al archivo en el que se quiere escribir
    t_fcb *fcbArchivo = dictionary_get(listaFcbs, nombreArchivo);
    if (fcbArchivo == NULL)
    {
        log_error(filesystemLogger, "No se encontró el fcb en la lista de fcbs.");
        log_error(filesystemDebuggingLogger, "No se encontró el fcb en la lista de fcbs.");
        return;
    }

    // Solicitar a la Memoria la información que se encuentra a partir de la dirección física
    solicitar_informacion_memoria(direccionFisica, cantidadBytesAEscribir, pidProceso);
    log_info(filesystemLogger,"Se solicitó la información a memoria"); // LOG A SACAR

    void *informacion = recibir_buffer_informacion_memoria(cantidadBytesAEscribir);

    //char* infoImprimible = agregarCaracterNulo(informacion); // no hace falta
    //log_info(filesystemLogger,"Recibió: <%s>", infoImprimible); // LOG A SACAR

    char* informacionAEscribir = (char*)informacion;
    free(informacion);

    log_info(filesystemLogger,"Recibió: <%s>", informacionAEscribir);

    // Escribir la información en los bloques correspondientes del archivo a partir del puntero recibido.   
    bloqueActual = obtener_bloque_absoluto(fcbArchivo, punteroProceso);
    log_info(filesystemLogger,"Va a empezar a escribir en el bloque <%u> del archivo de bloques.", bloqueActual); // LOG A SACAR
    bloqueRelativo = obtener_bloque_relativo(fcbArchivo, punteroProceso);
    log_info(filesystemLogger,"Va a empezar a escribir en el bloque <%u> del archivo.", bloqueRelativo); // LOG A SACAR
    posicion = obtener_posicion_absoluta(fcbArchivo, punteroProceso);
    log_info(filesystemLogger,"Va a empezar a escribir desde la posicion <%u>.", posicion); // LOG A SACAR

    espacioDisponible = espacio_disponible_en_bloque_desde_posicion(punteroProceso);

    // Si se tienen que escribir menos bytes de los que hay disponibles con escribir solo en este bloque alcanza.
    if (cantidadBytesAEscribir <= espacioDisponible)
    {
        log_info(filesystemLogger,"La cantidad de bytes a escribir <%u> es menor al espacio disponible <%u> en el bloque.",cantidadBytesAEscribir,espacioDisponible); // LOG A SACAR
        log_info(filesystemLogger,"Tiempo de retardo de acceso a bloque <%u>.",tiempoRetardo);
        sleep(tiempoRetardo);
        log_acceso_bloque(nombreArchivo,bloqueRelativo,bloqueActual);
        escribir_en_bloque(posicion, cantidadBytesAEscribir, informacionAEscribir);
        bytesEscritos = cantidadBytesAEscribir;
    }

    char *buffer = malloc(sizeof(tamanioBloques));
    
    /* Si se tienen que escribir más bytes de los que hay disponibles hay que escribir una parte en este bloque
    y el resto en los bloques siguientes */
    while (bytesEscritos < cantidadBytesAEscribir)
    {
        log_info(filesystemLogger,"La cantidad de bytes a escribir <%u> es mayor al espacio disponible <%u> en el bloque.",cantidadBytesAEscribir,espacioDisponible); // LOG A SACAR
        
        if (espacioDisponible == 0)
        {
            log_info(filesystemLogger,"Se quedo sin espacio en el bloque, pasa a buscar el siguiente"); // LOG A SACAR
            nuevoBloque = buscar_siguiente_bloque(bloqueActual, fcbArchivo);
            bloqueActual = nuevoBloque;
            log_info(filesystemLogger,"Nuevo bloque: <%u>", bloqueActual); // LOG A SACAR
            posicion = bloqueActual * tamanioBloques;
            log_info(filesystemLogger,"Va a empezar a escribir en la posicion <%u>", posicion); // LOG A SACAR
            bloqueRelativo = obtener_bloque_relativo(fcbArchivo, posicion);
            log_info(filesystemLogger,"El nuevo bloque es el bloque <%u> del archivo.", bloqueRelativo); // LOG A SACAR
            espacioDisponible = tamanioBloques;
        }

        log_info(filesystemLogger,"Espacio disponible: <%u>", espacioDisponible); // LOG A SACAR

        if (bytesPorEscribir > espacioDisponible || bytesPorEscribir >= tamanioBloques)
        {
            log_info(filesystemLogger,"La cantidad de bytes que quedan por escribir <%u> es mayor al espacio disponible <%u>",bytesPorEscribir,espacioDisponible); // LOG A SACAR
            memcpy(buffer,informacionAEscribir+bytesEscritos,espacioDisponible);
            log_info(filesystemLogger,"Tiempo de retardo de acceso a bloque <%u>.",tiempoRetardo);
            sleep(tiempoRetardo);
            log_acceso_bloque(nombreArchivo,bloqueRelativo,bloqueActual);
            escribir_en_bloque(posicion, espacioDisponible, buffer);
            bytesEscritos += espacioDisponible;
            bytesPorEscribir -= espacioDisponible;
        }
        else if (bytesPorEscribir <= tamanioBloques)
        {
            log_info(filesystemLogger,"La cantidad de bytes que quedan por escribir <%u> es menor al tamanio de bloques <%u>",bytesPorEscribir,tamanioBloques); // LOG A SACAR
            memcpy(buffer,informacionAEscribir+bytesEscritos,bytesPorEscribir);
            log_info(filesystemLogger,"Tiempo de retardo de acceso a bloque <%u>.",tiempoRetardo);
            sleep(tiempoRetardo);
            log_acceso_bloque(nombreArchivo,bloqueRelativo,bloqueActual);
            escribir_en_bloque(posicion, bytesPorEscribir, buffer);
            bytesEscritos += bytesPorEscribir;
        }
        log_info(filesystemLogger,"Faltan escribir <%u> bytes", bytesPorEscribir); // LOG A SACAR
        log_info(filesystemLogger,"Hasta ahora se escribieron <%u> bytes", bytesEscritos); // LOG A SACAR
        espacioDisponible = 0;
    }
    free(buffer);

    log_escritura_archivo(nombreArchivo, punteroProceso, direccionFisica, cantidadBytesAEscribir);
    enviar_confirmacion_fwrite_finalizado();
}

void escribir_en_bloque(uint32_t posicion, uint32_t cantidadBytesAEscribir, char *informacionAEscribir)
{
    archivoDeBloques = abrir_archivo_de_bloques();
    fseek(archivoDeBloques,posicion,SEEK_SET);
    fwrite(informacionAEscribir,sizeof(char),cantidadBytesAEscribir,archivoDeBloques);
    log_info(filesystemLogger,"Escribio <%s>", informacionAEscribir); // LOG A SACAR
    log_info(filesystemLogger,"Escribio <%u> bytes en el bloque", cantidadBytesAEscribir); // LOG A SACAR
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
                uint32_t pidProceso;
                recibir_buffer_escritura_archivo(&nombreArchivo, &puntero, &direccionFisica, &cantidadBytes, &pidProceso);
                log_info(filesystemLogger, "FS recibe la solicitud de escribir archivo %s, %d cantidad de bytes, en el puntero %d, direccion fisica %d", nombreArchivo, cantidadBytes, puntero, direccionFisica);
                escribir_archivo(nombreArchivo, puntero, direccionFisica, cantidadBytes, pidProceso);
                free(nombreArchivo);
                break;
            }
            case HEADER_solicitud_leer_archivo:
            {
                char *nombreArchivo = NULL;
                uint32_t direccionFisica;
                uint32_t cantidadBytes;
                uint32_t puntero;
                uint32_t pidProceso;
                recibir_buffer_lectura_archivo(&nombreArchivo, &puntero, &direccionFisica, &cantidadBytes, &pidProceso);
                log_info(filesystemLogger, "FS recibe la solicitud de leer archivo %s, %d cantidad de bytes, en el puntero %d, direccion fisica %d", nombreArchivo, cantidadBytes, puntero, direccionFisica);
                leer_archivo(nombreArchivo, puntero, direccionFisica, cantidadBytes, pidProceso);
                free(nombreArchivo);
                break;
            }
            case HEADER_solicitud_modificar_tamanio_archivo:
            {   
                char *nombreArchivo = NULL;
                uint32_t tamanioNuevo;
                recibir_buffer_truncate_archivo(&nombreArchivo, &tamanioNuevo);
                log_info(filesystemLogger, "FS recibe la solicitud de cambiarle el tamanio (%d) al archivo %s", tamanioNuevo, nombreArchivo );
                truncar_archivo(nombreArchivo, tamanioNuevo);
                enviar_confirmacion_tamanio_archivo_modificado();

                free(nombreArchivo);
                break;
            }
            case HEADER_consulta_existencia_archivo:
            {   
                char* nombreArchivo = recibir_buffer_nombre_archivo();
                log_info(filesystemLogger, "FS recibe la consulta de existencia del archivo %s", nombreArchivo);
                verificar_existencia_archivo(nombreArchivo);
                free(nombreArchivo);
                break;
            }
        }
    }
    return;
}