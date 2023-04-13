#include <memoria-utils.h>

// Funciones privadas

// Adapta la funcion para destruir el config
static void __config_destroyer(void *moduleConfig)
{
    t_memoria_config *memoriaConfigTemp = (t_memoria_config *) moduleConfig;
    memoria_config_destroy(memoriaConfigTemp);

    return;
}

// Funciones publicas
void memoria_destroy(t_memoria_config *memoriaConfig, t_log *memoriaLogger, t_log *memoriaDebuggingLogger)
{
    module_destroy((void *) memoriaConfig, __config_destroyer, memoriaLogger, memoriaDebuggingLogger); 

    return;
}

