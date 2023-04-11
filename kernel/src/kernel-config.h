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
#include <kernel-estructuras.h>

/**
 * @brief Crea una instancia de t_kernel_config y la instancia segun el archivo de configuracion pasado por path
 * 
 * @param kernelConfigPath: Ruta desde el punto de ejecucion hacia el archivo de config 
 * @param kernelLogger: Logger de la consola
 * @return t_kernel_config*: instancia creada e inicalizada de la estructura de configuracion
 * 
 * @example t_kernel_config *consolaConfig = consola_config_create("./config/config-inicial.h", consolaLogger);
 */
t_kernel_config *kernel_config_create(char *kernelConfigPath, t_log *kernelLogger);

/**
 * @brief Destruye (libera memoria) de la estructura de configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * 
 * @example kernel_config_destroy(kernelConfig); 
 */
void kernel_config_destroy(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor de la ip guardada en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion de la kernel
 * @return char*: Ip guardada en la configuracion
 * 
 * @example char *ipMemoria = kernel_config_get_memoria_ip(kernelConfig);
 */
char *kernel_config_get_memoria_ip(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor del puerto guardada en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Puerto guardado en la configuracion
 * 
 * @example char *puertoMemoria = kernel_config_get_memoria_puerto(kernelConfig);
 */
char *kernel_config_get_memoria_puerto(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor de la ip guardada en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion de la kernel
 * @return char*: Ip guardada en la configuracion
 * 
 * @example char *ipFileSystem = kernel_config_get_fileSystem_ip(kernelConfig);
 */
char *kernel_config_get_fileSystem_ip(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor del puerto guardada en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Puerto guardado en la configuracion
 * 
 * @example char *puertoFileSystem = kernel_config_get_FileSystem_puerto(kernelConfig);
 */
char *kernel_config_get_fileSystem_puerto(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor de la ip guardada en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion de la kernel
 * @return char*: Ip guardada en la configuracion
 * 
 * @example char *ipCPU = kernel_config_get_CPU_ip(kernelConfig);
 */
char *kernel_config_get_CPU_ip(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor del puerto guardada en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Puerto guardado en la configuracion
 * 
 * @example char *puertoCPU = kernel_config_get_CPU_puerto(kernelConfig);
 */
char *kernel_config_get_CPU_puerto(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor del puerto guardada en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Puerto guardado en la configuracion
 * 
 * @example char *puertoEscucha = kernel_config_get_Escucha_puerto(kernelConfig);
 */
char *kernel_config_get_Escucha_puerto(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor del algoritmo de planificacion guardado en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Algoritmo de planificacion guardado en la configuracion
 * 
 * @example char *algoritmoPlanificacion = kernel_config_get_algoritmo_planificacion(kernelConfig);
 */
char *kernel_config_get_algoritmo_planificacion(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor de la estimacion inicial guardado en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Estimacion inicial guardada en la configuracion
 * 
 * @example char *estimacionInicial = kernel_config_get_estimacion_inicial(kernelConfig);
 */
int kernel_config_get_estimacion_inicial(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor del alfa para el HRRN guardado en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Valor del alfa guardado en la configuracion
 * 
 * @example char *valorAlfa = kernel_config_get_valor_alfa(kernelConfig);
 */
int kernel_config_get_valor_alfa(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor del grado maximo de multiprogramacion guardado en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Grado maximo de multiprogramacion guardado en la configuracion
 * 
 * @example char *gradoMultiprogramacion = kernel_config_get_grado_multiprogramacion(kernelConfig);
 */
int kernel_config_get_grado_multiprogramacion(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor de los recursos guardado en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Recursos guardados en la configuracion
 * 
 * @example char *recursos = kernel_config_get_recursos(kernelConfig);
 */
char **kernel_config_get_recursos(t_kernel_config *kernelConfig);

/**
 * @brief Toma el valor de las instancias de los recursos guardado en configuracion
 * 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @return char*: Recursos guardados en la configuracion
 * 
 * @example char *instanciasRecursos = kernel_config_get_instancias_recursos(kernelConfig);
 */
char **kernel_config_get_instancias_recursos(t_kernel_config *kernelConfig);

#endif