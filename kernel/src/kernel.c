#include <kernel.h>

// Variables globales para definir
t_log *kernelDebuggingLogger;
t_log *kernelLogger;
t_kernel_config *kernelConfig;
t_dictionary *tablaArchivosAbiertos;

int main(int argc, char* argv[]) 
{
    kernelLogger = create_logger(KERNEL_LOG_PATH, KERNEL_MODULE_NAME, true, LOG_LEVEL_INFO);
    kernelDebuggingLogger = create_logger(KERNEL_LOG_PATH_DEBUGGING, KERNEL_MODULE_NAME, false, LOG_LEVEL_DEBUG);
    
    check_arguments(argc, NUMBER_OF_ARGS_REQUIRED, ARGUMENTS_ERROR_MESSAGE, kernelDebuggingLogger);

    // Cargo la estructura de configuracion del kernel
    char *pathArchivoConfiguracion = string_duplicate(argv[1]);
    kernelConfig = kernel_config_create(pathArchivoConfiguracion, kernelDebuggingLogger);
    free(pathArchivoConfiguracion);

    // Conexion con Memoria
    conectar_a_memoria();
    
    // Conexion con Cpu
    conectar_a_cpu();
  
    // Conexion con Fileystem
    conectar_a_filesystem();

    // Levantar servidor de instancias Consola
    const int socketEscucha = inicializar_servidor_kernel();
    
    inicializar_estructuras();
    tablaArchivosAbiertos = crear_diccionario_semaforos_archivos();

    aceptar_conexiones_kernel(socketEscucha);

    return 0;
}
