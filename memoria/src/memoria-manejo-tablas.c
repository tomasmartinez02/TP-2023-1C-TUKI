#include <memoria-manejo-tablas.h>

t_info_segmentos* crear_tabla_nuevo_proceso() 
{
    uint32_t cantidadSegmentos = memoria_config_get_cantidad_segmentos(memoriaConfig);
    t_info_segmentos tablaNueva[cantidadSegmentos];

    tablaNueva[0].idSegmento = 0;
    tablaNueva[0].direccionBase = segmentoCero->direccionBase;
    tablaNueva[0].tamanio = segmentoCero->tamanio;
    
    for (int i = 1; i < cantidadSegmentos; i++) {
        tablaNueva[i].idSegmento = -1; // le ponemos -1 para identificar que posiciones de la tabla estan vacias
    }

    return tablaNueva;
}