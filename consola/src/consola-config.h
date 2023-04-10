// Biblioteca utilizada para la instanciacion y la interfaz de configuracion de la consola
#ifndef CONSOLA_CONFIG_H_
#define CONSOLA_CONFIG_H_

//Bibliotecas estandar
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//Bibliotecas commons
#include <commons/log.h>
//Bibliotecas static-utils
#include <utils/config-modulo.h>
//Bibliotecas internas modulo consola
//#include <consola-utils.h>
#include <consola-estructuras.h>

// Prototipos

/**
 * @brief Crea una instancia de t_consola_config y la instancia segun el archivo de configuracion pasado por path
 * 
 * @param consolaConfigPath: Ruta desde el punto de ejecucion hacia el archivo de config 
 * @param consolaLogger: Logger de la consola
 * @return t_consola_config*: instancia creada e inicalizada de la estructura de configuracion
 * 
 * @example t_consola_config *consolaConfig = consola_config_create("./config/config-inicial.h", consolaLogger);
 */
t_consola_config *consola_config_create(char *consolaConfigPath, t_log *consolaLogger);

/**
 * @brief Destruye (libera memoria) de la estructura de configuracion
 * 
 * @param consolaConfig: Estructura de configuracion de la consola
 * 
 * @example consola_config_destroy(consolaConfig); 
 */
void consola_config_destroy(t_consola_config *consolaConfig);

/**
 * @brief Toma el valor de la ip guardada en configuracion
 * 
 * @param consolaConfig: Estructura de configuracion de la consola
 * @return char*: Ip guardada en la configuracion
 * 
 * @example char *ipKernel = consola_config_get_kernel_ip(consolaConfig);
 */
char *consola_config_get_kernel_ip(t_consola_config *consolaConfig);

/**
 * @brief Toma el valor del puerto guardada en configuracion
 * 
 * @param consolaConfig: Estructura de configuracion de la consola 
 * @return char*: Puerto guardado en la configuracion
 * 
 * @example char *puertoKernel = consola_config_get_kernel_puerto(consolaConfig);
 */
char *consola_config_get_kernel_puerto(t_consola_config *consolaConfig);

#endif