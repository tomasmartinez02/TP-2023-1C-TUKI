#include <utils/tablas-pcb.h>

// Funciones privadas

// Funciones publicas

t_info_segmentos *crear_info_segmentos(void)
{
    t_info_segmentos *infoSegmentos = malloc(sizeof(*infoSegmentos));

    infoSegmentos->direccionBase = 0;
    infoSegmentos->tamanio = 0;

    return infoSegmentos;
}

void destruir_info_segmentos(t_info_segmentos *infoSegmentos)
{
    free(infoSegmentos);
}

t_buffer *empaquetar_tabla_segmentos(t_info_segmentos **tablaSegmentos)
{
    t_buffer *bufferTablaSegmentos = buffer_create();
    
    uint32_t tamanioTablaSegmentos = tamanio_tabla_segmentos(tablaSegmentos);
    buffer_pack(bufferTablaSegmentos, &tamanioTablaSegmentos, sizeof(tamanioTablaSegmentos));

    for (int i = 0; i < tamanioTablaSegmentos; i++) {
        t_info_segmentos *infoSegmento = tablaSegmentos[i];
        
        uint32_t direccionBase = infoSegmento->direccionBase;
        buffer_pack(bufferTablaSegmentos, &direccionBase, sizeof(direccionBase));

        uint32_t tamanio = infoSegmento->tamanio;
        buffer_pack(bufferTablaSegmentos, &tamanio, sizeof(tamanio));
    }

    return bufferTablaSegmentos;
}

t_info_segmentos **desempaquetar_tabla_segmentos(t_buffer *bufferTablaSegmentos, uint32_t tamanioTablaSegmentos)
{
    t_info_segmentos **tablaSegmentos = malloc(tamanioTablaSegmentos + 1);
    memset(tablaSegmentos, '\0', tamanioTablaSegmentos + 1);

    for (int i = 0; i < tamanioTablaSegmentos; i++) {
        t_info_segmentos *infoSegmento = crear_info_segmentos();

        uint32_t direccionBase;
        buffer_unpack(bufferTablaSegmentos, &direccionBase, sizeof(direccionBase));
        infoSegmento->direccionBase = direccionBase;

        uint32_t tamanio;
        buffer_unpack(bufferTablaSegmentos, &tamanio, sizeof(tamanio));
        infoSegmento->tamanio = tamanio;

        tablaSegmentos[i] = infoSegmento;
    }
    tablaSegmentos[tamanioTablaSegmentos] = NULL;

    return tablaSegmentos;
}

void destruir_tabla_segmentos(t_info_segmentos **tablaSegmentos)
{
    uint32_t tamanioTablaSegmentos = tamanio_tabla_segmentos(tablaSegmentos);

    for (int i = 0; i < tamanioTablaSegmentos; i++) {
        t_info_segmentos *infoSegmento = tablaSegmentos[i];
        destruir_info_segmentos(infoSegmento);
    }
    free(tablaSegmentos);
}

uint32_t tamanio_tabla_segmentos(t_info_segmentos **tablaSegmentos)
{
    uint32_t tamanioTablaSegmentos;
    for (int i = 0; tablaSegmentos[i] != NULL; i++) {
        tamanioTablaSegmentos++;
    }

    return tamanioTablaSegmentos;
}

// Set y Get Direccion Base info_segmentos

uint32_t info_segmentos_get_direccion_base(t_info_segmentos *infoSegmentos)
{
    return infoSegmentos->direccionBase;
}

void info_segmentos_set_direccion_base(t_info_segmentos *infoSegmentos, uint32_t direccionBase) 
{
    infoSegmentos->direccionBase = direccionBase;
}

// Set y Get tamanio info_segmentos

uint32_t info_segmentos_get_tamanio(t_info_segmentos *infoSegmentos)
{
    return infoSegmentos->tamanio;
}

void info_segmentos_set_tamanio(t_info_segmentos *infoSegmentos, uint32_t tamanio) 
{
    infoSegmentos->tamanio = tamanio;
}

t_info_archivos *crear_info_archivos(void) 
{
    t_info_archivos *infoArchivos = malloc(sizeof(*infoArchivos));
    
    infoArchivos->nombreArchivo = NULL;
    infoArchivos->posicionPuntero = 0;

    return infoArchivos;
}

void destruir_info_archivos(t_info_archivos *infoArchivos)
{
    char *nombreArchivo = infoArchivos->nombreArchivo;
    if (nombreArchivo != NULL) {
        free(nombreArchivo);
    }
    
    free(infoArchivos);
}

// Set y Get Nombre info_archivos

char *info_archivos_get_nombre_archivo(t_info_archivos *infoArchivo)
{
    char *nombreArchivo = infoArchivo->nombreArchivo;
    return nombreArchivo != NULL ? string_duplicate(nombreArchivo) : nombreArchivo;
}

void info_archivos_set_nombre_archivo(t_info_archivos *infoArchivo, char *nombreArchivo)
{
    infoArchivo->nombreArchivo = nombreArchivo != NULL ? string_duplicate(nombreArchivo) : nombreArchivo;
}

// Set y Get Posicion del Puntero info_archivos

uint32_t info_archivos_get_posicion_puntero(t_info_archivos *infoArchivo)
{
    return infoArchivo->posicionPuntero;
}

void info_archivos_set_posicion_puntero(t_info_archivos *infoArchivo, uint32_t posicionPuntero)
{
    infoArchivo->posicionPuntero = posicionPuntero;
}

void agregar_archivo_abierto(t_list *archivosAbiertos, t_info_archivos *archivoAbierto)
{
    list_add(archivosAbiertos, archivoAbierto);
    return;
}

t_info_archivos *encontrar_archivo_por_nombre(t_list *archivosAbiertos, char *nombreArchivo)
{
    bool __es_este_archivo_por_nombre(void *infoArchivo) 
    {
        t_info_archivos *archivoAbierto = (t_info_archivos *) infoArchivo;

        return string_equals_ignore_case(nombreArchivo, archivoAbierto->nombreArchivo);
    }

    return (t_info_archivos *) list_find(archivosAbiertos, __es_este_archivo_por_nombre);
}

void destruir_lista_archivos_abiertos(t_list *archivosAbiertos)
{
    void __destruir_archivo_abierto(void *infoArchivo)
    {
        t_info_archivos *archivoAbierto = (t_info_archivos *) infoArchivo;

        destruir_info_archivos(archivoAbierto);
    }

    list_destroy_and_destroy_elements(archivosAbiertos, __destruir_archivo_abierto);

    return;
}