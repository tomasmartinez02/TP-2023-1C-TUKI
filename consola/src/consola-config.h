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
 * @param self: Estructura de configuracion de la consola
 * 
 * @example consola_config_destroy(consolaConfig); 
 */
void consola_config_destroy(t_consola_config *self);

/**
 * @brief Toma el valor de la ip guardada en configuracion
 * 
 * @param self: Estructura de configuracion de la consola
 * @return char*: Ip guardada en la configuracion
 * 
 * @example char *ipKernel = consola_config_get_kernel_ip(consolaConfig);
 */
char *consola_config_get_ip_kernel(t_consola_config *self);

/**
 * @brief Toma el valor del puerto guardada en configuracion
 * 
 * @param self: Estructura de configuracion de la consola 
 * @return char*: Puerto guardado en la configuracion
 * 
 * @example char *puertoKernel = consola_config_get_kernel_puerto(consolaConfig);
 */
char *consola_config_get_puerto_kernel(t_consola_config *self);

/**
 * @brief Toma el valor del puerto guardada en configuracion
 * 
 * @param self: Estructura de configuracion de la consola 
 * @return char*: Puerto guardado en la configuracion
 * 
 * @example char *puertoKernel = consola_config_get_kernel_puerto(consolaConfig);
 */
int consola_config_get_socket_kernel(t_consola_config *self);

/**
 * @brief Toma el valor del pid de la consola
 * 
 * @param self: Estructura de configuracion de la consola  
 * @return int: Pid de la consola 
 */
int consola_config_get_pid(t_consola_config *self);

/**
 * @brief Setea el valor del socket del kernel en config
 * 
 * @param self: Estructura de configuracion de la consola
 * @param socketKernel: Descriptor del socket del kernel
 * 
 * @example consola_config_set_socket_kernel(consolaConfig, socketKernel);
 */
void consola_config_set_socket_kernel(t_consola_config *self, int socketKernel);

/**
 * @brief Setea el valor del pid de la consola en config
 * 
 * @param self: Estructura de configuracion de la consola 
 * @param pid: Pid de la consola 
 */
void consola_config_set_pid(t_consola_config *self, uint32_t pid);

#endif