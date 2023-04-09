#include <utils/config-modulo.h>

// Funciones publicas
int config_init(void *moduleConfig, char *pathToConfig, t_log *moduleLogger,
                void (*config_initializer)(void *moduleConfig, t_config *tempConfig)) 
{
    // Inicializo el config
    t_config *tempConfig = config_create(pathToConfig);
    
    // Chequeo si el config se inicializo correctamente
    if (tempConfig == NULL) {
        log_error(moduleLogger, "Path del archivo de configuracion \"%s\" no encontrado", pathToConfig);
        return -1;
    }
    // Inicializo la estructura de configuracion y notifico lo sucedido
    config_initializer(moduleConfig, tempConfig);
    log_info(moduleLogger, "Inicialización de campos de configuracion correcta");
    
    config_destroy(tempConfig);
    return 1;
}