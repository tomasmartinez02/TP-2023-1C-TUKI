// Modulo que contiene al hilo main de la instanciacion de las consolas
#include <consola.h>

t_log *consolaDebuggingLogger;

int main(int argc, char *argv[]) 
{
    t_log *consolaLogger = create_logger(CONSOLA_LOG_PATH, CONSOLA_MODULE_NAME, true, LOG_LEVEL_INFO);
    consolaDebuggingLogger = create_logger(CONSOLA_LOG_PATH_DEBUGGING, CONSOLA_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, consolaLogger);

    // Cargo la estructura de configuracion de la consola
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    t_consola_config *consolaConfig = consola_config_create(pathArchivoConfiguracion, consolaLogger);
    free(pathArchivoConfiguracion);

    char *ipKernel = consola_config_get_kernel_ip(consolaConfig);
    char *puertoKernel = consola_config_get_kernel_puerto(consolaConfig);

    // Conexion con kernel
    const int socketKernel = conectar_a_kernel(ipKernel, puertoKernel, consolaConfig, consolaLogger, consolaDebuggingLogger);
    send_handshake_kernel(socketKernel, consolaLogger);
    receive_handshake_kernel(socketKernel, consolaConfig, consolaLogger, consolaDebuggingLogger);

    return 0;
}