// Modulo que contiene al hilo main de cpu
#include <cpu.h>

t_log *cpuDebugginLogger; 
t_log *cpuLogger;
t_cpu_config *cpuConfig;

int main(int argc, char* argv[]) {
    
    cpuLogger = create_logger(CPU_LOG_PATH, CPU_MODULE_NAME, true, LOG_LEVEL_INFO);
    cpuDebugginLogger = create_logger(CPU_LOG_PATH_DEBUGGING, CPU_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, cpuLogger);

    // Cargo la estructura de configuración de la consola
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    cpuConfig = cpu_config_create(pathArchivoConfiguracion, cpuLogger);
    free(pathArchivoConfiguracion);

    uint32_t retardoInstruccion = cpu_config_get_retardo_instruccion(cpuConfig);
    char *ipMemoria = cpu_config_get_ip_memoria(cpuConfig);
    char* puertoEscucha = cpu_config_get_puerto_escucha(cpuConfig);
    char* puertoMemoria = cpu_config_get_puerto_memoria(cpuConfig);
    uint32_t tamMaxSegmento = cpu_config_get_tam_max_segmento(cpuConfig);

    // Conexión a memoria
    conectar_a_memoria();

    // Levantar servidor del Kernel
    const int socketEscucha = inicializar_servidor_cpu();
    aceptar_conexion_kernel(socketEscucha);

    return 0;
}