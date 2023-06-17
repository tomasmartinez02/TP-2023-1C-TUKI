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


void desasignar_ultimo_bloque(t_fcb *fcbArchivo)
{   
    uint32_t cero = 0;
    uint32_t bloqueADesasignar;
    uint32_t punteroIndirecto = fcb_get_puntero_indirecto(fcbArchivo);
    uint32_t tamanioBloques = get_superbloque_block_size(superbloque);

    // ABRIR EL ARCHIVO DE BLOQUES
    FILE *archivoBloques = abrir_archivo_de_bloques();
    if (archivoBloques == NULL)
    {
        log_error(filesystemDebuggingLogger, "Error al desasignar el último bloque");
        return;
    }

    // CALCULAR DESPLAZAMIENTO
    // Para saber la cantidad de punteros que hay en el bloque indirecto, tomo la cantidad total de bloques asignados y le
    // resto 1 por el puntero directo (el primer bloque) y le resto 1 por el puntero indirecto (bloque de punteros)
    uint32_t cantidadPunterosEnBloqueIndirecto = fcb_get_cantidad_bloques_asignados(fcbArchivo) - 1 - 1;

    // Me muevo en el archivo de bloques al inicio del bloque indirecto del fcb;
    //fseek(archivoBloques,punteroIndirecto*tamanioBloques,SEEK_SET);
    // Ahora me quiero mover al último puntero del bloque de punteros
    // Multiplico el tamaño de los punteros por el puntero anterior al que quiero acceder para quedar en posicion para leerlo
    //fseek(archivoBloques, sizeof(uint32_t)*(cantidadPunterosEnBloqueIndirecto-1), SEEK_CUR);

    // DESPLAZARSE HASTA EL ÚLTIMO PUNTERO EN EL BLOQUE DE PUNTEROS
    uint32_t desplazamiento = punteroIndirecto*tamanioBloques + sizeof(uint32_t)*(cantidadPunterosEnBloqueIndirecto-1);
    fseek(archivoBloques, desplazamiento, SEEK_SET);
    
    // LEO CUAL ES EL ÚLTIMO PUNTERO
    fread(&bloqueADesasignar, sizeof(uint32_t), 1, archivoBloques);
    // ESCRIBO CERO SOBRE EL ÚLTIMO PUNTERO PARA MARCAR QUE ESTÁ DESOCUPADI
    fseek(archivoBloques, -sizeof(uint32_t), SEEK_CUR);
    fwrite(&cero, sizeof(uint32_t), 1, archivoBloques);

    bitmap_marcar_bloque_libre(bloqueADesasignar);
    uint32_t nuevaCantidadDeBloques = fcb_get_cantidad_bloques_asignados(fcbArchivo) - 1;
    uint32_t nuevoTamanio = fcb_get_tamanio_archivo(fcbArchivo) - tamanioBloques;
    fcb_set_cantidad_bloques_asignados(fcbArchivo, nuevaCantidadDeBloques);
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

int32_t archivo_de_bloques_leer_primer_puntero_de_bloque_de_punteros(uint32_t bloque)
{
    uint32_t tamanioBloques = get_superbloque_block_size(superbloque);
    uint32_t desplazamiento = bloque * tamanioBloques;
    int32_t punteroLeido;
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

int32_t archivo_de_bloques_leer_ultimo_puntero_de_bloque_de_punteros(uint32_t bloque)
{
    // TODO
    return 0;
}
