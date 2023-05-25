// Modulo que contiene al hilo main de cpu
#include <cpu.h>

t_log *cpuDebuggingLogger; 
t_log *cpuLogger;
t_cpu_config *cpuConfig;

int main(int argc, char* argv[]) {
    
    cpuLogger = create_logger(CPU_LOG_PATH, CPU_MODULE_NAME, true, LOG_LEVEL_INFO);
    cpuDebuggingLogger = create_logger(CPU_LOG_PATH_DEBUGGING, CPU_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, cpuDebuggingLogger);

    // Cargo la estructura de configuración de la consola
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    cpuConfig = cpu_config_create(pathArchivoConfiguracion, cpuDebuggingLogger);
    free(pathArchivoConfiguracion);

    // Conexión a memoria
    conectar_a_memoria();

    // Levantar servidor del Kernel
    const int socketEscucha = inicializar_servidor_cpu();
    aceptar_conexion_kernel(socketEscucha);

    atender_peticiones_kernel();

    cpu_destroy(cpuConfig, cpuLogger, cpuDebuggingLogger);

    return 0;
}