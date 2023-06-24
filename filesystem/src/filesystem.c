// Modulo que contiene al hilo main de la instanciacion del filesystem
#include <filesystem.h>

t_log *filesystemDebuggingLogger;
t_log *filesystemLogger;
t_filesystem_config *filesystemConfig;
t_dictionary *listaFcbs;
uint32_t tiempoRetardo;

int main(int argc, char* argv[]) 
{
    filesystemLogger = create_logger(FILESYSTEM_LOG_PATH, FILESYSTEM_MODULE_NAME, true, LOG_LEVEL_INFO);
    filesystemDebuggingLogger = create_logger(FILESYSTEM_LOG_PATH_DEBUGGING, FILESYSTEM_MODULE_NAME, false, LOG_LEVEL_DEBUG);

    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, filesystemLogger);

    // Cargar la estructura de configuracion Filesystem
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    filesystemConfig = filesystem_config_create(pathArchivoConfiguracion, filesystemDebuggingLogger);
    tiempoRetardo = filesystem_config_get_retardo_acceso_bloque(filesystemConfig) / 1000;
    free(pathArchivoConfiguracion);

    listaFcbs = dictionary_create();
    // Conexion con Memoria
    conectar_a_memoria();

    inicializar_estructuras();

    // Levantar servidor del Kernel
    const int socketEscucha = inicializar_servidor_filesystem();
    aceptar_conexion_kernel(socketEscucha);
    atender_peticiones_kernel();

    dictionary_destroy(listaFcbs);
    //filesystem_destroy(filesystemConfig, filesystemLogger, filesystemDebuggingLogger);
}
