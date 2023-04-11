// Modulo que contiene al hilo main de cpu
#include <cpu.h>

t_log *cpuDebugginLogger; 

int main(int argc, char* argv[]) {
    
    t_log *cpuLogger = create_logger(CPU_LOG_PATH, CPU_MODULE_NAME, true, LOG_LEVEL_INFO);
    cpuDebugginLogger = create_logger(CPU_LOG_PATH_DEBUGGING, CPU_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, cpuLogger);

    // Cargo la estructura de configuración de la consola
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    t_cpu_config *cpuConfig = cpu_config_create(pathArchivoConfiguracion, cpuLogger);
    free(pathArchivoConfiguracion);

    uint32_t retardoInstruccion = cpu_config_get_retardo_instruccion(cpuConfig);
    char *ipMemoria = cpu_config_get_ip_memoria(cpuConfig);
    uint32_t puertoEscucha = cpu_config_get_puerto_escucha(cpuConfig);
    uint32_t puertoMemoria = cpu_config_get_puerto_memoria(cpuConfig);
    uint32_t tamMaxSegmento = cpu_config_get_tam_max_segmento(cpuConfig);

    // Conexión a memoria
    const int socketMemoria = conectar_a_memoria(0, 0, cpuConfig, cpuLogger, cpuDebugginLogger);
    send_handshake_memoria(socketMemoria, cpuLogger);
    receive_handshake_memoria(socketMemoria, cpuConfig, cpuLogger, cpuDebugginLogger);

    return 0;
}