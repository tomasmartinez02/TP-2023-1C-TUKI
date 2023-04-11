#include <kernel.h>

int main(int argc, char* argv[]) {
    t_log *kernelLogger = create_logger(KERNEL_LOG_PATH, KERNEL_MODULE_NAME, true, LOG_LEVEL_INFO);
    kernelDebuggingLogger = create_logger(KERNEL_LOG_PATH_DEBUGGING, KERNEL_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, kernelLogger);

    // Cargo la estructura de configuracion del kernel
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    t_kernel_config *kernelConfig = kernel_config_create(pathArchivoConfiguracion, kernelLogger);
    free(pathArchivoConfiguracion);

    char *ipMemoria = kernel_config_get_memoria_ip(consolaConfig);
    char *puertoMemoria = kernel_config_get_memoria_puerto(consolaConfig);
    char *ipFileSystem = kernel_config_get_fileSystem_ip(consolaConfig);
    char *puertoFileSystem = kernel_config_get_fileSystem_puerto(consolaConfig);
    char *ipCpu = kernel_config_get_CPU_ip(consolaConfig);
    char *puertoCPU = kernel_config_get_CPU_puerto(consolaConfig);
    char *puertoEscucha = kernel_config_get_Escucha_puerto(consolaConfig);
    char *algoritmoPlanificacion = kernel_config_get_algoritmo_planificacion(consolaConfig);
    char *estimacionInicial = kernel_config_get_estimacion_inicial(consolaConfig);
    char *hrrnAlfa = kernel_config_get_valor_alfa(consolaConfig);
    char *gradoMultiprogramacion = kernel_config_get_grado_multiprogramacion(consolaConfig);
    char *recursos = kernel_config_get_recursos(consolaConfig);
    char *instanciasRecursos = kernel_config_get_instancias_recursos(consolaConfig);

    // Conexion con memoria
    const int socketMemoria = conectar_a_memoria(ipMemoria, puertoMemoria, kernelConfig, kernelLogger, kernelDebuggingLogger);
    send_handshake_memoria(socketMemoria, kernelLogger);
    receive_handshake_memoria(socketMemoria, kernelConfig, kernelLogger, kernelDebuggingLogger);

    // Conexion con cpu
    const int socketCpu= conectar_a_cpu(ipCpu, puertoCPU, kernelConfig, kernelLogger, kernelDebuggingLogger);
    send_handshake_memoria(socketCpu, kernelLogger);
    receive_handshake_memoria(socketCpu, kernelConfig, kernelLogger, kernelDebuggingLogger);
  
    // Conexion con fileSystem
    const int socketfileSystem= conectar_a_fileSystem(ipFileSystem, puertoFileSystem, kernelConfig, kernelLogger, kernelDebuggingLogger);
    send_handshake_fileSystem(socketfileSystem, kernelLogger);
    receive_handshake_fileSystem(socketfileSystem, kernelConfig, kernelLogger, kernelDebuggingLogger);

    return 0;
}
