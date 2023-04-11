// Biblioteca usada para instanciar y crear una estructura de configuracion para un modulo
#ifndef CONFIG_MODULO_H_
#define CONFIG_MODULO_H_

// Bibliotecas de las commons
#include <commons/config.h>
#include <commons/log.h>

// Funciones publicas

/**
 * @brief Inicializa una estructura de configuracion de un modulo particular usando la funcion config_initializer
 * 
 * @param moduleConfig: estructura de configuracion del modulo
 * @param pathToConfig: path hacia el archivo de config desde el ejecutable del modulo
 * @param moduleLogger: logger del modulo que la instancia
 * @param config_initializer: puntero a funcion que inicializa los valores del moduleConfig 
 * @return int: -1 error, 1 configuracion correcta
 * 
 * @example int configOk = config_init((void*) configModulo, "../config/config-inicial.config", loggerModulo, inicalizar_config_modulo);
 */
int config_init(void *moduleConfig, char *pathToConfig, t_log *moduleLogger,
                void (*config_initializer)(void *moduleConfig, t_config *tempConfig));

#endif