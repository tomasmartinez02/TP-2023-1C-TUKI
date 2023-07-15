#include <filesystem-fcb.h>

// ESTRUCTURA DE FCBs
t_fcb *crear_nuevo_fcb(char *nombreArchivo)
{
    t_fcb *fcb = malloc(sizeof(*fcb));
    fcb->NOMBRE_ARCHIVO = strdup(nombreArchivo);
    fcb->TAMANIO_ARCHIVO = 0;
    fcb->PUNTERO_DIRECTO = 0;
    fcb->PUNTERO_INDIRECTO = 0;
    fcb->cantidad_bloques_asignados = 0;

    return fcb;
}

void destruir_fcb(t_fcb *fcb)
{
    free(fcb->NOMBRE_ARCHIVO);
    free(fcb);
    return;
}

char *fcb_get_nombre_archivo(t_fcb *fcb)
{
    if (fcb->NOMBRE_ARCHIVO == NULL)
    {
        return NULL;
    }
    return strdup(fcb->NOMBRE_ARCHIVO);
}

void fcb_set_nombre_archivo(t_fcb *fcb, char *nuevoNombreArchivo)
{   
    if (fcb->NOMBRE_ARCHIVO != NULL)
    {
        free(fcb->NOMBRE_ARCHIVO);
    }
    fcb->NOMBRE_ARCHIVO = strdup(nuevoNombreArchivo);
    return;
}

uint32_t fcb_get_tamanio_archivo(t_fcb *fcb)
{
    return fcb->TAMANIO_ARCHIVO;
}

void fcb_set_tamanio_archivo(t_fcb *fcb, uint32_t nuevoTamanio)
{
    fcb->TAMANIO_ARCHIVO = nuevoTamanio;
    return;
}

uint32_t fcb_get_puntero_directo(t_fcb *fcb)
{
    return fcb->PUNTERO_DIRECTO;
}

void fcb_set_puntero_directo(t_fcb *fcb, uint32_t nuevoPunteroDirecto)
{
    fcb->PUNTERO_DIRECTO = nuevoPunteroDirecto;
    return;
}

uint32_t fcb_get_puntero_indirecto(t_fcb *fcb)
{
    return fcb->PUNTERO_INDIRECTO;
}

void fcb_set_puntero_indirecto(t_fcb *fcb, uint32_t nuevoPunteroIndirecto)
{
    fcb->PUNTERO_INDIRECTO = nuevoPunteroIndirecto;
    return;
}

uint32_t fcb_get_cantidad_bloques_asignados(t_fcb *fcb)
{
    return fcb->cantidad_bloques_asignados;
}

void fcb_set_cantidad_bloques_asignados(t_fcb *fcb, uint32_t nuevaCantidadBloques)
{
    fcb->cantidad_bloques_asignados = nuevaCantidadBloques;
    return;
}

void fcb_incrementar_cantidad_bloques_asignados(t_fcb *fcb)
{
    uint32_t cantidadActual = fcb_get_cantidad_bloques_asignados(fcb);
    fcb_set_cantidad_bloques_asignados(fcb, cantidadActual+1);
    return;
}

void fcb_decrementar_cantidad_bloques_asignados(t_fcb *fcb)
{
    uint32_t cantidadActual = fcb_get_cantidad_bloques_asignados(fcb);
    fcb_set_cantidad_bloques_asignados(fcb, cantidadActual-1);
    return;
}

void fcb_decrementar_tamanio_en_bloque(t_fcb *fcb)
{
    uint32_t tamanioBloque = get_superbloque_block_size(superbloque);
    uint32_t nuevoTamanio = fcb_get_tamanio_archivo(fcb) - tamanioBloque;
    fcb_set_tamanio_archivo(fcb, nuevoTamanio);
    return;
}

void fcb_incrementar_tamanio_en_bloque(t_fcb *fcb)
{
    uint32_t tamanioBloque = get_superbloque_block_size(superbloque);
    uint32_t nuevoTamanio = fcb_get_tamanio_archivo(fcb) + tamanioBloque;
    fcb_set_tamanio_archivo(fcb, nuevoTamanio);
    return;
}

uint32_t obtener_cantidad_punteros_bloque_indirecto(t_fcb* fcb)
{   
    uint32_t cantidadPunteros = fcb_get_cantidad_bloques_asignados(fcb) - 1;
    return cantidadPunteros;
}

// Para pruebas
void fcb_mostrar_por_pantalla(t_fcb* fcb)
{   
    log_info(filesystemLogger, "Nombre archivo: %s", fcb->NOMBRE_ARCHIVO);
    log_info(filesystemLogger, "Puntero directo: %u", fcb->PUNTERO_DIRECTO);
    log_info(filesystemLogger, "Puntero indirecto: %u", fcb->PUNTERO_INDIRECTO);
    log_info(filesystemLogger, "Cantidad de bloques asignados: %u", fcb->cantidad_bloques_asignados);
    log_info(filesystemLogger, "Tamanio archivo: %u", fcb->TAMANIO_ARCHIVO);
    return;
}

// ARCHIVOS DE FCBS

t_fcb* levantar_fcb(char *pathFcb)
{
    t_config *config_fcb = config_create(pathFcb);
    t_fcb *fcb = malloc(sizeof(*fcb));

    fcb->NOMBRE_ARCHIVO = (char*) config_get_string_value(config_fcb, "NOMBRE_ARCHIVO");
    fcb->TAMANIO_ARCHIVO = (uint32_t) config_get_int_value(config_fcb, "TAMANIO_ARCHIVO");
    fcb->PUNTERO_DIRECTO = (uint32_t) config_get_int_value(config_fcb, "PUNTERO_DIRECTO");
    fcb->PUNTERO_INDIRECTO = (uint32_t) config_get_int_value(config_fcb, "PUNTERO_INDIRECTO");
    fcb->cantidad_bloques_asignados = redondear_hacia_arriba(fcb->TAMANIO_ARCHIVO, tamanioBloques);

    return fcb;
}

bool crear_archivo_nuevo_fcb(t_fcb *nuevoFcb)
{
    FILE *archivo;
    char *nombreArchivo = fcb_get_nombre_archivo(nuevoFcb);
    uint32_t tamanioArchivo = fcb_get_tamanio_archivo(nuevoFcb);
    uint32_t punteroDirecto = fcb_get_puntero_directo(nuevoFcb);
    uint32_t punteroIndirecto = fcb_get_puntero_indirecto(nuevoFcb);
    char rutaFcb[PATH_MAX];
    
    // Ruta completa del archivo
    if (snprintf(rutaFcb, sizeof(rutaFcb), "%s/%s", directorioFcbs, nombreArchivo) < 0) {
        log_error(filesystemLogger, "Error al construir la ruta del archivo del FCB %s.", nombreArchivo);
        return false;
    }

    archivo = fopen(rutaFcb,"w");
    if (archivo == NULL) {
        log_error(filesystemLogger, "Error al crear el archivo del FCB %s.", nombreArchivo);
        return false;
    }

    fprintf(archivo,"NOMBRE_ARCHIVO=%s\n",nombreArchivo);
    fprintf(archivo,"TAMANIO_ARCHIVO=%u\n",tamanioArchivo);
    fprintf(archivo,"PUNTERO_DIRECTO=%u\n",punteroDirecto);
    fprintf(archivo,"PUNTERO_INDIRECTO=%u\n",punteroIndirecto);

    fclose(archivo);

    // Si se pudo crear el archivo satisfactoriamente
    dictionary_put(listaFcbs, nombreArchivo, (void*)nuevoFcb);

    free(nombreArchivo);

    return true;
}

void recorrer_directorio_fcbs(void)
{   
    DIR *dir;
    struct dirent *ent;
    char rutaFcb[PATH_MAX];
    t_fcb* fcb_temp;
    char *nombreTemp;

    dir = opendir(directorioFcbs);
    if (dir == NULL) {
        log_info(filesystemLogger , "No se pudo abrir el directorio");
        log_info(filesystemDebuggingLogger , "No se pudo abrir el directorio");
        return;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            snprintf(rutaFcb, sizeof(rutaFcb), "%s/%s", directorioFcbs, ent->d_name);
            fcb_temp = levantar_fcb(rutaFcb);
            nombreTemp = fcb_get_nombre_archivo(fcb_temp);
            dictionary_put(listaFcbs, nombreTemp, (void*)fcb_temp);
        }
    }
    closedir(dir);
    //free(nombreTemp); // probar si funciona con el free aca
}

bool persistir_fcb(t_fcb* fcb)
{   
    char *nombreArchivo = fcb_get_nombre_archivo(fcb); // CHECKEAR
    uint32_t tamanioArchivo = fcb_get_tamanio_archivo(fcb);
    uint32_t punteroDirecto = fcb_get_puntero_directo(fcb);
    uint32_t punteroIndirecto = fcb_get_puntero_indirecto(fcb);
    char rutaFcb[PATH_MAX];
    
    // Ruta completa del archivo
    if (snprintf(rutaFcb, sizeof(rutaFcb), "%s/%s", directorioFcbs, nombreArchivo) < 0) {
        log_error(filesystemLogger, "Error al construir la ruta del archivo del FCB %s.", nombreArchivo);
        return false;
    }

    FILE* archivoFcb = fopen(rutaFcb, "w");
    if (archivoFcb == NULL) {
        log_error(filesystemLogger, "Error al abrir el archivo del FCB %s.", nombreArchivo);
        return false;
    }

    fprintf(archivoFcb,"NOMBRE_ARCHIVO=%s\n",nombreArchivo);
    fprintf(archivoFcb,"TAMANIO_ARCHIVO=%u\n",tamanioArchivo);
    fprintf(archivoFcb,"PUNTERO_DIRECTO=%u\n",punteroDirecto);
    fprintf(archivoFcb,"PUNTERO_INDIRECTO=%u\n",punteroIndirecto);

    fclose(archivoFcb);

    free(nombreArchivo); // CHECKEAR
    
    return true;
}