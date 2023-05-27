#include <memoria-config.h>

// Variables estaticas
void* memoriaPrincipal; // por el momento va aca, despues vemos donde va

// Funciones privadas

// Funciones inicializacion estructuras de la memoria

static void __inicializar_memoria_principal(void){
    
    uint32_t tamanioMemoria;
    tamanioMemoria = memoria_config_get_tamanio_memoria(memoriaConfig);

    memoriaPrincipal = malloc(tamanioMemoria); // Esto nunca se libera no?

    log_info(memoriaDebuggingLogger, "Se inicializa a la memoria principal con un tamanio de %d bytes", tamanioMemoria);

    return;

}

// Funciones publicas

void inicializar_memoria(void){
    
    __inicializar_memoria_principal();

    return;

}