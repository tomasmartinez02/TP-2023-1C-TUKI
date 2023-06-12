#include <filesystem-fcb.h>

// ESTRUCTURA DE FCBs
t_fcb *crear_nuevo_fcb(char *nombreArchivo)
{
    t_fcb *fcb = malloc(sizeof(*fcb));
    fcb->NOMBRE_ARCHIVO = strdup(nombreArchivo);
    fcb->TAMANIO_ARCHIVO = 0;
    fcb->PUNTERO_DIRECTO = 0;
    fcb->PUNTERO_INDIRECTO = 0;

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
    fcb->PUNTERO_DIRECTO = nuevoPunteroIndirecto;
    return;
}

// ARCHIVOS DE FCBS
static void __inicializar_fcb(void *module, t_config *tempFcb)
{
    fcb = (t_fcb *) module;

    fcb->NOMBRE_ARCHIVO = (char*) config_get_string_value(tempFcb, "NOMBRE_ARCHIVO");
    fcb->TAMANIO_ARCHIVO = (uint32_t) config_get_int_value(tempFcb, "TAMANIO_ARCHIVO");
    fcb->PUNTERO_DIRECTO = (uint32_t) config_get_int_value(tempFcb, "PUNTERO_DIRECTO");
    fcb->PUNTERO_INDIRECTO = (uint32_t) config_get_int_value(tempFcb, "PUNTERO_INDIRECTO");

    return;
}

t_fcb* levantar_fcb(char *pathFcb)
{
    t_fcb *fcb = malloc(sizeof(*fcb));

    // Checkear que se haya inicializado bien
    int32_t inicializacionCorrecta = config_init(fcb, pathFcb, filesystemLogger, __inicializar_fcb);
    if (inicializacionCorrecta == -1) {
        exit(EXIT_FAILURE);
    }

    return fcb;
}

bool crear_archivo_nuevo_fcb(t_fcb *nuevoFcb)
{
    FILE *archivo;

    archivo = fopen(nuevoFcb->NOMBRE_ARCHIVO,"w");
    if (archivo == NULL) {
        //error
        return false;
    }

    fprintf(archivo,"NOMBRE_ARCHIVO=%s\n",nuevoFcb->NOMBRE_ARCHIVO);
    fprintf(archivo,"TAMANIO_ARCHIVO=%d\n",nuevoFcb->TAMANIO_ARCHIVO);
    fprintf(archivo,"PUNTERO_DIRECTO=%d\n",nuevoFcb->PUNTERO_DIRECTO);
    fprintf(archivo,"PUNTERO_INDIRECTO=%d\n",nuevoFcb->PUNTERO_INDIRECTO);

    fclose(archivo);
    // Si se pudo crear el archivo satisfactoriamente
    return true;
}


void recorrer_directorio_fcbs(char *directorioFcbs)
{   
    DIR *dir;
    struct dirent *ent;
    char pathFcb[PATH_MAX];
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
            snprintf(pathFcb, sizeof(pathFcb), "%s/%s", directorioFcbs, ent->d_name);
            fcb_temp = levantar_fcb(pathFcb);
            nombreTemp = fcb_get_nombre_archivo(fcb_temp);
            dictionary_put(listaFcbs, nombreTemp, (void*)fcb_temp);
        }
    }
    closedir(dir);
}
