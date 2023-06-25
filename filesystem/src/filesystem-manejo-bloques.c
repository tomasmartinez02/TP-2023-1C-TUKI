#include <filesystem-manejo-bloques.h>

void asignar_puntero_directo(t_fcb *fcbArchivo)
{
    uint32_t bloque = bitmap_encontrar_bloque_libre();
    fcbArchivo->PUNTERO_DIRECTO = bloque;
    bitmap_marcar_bloque_ocupado(bloque);
    fcb_incrementar_cantidad_bloques_asignados(fcbArchivo);
    fcb_incrementar_tamanio_en_bloque(fcbArchivo);
    log_info(filesystemLogger, "Se asigna al bloque %u como bloque directo.", bloque);
    return;
}

void asignar_puntero_indirecto(t_fcb *fcbArchivo)
{
    uint32_t bloquePunteros = bitmap_encontrar_bloque_libre();
    fcbArchivo->PUNTERO_INDIRECTO = bloquePunteros; 
    bitmap_marcar_bloque_ocupado(bloquePunteros);
    log_info(filesystemLogger, "Se asigna al bloque %u como bloque indirecto.", bloquePunteros);
}

// Asignar punteros dentro del bloque de punteros
void asignar_bloques(t_fcb *fcbArchivo, uint32_t cantidadBloques)
{   
    char* nombreArchivo = fcb_get_nombre_archivo(fcbArchivo);
    uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcbArchivo);
    uint32_t cantidadPunteros = obtener_cantidad_punteros_bloque_indirecto(fcbArchivo);

    uint32_t desplazamientoArchivoBloque = bloquePunteros * tamanioBloques;
    uint32_t desplazamientoBloque = cantidadPunteros * sizeof(uint32_t);
    uint32_t desplazamiento = desplazamientoArchivoBloque + desplazamientoBloque;

    archivoDeBloques = fopen(pathArchivoBloquesHardcodeado, "r+b"); // Modo lectura y escritura binaria ---> PROBAR

    fseek(archivoDeBloques, desplazamiento, SEEK_SET);

    for (uint32_t i = 0; i < cantidadBloques; i++) {

        // El bloque de punteros siempre va a ser el bloque número 1 del archivo y el bloque al que apunta el puntero indrecto va a ser el 0
        log_acceso_bloque_punteros(nombreArchivo, bloquePunteros);
        uint32_t bloqueDatos = bitmap_encontrar_bloque_libre();
        bitmap_marcar_bloque_ocupado(bloqueDatos);
        log_bloque_asignado(nombreArchivo, bloqueDatos);
        fwrite(&bloqueDatos, sizeof(uint32_t), 1, archivoDeBloques);
        //sleep(tiempoRetardo);
        fcb_incrementar_cantidad_bloques_asignados(fcbArchivo);
        fcb_incrementar_tamanio_en_bloque(fcbArchivo);
    }
    fclose(archivoDeBloques);
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
        uint32_t temp = tamanioNuevo-tamanioBloques; // por las dudas de q le agarre la loca pasandole un menos
        uint32_t cantidadPunteros = redondearHaciaArriba(temp, tamanioBloques);
        asignar_puntero_directo(fcbArchivo);
        asignar_puntero_indirecto(fcbArchivo);
        asignar_bloques(fcbArchivo, cantidadPunteros);
    }
}


// Archivos que ya tienen punteros asignados
void asignar_bloques_archivo_no_vacio(t_fcb *fcbArchivo, uint32_t tamanioNuevo)
{
    uint32_t cantidadBloquesAsignados = fcb_get_cantidad_bloques_asignados(fcbArchivo);
    uint32_t temp = tamanioNuevo-tamanioBloques; // por las dudas de q le agarre la loca pasandole un menos
    uint32_t cantidadBloques = redondearHaciaArriba(temp, tamanioBloques);

    if (cantidadBloquesAsignados == 1)
    {
        asignar_puntero_indirecto(fcbArchivo);
        asignar_bloques(fcbArchivo, cantidadBloques);
    }
    else
    {
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
    FILE *archivoBloques = abrir_archivo_de_bloques();
    if (archivoBloques == NULL)
    {
        log_error(filesystemDebuggingLogger, "Error al abrir el archivo de bloques");
        return -1; 
    }
    
    fseek(archivoBloques, desplazamiento, SEEK_SET);
    fread(&punteroLeido, sizeof(uint32_t), 1, archivoBloques);
    fclose(archivoBloques);
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
        log_error(filesystemLogger, "Error al desasignar el último bloque");
        return;
    }
    
    ultimoBloque = leer_ultimo_puntero_de_bloque_de_punteros(fcbArchivo);

    bitmap_marcar_bloque_libre(ultimoBloque);

    log_bloque_desasignado(nombreArchivo, ultimoBloque);

    //ACTUALIZAR FCB 
    // El archivo tiene un bloque asignado menos.
    fcb_decrementar_cantidad_bloques_asignados(fcbArchivo);
    fcb_decrementar_tamanio_en_bloque(fcbArchivo);
    fclose(archivoBloques);
}
    // REVISAR!!!!!!!!!!
void desasignar_bloque_punteros(t_fcb *fcbArchivo)
{
    uint32_t bloquePunteros = fcb_get_puntero_indirecto(fcbArchivo);
    fcb_set_puntero_indirecto(fcbArchivo, 0); // COMO MARCO QUE NO TIENE PUNTERO INDIRECTO ASOCIADO????????
    bitmap_marcar_bloque_libre(bloquePunteros);
    log_info(filesystemLogger, "Bloque de punteros desasignado.");
    return;
}

// REVISAR!!!!!!!
void desasignar_puntero_directo(t_fcb *fcbArchivo)
{
    uint32_t punteroDirecto = fcb_get_puntero_directo(fcbArchivo);
    fcb_set_tamanio_archivo(fcbArchivo, 0);
    fcb_set_puntero_directo(fcbArchivo, 0); // COMO MARCO QUE NO TIENE PUNTERO DIRECTO ASOCIADO????????
    bitmap_marcar_bloque_libre(punteroDirecto);
    log_info(filesystemLogger, "Puntero directo desasignado.");
    return;
}

// HORRIBLE lo se
void desasignar_bloques(t_fcb *fcbArchivo, uint32_t cantidadBloquesDesasignar)
{   
    uint32_t contadorBloquesDesasignados = 0;
    uint32_t cantidadBloquesAsignadosActual = fcb_get_cantidad_bloques_asignados(fcbArchivo);

    // Si tiene 2 bloques asignados significa que tiene el bloque del puntero directo y un bloque en el bloque de punteros
    if (cantidadBloquesAsignadosActual == 2)
    {   
        // Se desasigna el último bloque del bloque de punteros
        desasignar_ultimo_bloque(fcbArchivo);
        cantidadBloquesAsignadosActual = fcb_get_cantidad_bloques_asignados(fcbArchivo);
        contadorBloquesDesasignados++;
    }

    if (cantidadBloquesAsignadosActual > 1) // SI TIENE BLOQUE DE PUNTEROS
    {
        for (uint32_t i = 0; i<cantidadBloquesDesasignar; i++)
        {
            desasignar_ultimo_bloque(fcbArchivo);
            //cantidadBloquesAsignadosActual = fcb_get_cantidad_bloques_asignados(fcbArchivo);
            contadorBloquesDesasignados++;
        }
        cantidadBloquesAsignadosActual = fcb_get_cantidad_bloques_asignados(fcbArchivo);
    }

    // Si el archivo quedo solamente con un bloque asignado significa que su bloque de punteros está vacio
    if (cantidadBloquesAsignadosActual == 1)
    {
        desasignar_bloque_punteros(fcbArchivo);
    }
    
    // Si al terminar de desasignar todavia le falta desasignar uno significa que hay que desasignar el último bloque.
    if ((cantidadBloquesDesasignar - contadorBloquesDesasignados) == 1)
    {
        desasignar_puntero_directo(fcbArchivo);
    }
    
}

void archivo_de_bloques_escribir(t_fcb *fcbArchivo, uint32_t puntero, char *informacion)
{
    /* Si me pasan que escriba desde el puntero 34 y los bloques tienen un tamaño de 64 --> Escribo en el bloque 0, byte 35 
     Si me pasan que escriba en el puntero 129 --> Escribo en el tercer bloque [0, 1, 2] en el primer o segundo? byte
     Puntero / tamaño_bloque --> Resultado = Numero de bloque
     --> Resto = byte del bloque

     desplazamientoAlBloque = numero de bloque * tam_bloque
     desplazamientoDentroDelBloque = byte del bloque
     desplazamientoTotal = desplazamientoAlBloque + desplazamientoDentroDelBloque

     espacioDisponible en bloque = tam_bloque - byte del bloque

     fseek(archivoDeBloques, desplazamientoTotal, SEEK_SET)

     fwrite(informacion, sizeof(char), espacioDisponible)

     moverse_al_siguiente_bloque_Asignado

     fwrite(informacion, sizeof(char), tam_bloque)

     hasta que no haya mas informacion para escribir */
}


// probar!!!!!!!!
char* archivo_de_bloques_leer_bloque(uint32_t bloque)
{
    uint32_t tamanioBloques = get_superbloque_block_size(superbloque);
    uint32_t desplazamiento = bloque * tamanioBloques;
    char *contenido = malloc(tamanioBloques);
    // ABRIR EL ARCHIVO DE BLOQUES
    FILE *archivoBloques = abrir_archivo_de_bloques();
    if (archivoBloques == NULL)
    {
        log_error(filesystemDebuggingLogger, "Error al abrir el archivo de bloques");
        free(contenido);
        return NULL;
    }
    
    fseek(archivoBloques, desplazamiento, SEEK_SET);
    fread(contenido, sizeof(char), tamanioBloques, archivoBloques);
    fclose(archivoBloques);
    return contenido;
}

/*
bool archivo_de_bloques_escribir_en_bloque(uint32_t bloque, char* informacion)
{       
    // ACA LA INFORMACION PASADA DEBERIA SER DEL MISMO TAMAÑO QUE EL BLOQUE, OSEA 
    // HABRIA QUE TENER LA INFORMACION DIVIDA EN PARTECITAS IDK HAY Q VERLO
    uint32_t tamanioBloques = get_superbloque_block_size(superbloque);
    uint32_t desplazamiento = bloque * tamanioBloques;
    // ABRIR EL ARCHIVO DE BLOQUES
    FILE *archivoBloques = abrir_archivo_de_bloques();
    if (archivoBloques == NULL)
    {
        log_error(filesystemDebuggingLogger, "Error al abrir el archivo de bloques");
        return false;
    }
    // DESPLAZAR AL BLOQUE CORRESPONDIENTE
    fseek(archivoBloques, desplazamiento, SEEK_SET);
    fwrite(informacion, sizeof(char), tamanioBloques, archivoBloques);
    fclose(archivoBloques);
    return true;
}
*/
