#include <memoria.h>

// Variables globales para definir
t_log *memoriaDebuggingLogger; 
t_log *memoriaLogger;
t_memoria_config *memoriaConfig;

int main(int argc, char* argv[]) 
{
    memoriaLogger = create_logger(MEMORIA_LOG_PATH, MEMORIA_MODULE_NAME, true, LOG_LEVEL_INFO);
    memoriaDebuggingLogger = create_logger(MEMORIA_LOG_PATH_DEBUGGING, MEMORIA_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, memoriaDebuggingLogger);

    // Cargo la estructura de configuracion de memoria
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    memoriaConfig = memoria_config_create(pathArchivoConfiguracion, memoriaDebuggingLogger);
    free(pathArchivoConfiguracion);

    //inicializo server de escucha y acepto conexiones
    const int socketEscucha = inicializar_servidor_memoria();

    aceptar_conexiones(socketEscucha);
    inicializar_memoria();

    memoria_destroy(memoriaConfig, memoriaLogger, memoriaDebuggingLogger);
}
