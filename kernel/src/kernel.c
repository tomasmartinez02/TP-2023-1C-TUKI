#include <kernel.h>

int main(int argc, char* argv[]) {
    t_log *kernelLogger = create_logger(KERNEL_LOG_PATH, KERNEL_MODULE_NAME, true, LOG_LEVEL_INFO);
    kernelDebuggingLogger = create_logger(KERNEL_LOG_PATH_DEBUGGING, KERNEL_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, kernelLogger);

    // Cargo la estructura de configuracion del kernel
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    t_kernel_config *kernelConfig = kernel_config_create(pathArchivoConfiguracion, kernelLogger);
    free(pathArchivoConfiguracion);

    char *ipMemoria = kernel_config_get_memoria_ip(kernelConfig);
    char *puertoMemoria = kernel_config_get_memoria_puerto(kernelConfig);
    char *ipFileSystem = kernel_config_get_fileSystem_ip(kernelConfig);
    char *puertoFileSystem = kernel_config_get_fileSystem_puerto(kernelConfig);
    char *ipCpu = kernel_config_get_CPU_ip(kernelConfig);
    char *puertoCPU = kernel_config_get_CPU_puerto(kernelConfig);
    char *puertoEscucha = kernel_config_get_Escucha_puerto(kernelConfig);
    char *algoritmoPlanificacion = kernel_config_get_algoritmo_planificacion(kernelConfig);
    int estimacionInicial = kernel_config_get_estimacion_inicial(kernelConfig);
    int hrrnAlfa = kernel_config_get_valor_alfa(kernelConfig);
    int gradoMultiprogramacion = kernel_config_get_grado_multiprogramacion(kernelConfig);
    char **recursos = kernel_config_get_recursos(kernelConfig);
    char **instanciasRecursos = kernel_config_get_instancias_recursos(kernelConfig);

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
