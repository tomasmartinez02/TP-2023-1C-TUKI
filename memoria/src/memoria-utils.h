// Modulo con funciones utilitarias del modulo memoria
#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H

// Bibliotecas static utils
#include <utils/common-utils.h>
// Bibliotecas internas
#include <memoria-estructuras.h>
#include <memoria-config.h>

// Prototipos

/**
 * @brief Destruye las estructuras principales de la memoria
 * 
 * @param memoriaConfig: Estructura de configuracion de la Memoria
 * @param memoriaLogger: Logger de la Memoria
 * @param memoriaDebuggingLogger: Logger de debugging de la Memoria
 * 
 * @example memoria_destroy(memoriaConfig, memoriaLogger, memoriaDebuggingLogger); 
 */

void memoria_destroy(t_memoria_config *memoriaConfig, t_log *memoriaLogger, t_log *memoriaDebuggingLogger);

#endif