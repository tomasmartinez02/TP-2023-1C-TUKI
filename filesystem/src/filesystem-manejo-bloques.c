#include <filesystem-manejo-bloques.h>


void asignar_puntero_directo(t_fcb *fcbArchivo)
{
    uint32_t bloque = bitmap_encontrar_bloque_libre();
    fcbArchivo->PUNTERO_DIRECTO = bloque; // mapear el fcb y hacer un msync aca
    bitmap_marcar_bloque_ocupado(bloque);
}

void asignar_puntero_indirecto(t_fcb *fcbArchivo,uint32_t cantidadPunteros)
{
    uint32_t bloque = bitmap_encontrar_bloque_libre();
    fcbArchivo->PUNTERO_INDIRECTO = bloque; // mapear el fcb y hacer un msync aca
    bitmap_marcar_bloque_ocupado(bloque);

    // asignar los punteros al bloque de punteros segun la cantidad pasada por parametro
}

// esta implementacion es solo para archivos nuevos o archivos con tamaño 0 y sin punteros
void asignar_bloques_archivo_vacio(t_fcb *fcbArchivo,uint32_t tamanioNuevo)
{
    uint32_t tamanioBloques = get_superbloque_block_size(superbloque);

    // Si el tamanio del bloque alcanza, se le asigna solo el puntero directo
    if (tamanioNuevo <= tamanioBloques) {
        asignar_puntero_directo(fcbArchivo);
    }
    else {
        // cantidad de punteros que deberia haber en el bloque de punteros
        uint32_t cantidadPunteros = tamanioNuevo / tamanioBloques - 1;

        asignar_puntero_directo(fcbArchivo);
        asignar_puntero_indirecto(fcbArchivo,cantidadPunteros);
    }
}

// esta implementacion es para archivos que ya tienen punteros asignados
/*void asignar_bloques_archivo_no_vacio(fcbArchivo,tamanioNuevo)
{
    asignar_puntero_indirecto(t_fcb *fcbArchivo);
}*/


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

// Los indices arrancan en 0, osea para leer el tercer puntero hay que pasar 2 --> [0, 1, 2]
int32_t archivo_de_bloques_leer_n_puntero_de_bloque_de_punteros(uint32_t bloque, uint32_t punteroN)
{   
    int32_t punteroLeido;
    uint32_t tamanioBloques = get_superbloque_block_size(superbloque);
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
    // resto 1 por el puntero directo (el primer bloque) y le resto 1 por el puntero indirecto (bloque de punteros)
    uint32_t cantidadBloquesAsignados = fcb_get_cantidad_bloques_asignados(fcb) - 1 - 1;
    // Si hay 4 punteros en el bloque de punteros, quiero acceder el puntero número 3 --> Bloque X: [Ptr.0 , Ptr. 1, Ptr. 2, Ptr. 3]
    uint32_t punteroAAcceder = cantidadBloquesAsignados - 1; 
    uint32_t ultimoPuntero = archivo_de_bloques_leer_n_puntero_de_bloque_de_punteros(bloquePunteros, punteroAAcceder);
    return ultimoPuntero;
}

// DESASIGNAR
// 1. Buscar último bloque asignado en el bloque de punteros.
// 2. Marcarlo como vacio en el bitmap.
// 3. Disminuir el tamaño del archivo y la cantidad de bloques asignados.
void desasignar_ultimo_bloque(t_fcb *fcbArchivo)
{   
    uint32_t ultimoBloque;
    uint32_t tamanioBloques = get_superbloque_block_size(superbloque);

    // ABRIR EL ARCHIVO DE BLOQUES
    FILE *archivoBloques = abrir_archivo_de_bloques();
    if (archivoBloques == NULL)
    {
        log_error(filesystemDebuggingLogger, "Error al desasignar el último bloque");
        return;
    }
    
    ultimoBloque = leer_ultimo_puntero_de_bloque_de_punteros(fcbArchivo);

    /* LLENO AL BLOQUE VACIO CON 0 --> no se si hace falta
    uint32_t desplazamiento = ultimoBloque * tamanioBloques;
    fseek(archivoBloques, desplazamiento, SEEK_SET);
    fwrite(&cero, sizeof(uint32_t), tamanioBloques/sizeof(uint32_t), archivoBloques);
    */
    bitmap_marcar_bloque_libre(ultimoBloque);

    //ACTUALIZAR FCB 
    // El archivo tiene un bloque asignado menos.
    uint32_t nuevaCantidadDeBloques = fcb_get_cantidad_bloques_asignados(fcbArchivo) - 1;
    fcb_set_cantidad_bloques_asignados(fcbArchivo, nuevaCantidadDeBloques);
    uint32_t nuevoTamanio = fcb_get_tamanio_archivo(fcbArchivo) - tamanioBloques;
    fcb_set_tamanio_archivo(fcbArchivo, nuevoTamanio);

    fclose(archivoBloques);
}

void desasignar_bloques(t_fcb *fcbArchivo, uint32_t cantidadBloquesDesasignar)
{
    for (uint32_t i = 0; i<cantidadBloquesDesasignar; i++)
    {
        desasignar_ultimo_bloque(fcbArchivo);
    }
    persistir_fcb(fcbArchivo);
}