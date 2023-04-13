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

    const int socketEscuchaCpu = inicializar_servidor_memoria_para_cpu();

    const int socketEscuchaFilesystem = inicializar_servidor_memoria_para_filesystem();

    const int socketEscuchaKernel = inicializar_servidor_memoria_para_kernel();

    aceptar_conexiones_memoria_con_cpu(socketEscuchaCpu);

    aceptar_conexiones_memoria_con_filesystem(socketEscuchaFilesystem);

    aceptar_conexiones_memoria_con_kernel(socketEscuchaKernel);

}
