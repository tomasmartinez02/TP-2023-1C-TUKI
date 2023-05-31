// Modulo con funciones utilitarias del modulo consola
#ifndef CONSOLA_UTILS_H_
#define CONSOLA_UTILS_H_

// Bibliotecas static utils
#include <utils/common-utils.h>
// Bibliotecas internas
#include <consola-estructuras.h>
#include <consola-config.h>

// Prototipos

/**
 * @brief Destruye las estructuras principales de la consola
 * 
 * @param consolaConfig: Estructura de configuracion de la consola 
 * @param consolaLogger: Logger de la consola
 * @param consolaDebuggingLogger: Logger de debugging de la consola
 * 
 * @example consola_destroy(consolaConfig, consolaLogger, consolaDebuggingLogger) 
 */
void consola_destroy(t_consola_config *consolaConfig, t_log *consolaLogger, t_log *consolaDebuggingLogger);

#endif