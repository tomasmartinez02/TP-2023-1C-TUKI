#include <consola-utils.h>

// Funciones privadas

// Adapta la funcion para destruir el config
static void __config_destroyer(void *moduleConfig)
{
    t_consola_config *consolaConfigTemp = (t_consola_config *) moduleConfig;
    consola_config_destroy(consolaConfigTemp);

    return;
}


// Funciones publicas
void consola_destroy(t_consola_config *consolaConfig, t_log *consolaLogger, t_log *consolaDebuggingLogger)
{
    module_destroy((void *) consolaConfig, __config_destroyer, consolaLogger, consolaDebuggingLogger);

    return;
}
