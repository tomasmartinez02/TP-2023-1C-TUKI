#include <kernel.h>

// Variables globales para definir
t_log *kernelDebuggingLogger;
t_log *kernelLogger;
t_kernel_config *kernelConfig;

int main(int argc, char* argv[]) 
{
    kernelLogger = create_logger(KERNEL_LOG_PATH, KERNEL_MODULE_NAME, true, LOG_LEVEL_INFO);
    kernelDebuggingLogger = create_logger(KERNEL_LOG_PATH_DEBUGGING, KERNEL_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, kernelDebuggingLogger);

    // Cargo la estructura de configuracion del kernel
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    kernelConfig = kernel_config_create(pathArchivoConfiguracion, kernelDebuggingLogger);
    free(pathArchivoConfiguracion);

    // Conexion con memoria
    char *ipMemoria = kernel_config_get_ip_memoria(kernelConfig);
    char *puertoMemoria = kernel_config_get_puerto_memoria(kernelConfig);
    const int socketMemoria = conectar_a_memoria(ipMemoria, puertoMemoria, kernelConfig, kernelLogger, kernelDebuggingLogger);
    kernel_config_set_socket_memoria(kernelConfig, socketMemoria);
    
    // Handshakes iniciales con memoria
    send_handshake_memoria(socketMemoria, kernelDebuggingLogger);
    receive_handshake_memoria(socketMemoria, kernelConfig, kernelLogger, kernelDebuggingLogger);

    // Conexion con cpu
    char *ipCpu = kernel_config_get_ip_cpu(kernelConfig);
    char *puertoCpu = kernel_config_get_puerto_cpu(kernelConfig);
    const int socketCpu = conectar_a_cpu(ipCpu, puertoCpu, kernelConfig, kernelLogger, kernelDebuggingLogger);
    kernel_config_set_socket_cpu(kernelConfig, socketCpu);

    // Handshakes iniciales con memoria
    send_handshake_cpu(socketCpu, kernelDebuggingLogger);
    receive_handshake_cpu(socketCpu, kernelConfig, kernelLogger, kernelDebuggingLogger);
  
    // Conexion con fileSystem
    char *ipFilesystem = kernel_config_get_ip_filesystem(kernelConfig);
    char *puertoFilesystem = kernel_config_get_puerto_filesystem(kernelConfig);
    const int socketfileSystem = conectar_a_filesystem(ipFilesystem, puertoFilesystem, kernelConfig, kernelLogger, kernelDebuggingLogger);
    kernel_config_set_socket_filesystem(kernelConfig, socketfileSystem);
    
    // Handshakes iniciales con filesystem
    send_handshake_filesystem(socketfileSystem, kernelLogger);
    receive_handshake_filesystem(socketfileSystem, kernelConfig, kernelLogger, kernelDebuggingLogger);

    return 0;
}
