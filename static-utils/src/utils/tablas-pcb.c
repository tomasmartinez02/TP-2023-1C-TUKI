#include <utils/tablas-pcb.h>


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