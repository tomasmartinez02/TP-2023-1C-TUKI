#include <utils/logging.h>

// Funciones publicas
t_log *create_logger(char *loggingPath, char *processName, bool isActiveConsole, t_log_level level)
{
    t_log *tempLogger;

    tempLogger = log_create(loggingPath, processName, isActiveConsole, level);

    //Chequeo si la instanciacion del logger es correcta
    if(tempLogger == NULL){
        fprintf(stderr, "No se ha podido abrir el archivo de logging. Path \"%s\" no encontrado.", loggingPath);
        exit(EXIT_FAILURE);
    }

    return tempLogger;
}