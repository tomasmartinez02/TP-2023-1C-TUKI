#include <filesystem-utils.h>

// Funciones privadas

// Adapta la funcion para destruir el config
static void __config_destroyer(void *moduleConfig)
{
    t_filesystem_config *filesystemConfigTemp = (t_filesystem_config *) moduleConfig;
    filesystem_config_destroy(filesystemConfigTemp);

    return;
}

// Funciones publicas
void filesystem_destroy(t_filesystem_config *filesystemConfig, t_log *filesystemLogger, t_log *filesystemDebuggingLogger)
{
    module_destroy((void *) filesystemConfig, __config_destroyer, filesystemLogger, filesystemDebuggingLogger);

    return;
}