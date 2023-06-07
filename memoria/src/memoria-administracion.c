#include <memoria-administracion.h>

// Variables estaticas
void* memoriaPrincipal; 
t_info_segmentos* tablaSegmentos;

// Variables globales
t_huecos_libres *listaHuecosLibres; 
t_info_segmentos *segmentoCero;

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

static t_huecos_libres* __crear_hueco (uint32_t direccionBase, uint32_t tamanio) // lo crea pero no lo agrega a la lista
{   
    t_huecos_libres* nuevoHueco;
    nuevoHueco->hueco->direccionBase = direccionBase;
    nuevoHueco->hueco->tamanio = tamanio; 

    return nuevoHueco;
}

static void __eliminar_hueco (t_huecos_libres *huecoAEliminar) 
{
    t_huecos_libres *auxiliarLista = listaHuecosLibres;

    while(auxiliarLista->hueco->direccionBase < huecoAEliminar->hueco->direccionBase) {
        auxiliarLista = auxiliarLista->siguiente;
    } // avanza hasta que encuentra el nodo anterior al que quiere eliminar

    auxiliarLista->siguiente = huecoAEliminar->siguiente;

    free(huecoAEliminar);

    return ;
}

static void __partir_hueco (t_huecos_libres *auxiliarLista, t_info_segmentos *segmento) 
{
    auxiliarLista->hueco->direccionBase = segmento->direccionBase + segmento->tamanio;
    auxiliarLista->hueco->tamanio = auxiliarLista->hueco->tamanio - segmento->tamanio;

    return ;
}

void actualizar_lista_huecos_libres (t_info_segmentos *segmento) // el nombre de esta funcion habria que cambiarlo
{
    t_huecos_libres *auxiliarLista = listaHuecosLibres;
    uint32_t posicionSegmento = segmento->direccionBase;
    uint32_t tamanioSegmento = segmento->tamanio;
    uint32_t direccionBaseHueco = auxiliarLista->hueco->direccionBase;
    uint32_t tamanioHueco = auxiliarLista->hueco->tamanio;

    while(direccionBaseHueco == posicionSegmento) {
        auxiliarLista = auxiliarLista->siguiente;
        direccionBaseHueco = auxiliarLista->hueco->direccionBase;
        tamanioHueco = auxiliarLista->hueco->tamanio;
    } // aca busca que hueco habria que modificar 

    if (tamanioHueco == tamanioSegmento){
        __eliminar_hueco(auxiliarLista); // ver bien despues esta funcion
    } else {
        __partir_hueco(auxiliarLista, segmento); 
    }
    
    return ;
}

void __crear_estructura_espacios_libres (void) 
{   
    listaHuecosLibres = __crear_hueco(0, memoria_config_get_tamanio_memoria(memoriaConfig));
    return ;
}

void __crear_segmentos_cero(void)
{
    uint32_t tamanioSegmentoCero = memoria_config_get_tamanio_segmento_cero(memoriaConfig);
    segmentoCero = crear_info_segmentos(0,0, tamanioSegmentoCero); // despues ver esto
    actualizar_lista_huecos_libres(segmentoCero);

    return;

}

// Funciones publicas

void inicializar_memoria(void){

    __crear_estructura_espacios_libres();
    __crear_segmentos_cero();
    __inicializar_memoria_principal();

    return;

}