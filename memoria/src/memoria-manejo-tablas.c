#include <memoria-manejo-tablas.h>

// Funciones privadas

void __agregar_tabla_a_lista_general(t_info_segmentos** tablaNueva, uint32_t pidProceso)
{
    lista_tablas *aux = tablasDeSegmentos;
    lista_tablas *nuevoNodo = malloc(sizeof(lista_tablas));

    nuevoNodo->pidProceso = pidProceso;
    nuevoNodo->tablaSegmentos = tablaNueva;
    nuevoNodo->siguiente = NULL;

    if (aux == NULL) {
        aux = nuevoNodo;
        tablasDeSegmentos = aux;
    } else {
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevoNodo;
    }

    return;   
}

//Funciones publicas

t_info_segmentos** crear_tabla_nuevo_proceso(uint32_t pidProceso) 
{
    uint32_t cantidadSegmentos = memoria_config_get_cantidad_segmentos(memoriaConfig);
    t_info_segmentos** tablaNueva = malloc(cantidadSegmentos * sizeof(t_info_segmentos));
    
    for (int i = 0; i < cantidadSegmentos; i++) {
    tablaNueva[i] = malloc(sizeof(t_info_segmentos));
    }

    tablaNueva[0]->idSegmento = 0;
    tablaNueva[0]->direccionBase = segmentoCero->direccionBase;
    tablaNueva[0]->tamanio = segmentoCero->tamanio;
    
    for (int i = 1; i < cantidadSegmentos; i++) {
        tablaNueva[i]->idSegmento = -1; // le ponemos -1 para identificar que posiciones de la tabla estan vacias
        tablaNueva[i]->direccionBase = 0;
        tablaNueva[i]->tamanio = 0;
    }

    __agregar_tabla_a_lista_general(tablaNueva, pidProceso);

    return tablaNueva;
}
