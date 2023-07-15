#include <filesystem-manejo-bloques.h>

void asignar_puntero_directo(t_fcb *fcbArchivo)
{
    uint32_t bloque = bitmap_encontrar_bloque_libre();
    fcb_set_puntero_directo(fcbArchivo, bloque);
    bitmap_marcar_bloque_ocupado(bloque);
    fcb_incrementar_cantidad_bloques_asignados(fcbArchivo);
    fcb_incrementar_tamanio_en_bloque(fcbArchivo);
    log_info(filesystemLogger, "Se asigna el bloque <%u> como bloque directo.", bloque);
    return;
}

void asignar_puntero_indirecto(t_fcb *fcbArchivo)
{
    uint32_t bloquePunteros = bitmap_encontrar_bloque_libre();
    fcb_set_puntero_indirecto(fcbArchivo, bloquePunteros);
    bitmap_marcar_bloque_ocupado(bloquePunteros);
    log_info(filesystemLogger, "Se asigna el bloque <%u> como bloque de punteros.", bloquePunteros);
    return;
}

// Asignar punteros dentro del bloque de punteros
void asignar_bloques(t_fcb *fcbArchivo, uint32_t cantidadBloques)
{   
    char* nombreArchivo = fcb_get_nombre_archivo(fcbArchivo); // checkear free
    uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcbArchivo);
    uint32_t cantidadPunteros = obtener_cantidad_punteros_bloque_indirecto(fcbArchivo);

    uint32_t desplazamientoArchivoBloque = bloquePunteros * tamanioBloques;
    uint32_t desplazamientoBloque = cantidadPunteros * sizeof(uint32_t);
    uint32_t desplazamiento = desplazamientoArchivoBloque + desplazamientoBloque;

    archivoDeBloques = abrir_archivo_de_bloques();
    fseek(archivoDeBloques, desplazamiento, SEEK_SET);

    for (uint32_t i = 0; i < cantidadBloques; i++) {
        // El bloque de punteros siempre va a ser el bloque número 1 del archivo y el 
        // bloque al que apunta el puntero indrecto va a ser el 0
        uint32_t bloqueDatos = bitmap_encontrar_bloque_libre();
        bitmap_marcar_bloque_ocupado(bloqueDatos);
        log_bloque_asignado(nombreArchivo, bloqueDatos);
        fwrite(&bloqueDatos, sizeof(uint32_t), 1, archivoDeBloques);
    }
    fclose(archivoDeBloques);
    free(nombreArchivo);
    return;
}

// esta implementacion es solo para archivos nuevos o archivos con tamaño 0 y sin punteros
void asignar_bloques_archivo_vacio(t_fcb *fcbArchivo,uint32_t tamanioNuevo)
{
    // Si el tamanio del bloque alcanza, se le asigna solo el puntero directo
    if (tamanioNuevo <= tamanioBloques) {
        asignar_puntero_directo(fcbArchivo);
    }
    else {
        // cantidad de punteros que deberia haber en el bloque de punteros
        uint32_t temp = tamanioNuevo-tamanioBloques;
        uint32_t cantidadPunteros = redondear_hacia_arriba(temp, tamanioBloques);
        asignar_puntero_directo(fcbArchivo);
        asignar_puntero_indirecto(fcbArchivo);

        char* nombreArchivo = fcb_get_nombre_archivo(fcbArchivo); // poner un free
        uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcbArchivo);
        log_acceso_bloque_punteros(nombreArchivo, bloquePunteros);
        sleep(tiempoRetardo);

        asignar_bloques(fcbArchivo, cantidadPunteros);
    }
}

// Archivos que ya tienen punteros asignados
void asignar_bloques_archivo_no_vacio(t_fcb *fcbArchivo, uint32_t tamanioNuevo)
{
    uint32_t cantidadBloquesAsignados = fcb_get_cantidad_bloques_asignados(fcbArchivo);
    uint32_t temp = tamanioNuevo-tamanioBloques; 
    uint32_t cantidadBloques = redondear_hacia_arriba(temp, tamanioBloques);

    if (cantidadBloquesAsignados == 1)
    {
        asignar_puntero_indirecto(fcbArchivo);

        char* nombreArchivo = fcb_get_nombre_archivo(fcbArchivo); // poner un free
        uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcbArchivo);
        log_acceso_bloque_punteros(nombreArchivo, bloquePunteros);
        sleep(tiempoRetardo);
        
        asignar_bloques(fcbArchivo, cantidadBloques);
    }
    else
    {
        char* nombreArchivo = fcb_get_nombre_archivo(fcbArchivo); // poner un free
        uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcbArchivo);
        log_acceso_bloque_punteros(nombreArchivo, bloquePunteros);
        sleep(tiempoRetardo);

        asignar_bloques(fcbArchivo, cantidadBloques);
    }    
    return;
}

// Los indices arrancan en 0, osea para leer el tercer puntero hay que pasar 2 --> [0, 1, 2]
int32_t archivo_de_bloques_leer_n_puntero_de_bloque_de_punteros(uint32_t bloque, uint32_t punteroN)
{   
    int32_t punteroLeido;
    // Desplazamiento para llegar al bloque de punteros 
    uint32_t desplazamientoBloques = bloque * tamanioBloques;
    // Desplazamiento para llegar al puntero correspondiente en el bloque de punteros
    uint32_t desplazamientoPuntero = punteroN * sizeof(uint32_t);
    // Desplazamiento total
    uint32_t desplazamiento = desplazamientoBloques + desplazamientoPuntero;
    
    // ABRIR EL ARCHIVO DE BLOQUES
    archivoDeBloques = abrir_archivo_de_bloques();
    if (archivoDeBloques == NULL)
    {
        log_error(filesystemDebuggingLogger, "Error al abrir el archivo de bloques.");
        return -1; 
    }
    fseek(archivoDeBloques, desplazamiento, SEEK_SET);
    fread(&punteroLeido, sizeof(int32_t), 1, archivoDeBloques);
    fclose(archivoDeBloques);
    return punteroLeido;
}

int32_t archivo_de_bloques_leer_primer_puntero_de_bloque_de_punteros(t_fcb* fcb)
{   
    uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcb);
    // Si quiero acceder al primer puntero del bloque de punteros, quiero acceder al puntero nro. 0 --> Bloque x:[Ptr 0, Ptr, 1, etc]
    uint32_t primerPuntero = archivo_de_bloques_leer_n_puntero_de_bloque_de_punteros(bloquePunteros, 0);
    return primerPuntero;
}

int32_t leer_ultimo_puntero_de_bloque_de_punteros(t_fcb* fcb)
{   
    uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcb);
    // Para saber la cantidad de punteros que hay en el bloque indirecto, tomo la cantidad total de bloques asignados y le
    // resto 1 por el puntero directo (el primer bloque)
    uint32_t cantidadBloquesAsignados = fcb_get_cantidad_bloques_asignados(fcb) - 1;
    // Si hay 4 punteros en el bloque de punteros, quiero acceder el puntero número 3 
    // Su indice va a ser el 2 --> Bloque X: [Ptr.0 , Ptr. 1, Ptr. 2, Ptr. 3]
    uint32_t punteroAAcceder = cantidadBloquesAsignados - 1; 
    int32_t ultimoPuntero = archivo_de_bloques_leer_n_puntero_de_bloque_de_punteros(bloquePunteros, punteroAAcceder);
    return ultimoPuntero;
}

// DESASIGNAR
// 1. Buscar último bloque asignado en el bloque de punteros.
// 2. Marcarlo como vacio en el bitmap.
// 3. Disminuir el tamaño del archivo y la cantidad de bloques asignados.
void desasignar_ultimo_bloque(t_fcb *fcbArchivo)
{   
    char* nombreArchivo = fcb_get_nombre_archivo(fcbArchivo);
    uint32_t ultimoBloque;

    // ABRIR EL ARCHIVO DE BLOQUES
    FILE *archivoBloques = abrir_archivo_de_bloques();
    if (archivoBloques == NULL)
    {
        log_error(filesystemLogger, "Error al desasignar el último bloque.");
        return;
    }
    
    ultimoBloque = leer_ultimo_puntero_de_bloque_de_punteros(fcbArchivo);

    bitmap_marcar_bloque_libre(ultimoBloque);

    log_bloque_desasignado(nombreArchivo, ultimoBloque);

    //ACTUALIZAR FCB  --> El archivo tiene un bloque asignado menos.
    fcb_decrementar_cantidad_bloques_asignados(fcbArchivo);
    fcb_decrementar_tamanio_en_bloque(fcbArchivo);
    fclose(archivoBloques);
}
void desasignar_bloque_punteros(t_fcb *fcbArchivo)
{
    uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcbArchivo);
    fcb_set_puntero_indirecto(fcbArchivo, 0); 
    bitmap_marcar_bloque_libre(bloquePunteros);
    log_info(filesystemLogger, "Bloque de Punteros Desasignado.");
    return;
}

void desasignar_puntero_directo(t_fcb *fcbArchivo)
{
    uint32_t punteroDirecto = fcb_get_puntero_directo(fcbArchivo);
    fcb_set_puntero_directo(fcbArchivo, 0); 
    bitmap_marcar_bloque_libre(punteroDirecto);
    log_info(filesystemLogger, "Puntero Directo Desasignado.");
    return;
}

void desasignar_bloques(t_fcb *fcbArchivo, uint32_t cantidadBloquesDesasignar)
{   
    for (uint32_t i = 0; i<cantidadBloquesDesasignar; i++)
    {
        desasignar_ultimo_bloque(fcbArchivo);
    }
    uint32_t cantidadBloquesAsignadosActual = fcb_get_cantidad_bloques_asignados(fcbArchivo);

    // Si el archivo quedo solamente con un bloque asignado significa que su bloque de punteros está vacio
    if (cantidadBloquesAsignadosActual == 1)
    {
        desasignar_bloque_punteros(fcbArchivo);
    } 
}

void vaciar_archivo(t_fcb *fcbArchivo)
{   
    log_info(filesystemLogger, "Se vacia el archivo.");
    uint32_t cantidadBloquesDesasignar = fcb_get_cantidad_bloques_asignados(fcbArchivo);
    if (cantidadBloquesDesasignar == 1)
    {
        desasignar_puntero_directo(fcbArchivo);
    }
    else
    {
        cantidadBloquesDesasignar--;
        desasignar_bloques(fcbArchivo, cantidadBloquesDesasignar);
        desasignar_puntero_directo(fcbArchivo);
    }
    fcb_set_cantidad_bloques_asignados(fcbArchivo, 0);
    fcb_set_tamanio_archivo(fcbArchivo, 0);
}

uint32_t obtener_bloque_absoluto(t_fcb* fcbArchivo, uint32_t punteroFseek)
{   
    uint32_t bloqueAbsoluto, punteroBloque, bloquePunteros, bloqueRelativo;
    bloqueRelativo = redondear_hacia_abajo(punteroFseek, tamanioBloques);
    // Si la posicion relativa es menor al tamaño del bloque significa que se quiere acceder al 
    // bloque 0 del archivo, que es apuntado por el puntero directo
    if (bloqueRelativo == 0)
    {              
        bloqueAbsoluto = fcb_get_puntero_directo(fcbArchivo);
    }
    // Si la posicion relativa es mayor al tamaño del bloque significa que se quiere acceder
    // a un puntero del bloque de punteros.
    else
    {   
        bloquePunteros = fcb_get_puntero_indirecto(fcbArchivo);
        // Si se quiere acceder al bloque numero 3 del archivo, se quiere acceder al segundo puntero del bloque de punteros.
        punteroBloque = bloqueRelativo - 1;
        bloqueAbsoluto = archivo_de_bloques_leer_n_puntero_de_bloque_de_punteros(bloquePunteros, punteroBloque);
    }
    return bloqueAbsoluto;
}

uint32_t obtener_bloque_relativo(t_fcb* fcbArchivo, uint32_t punteroFseek)
{   
    uint32_t bloqueRelativo;
    uint32_t resto = punteroFseek % tamanioBloques;
    uint32_t division = punteroFseek / tamanioBloques;

    if (resto == 0 && punteroFseek != 0)
    {
        return division+1;
    }
    bloqueRelativo = redondear_hacia_arriba(punteroFseek, tamanioBloques);
    return bloqueRelativo;
}

uint32_t obtener_posicion_en_bloque(uint32_t punteroFseek)
{
    uint32_t posicion, bloqueRelativo;
    bloqueRelativo = redondear_hacia_abajo(punteroFseek, tamanioBloques);
    posicion = punteroFseek - tamanioBloques * bloqueRelativo;
    return posicion;
}

uint32_t espacio_disponible_en_bloque_desde_posicion(uint32_t punteroFseek)
{
    uint32_t posicion, espacioDisponible;
    posicion = obtener_posicion_en_bloque(punteroFseek);
    espacioDisponible = tamanioBloques - posicion;
    return espacioDisponible;
}

// Funcion que sirve para saber desde donde empezar a leer/escribir.
uint32_t obtener_posicion_absoluta(t_fcb* fcbArchivo, uint32_t punteroFseek)
{
    uint32_t bloqueAbsoluto, desplazamientoAlBloque, desplazamientoEnBloque, desplazamiento;

    bloqueAbsoluto = obtener_bloque_absoluto(fcbArchivo, punteroFseek);
    // Posiciona al comienzo del bloque
    desplazamientoAlBloque = bloqueAbsoluto * tamanioBloques;
    // Posiciona al byte del bloque
    desplazamientoEnBloque = obtener_posicion_en_bloque(punteroFseek);
    desplazamiento = desplazamientoAlBloque + desplazamientoEnBloque; 
    return desplazamiento;
}

uint32_t espacio_disponible_en_bloque(uint32_t posicionEnBloque)
{
    uint32_t espacioDisponible;

    espacioDisponible = tamanioBloques - posicionEnBloque;
    return espacioDisponible;
}

uint32_t buscar_siguiente_bloque(uint32_t bloqueActual, t_fcb *fcbArchivo)
{
    uint32_t siguienteBloque;
    uint32_t punteroDirecto = fcb_get_puntero_directo(fcbArchivo);
    uint32_t punteroIndirecto = fcb_get_puntero_indirecto(fcbArchivo);
    
    char * nombreArchivo = fcb_get_nombre_archivo(fcbArchivo);
    sleep(tiempoRetardo);
    log_acceso_bloque_punteros(nombreArchivo,punteroIndirecto);
    free(nombreArchivo);

    if (bloqueActual == punteroDirecto) {
        /* el proximo bloque va a ser el primer bloque de datos al que se
        apunte en el bloque de punteros */
        siguienteBloque = archivo_de_bloques_leer_primer_puntero_de_bloque_de_punteros(fcbArchivo);
        return siguienteBloque;
    }
    else {
        uint32_t desplazamiento = (punteroIndirecto * tamanioBloques);

        archivoDeBloques = abrir_archivo_de_bloques();

        fseek(archivoDeBloques,desplazamiento,SEEK_SET);
        
        while (fread(&siguienteBloque,sizeof(uint32_t),1,archivoDeBloques)) {
            if (siguienteBloque == bloqueActual) {
                fread(&siguienteBloque,sizeof(uint32_t),1,archivoDeBloques);
                fclose(archivoDeBloques);
                return siguienteBloque;
            }
        }
        return 0; // sino tira warning
    }
}