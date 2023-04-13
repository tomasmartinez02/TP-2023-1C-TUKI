// Modulo que contiene al hilo main de la instanciacion de las consolas
#include <consola.h>

// Variables globales definidas en main
t_log *consolaLogger;
t_log *consolaDebuggingLogger;
t_consola_config *consolaConfig;

int main(int argc, char *argv[]) 
{
    consolaLogger = create_logger(CONSOLA_LOG_PATH, CONSOLA_MODULE_NAME, true, LOG_LEVEL_INFO);
    consolaDebuggingLogger = create_logger(CONSOLA_LOG_PATH_DEBUGGING, CONSOLA_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, consolaLogger);

    // Cargo la estructura de configuracion de la consola
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    consolaConfig = consola_config_create(pathArchivoConfiguracion, consolaDebuggingLogger);
    free(pathArchivoConfiguracion);

    // Conexion con kernel
    conectar_a_kernel();
    
    return 0;
}