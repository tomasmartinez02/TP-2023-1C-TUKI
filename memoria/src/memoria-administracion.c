#include <memoria-config.h>

// Variables estaticas
void* memoriaPrincipal; 
t_info_segmentos* tablaSegmentos;

// Funciones privadas

// Funciones inicializacion estructuras de la memoria

static void __inicializar_memoria_principal(void)
{
    uint32_t tamanioMemoria;
    tamanioMemoria = memoria_config_get_tamanio_memoria(memoriaConfig);

    memoriaPrincipal = malloc(tamanioMemoria); // Esto nunca se libera no?

    log_info(memoriaDebuggingLogger, "Se inicializa a la memoria principal con un tamanio de %d bytes", tamanioMemoria);

    return;
}

void __crear_segmentos_cero(void)
{
    uint32_t tamanioSegmentoCero = memoria_config_get_tamanio_segmento_cero(memoriaConfig);

    segmentoCero = crear_info_segmentos(0,0, tamanioSegmentoCero); // despues ver esto

    actualizar_lista_huecos_libres(segmentoCero);

    return;

}

void __crear_estructura_espacios_libres (void) 
{   

    listaHuecosLibres->hueco->direccionBase = 0;
    listaHuecosLibres->hueco->tamanio = memoria_config_get_tamanio_memoria(memoriaConfig); 

}

void actualizar_lista_huecos_libres(t_info_segmentos segmento)
{
    t_huecos_libres *auxiliarLista = listaHuecosLibres;

    while (auxiliarLista->hueco->direccionBase < segmento->direccionBase) {
        auxiliarLista = auxiliarLista->hueco->siguiente;
    }
}

// Funciones publicas

void inicializar_memoria(void){

    __crear_estructura_espacios_libres();
    __crear_segmentos_cero();


    __inicializar_memoria_principal();

    return;

}