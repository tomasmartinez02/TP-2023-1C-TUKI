// Biblioteca para crear un logger chequeando errores
#ifndef LOGGING_H_
#define LOGGING_H_

//Bibliotecas estandar
#include <stdlib.h>
//Bibliotecas de las commons
#include <commons/log.h>

/**
 * @brief Crea un logger chequeando que se haya hecho correctamente
 * 
 * @param loggingPath: path al archivo del logger desde el ejecutable 
 * @param processName: nombre del proceso que crea el logger 
 * @param isActiveConsole: true -> imprime por consola, false: no imprime por consola 
 * @param level: nivel de loggeo 
 * @return t_log*: puntero a la estructura del logger 
 */
t_log* create_logger(char* loggingPath, char *processName, bool isActiveConsole, t_log_level level);

#endif