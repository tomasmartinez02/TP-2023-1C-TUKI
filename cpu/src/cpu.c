// Modulo que contiene al hilo main de cpu
#include <cpu.h>

t_log *cpuDebugginLogger; 

int main(int argc, char* argv[]) {
    
    t_log *cpuLogger = create_logger(CPU_LOG_PATH, CPU_MODULE_NAME, true, LOG_LEVEL_INFO);
    cpuDebugginLogger = create_logger(CPU_LOG_PATH_DEBUGGING, CPU_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    // Cargo la estructura de configuración de la consola
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    t_cpu_config *cpuConfig = cpu_config_create(pathArchivoConfiguracion, cpuLogger);
    free(pathArchivoConfiguracion);

    return 0;
}