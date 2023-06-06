#include <filesystem-fcb.h>

t_fcb *crear_fcb(char *nombreArchivo)
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

