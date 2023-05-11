// Biblioteca utilizada para la instanciacion y la interfaz de configuracion del kernel
#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

//Bibliotecas estandar
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//Bibliotecas commons
#include <commons/log.h>
#include <commons/string.h>
//Bibliotecas static-utils
#include <utils/config-modulo.h>
//Bibliotecas internas modulo kernel
//#include <kernel-utils.h>
#include <kernel-estructuras.h>
#include <kernel-semaforos-recursos.h>

/**
 * @brief Crea una instancia de t_kernel_config y la instancia segun el archivo de configuracion pasado por path
 * 
 * @param kernelConfigPath: Ruta desde el punto de ejecucion hacia el archivo de config 
 * @param kernelLogger: Logger del kernel
 *
 * @return t_kernel_config*: instancia creada e inicalizada de la estructura de configuracion
 * 
 * @example t_kernel_config *kernelConfig = kernel_config_create("./config/config-inicial.h", kernelLogger);
 */
t_kernel_config *kernel_config_create(char *kernelConfigPath, t_log *kernelLogger);

/**
 * @brief Destruye (libera memoria) de la estructura de configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * 
 * @example kernel_config_destroy(kernelConfig); 
 */
void kernel_config_destroy(t_kernel_config *self);

/**
 * @brief Toma el valor de la ip de memoria guardada en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Ip de la memoria guardada en la configuracion
 * 
 * @example char *ipMemoria = kernel_config_get_ip_memoria(kernelConfig);
 */
char *kernel_config_get_ip_memoria(t_kernel_config *self);

/**
 * @brief Toma el valor del puerto de memoria guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Puerto de la memoria guardado en la configuracion
 * 
 * @example char *puertoMemoria = kernel_config_get_puerto_memoria(kernelConfig);
 */
char *kernel_config_get_puerto_memoria(t_kernel_config *self);

/**
 * @brief Toma el valor de la ip del filesystem guardada en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Ip guardada en la configuracion
 * 
 * @example char *ipFileSystem = kernel_config_get_ip_filesystem(kernelConfig);
 */
char *kernel_config_get_ip_filesystem(t_kernel_config *self);

/**
 * @brief Toma el valor del puerto de filesystem guardada en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Puerto del filesystem guardado en la configuracion
 * 
 * @example char *puertoFileSystem = kernel_config_get_puerto_filesystem(kernelConfig);
 */
char *kernel_config_get_puerto_filesystem(t_kernel_config *self);

/**
 * @brief Toma el valor de la ip del cpu guardada en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Ip del cpu guardada en la configuracion
 * 
 * @example char *ipCpu = kernel_config_get_ip_cpu(kernelConfig);
 */
char *kernel_config_get_ip_cpu(t_kernel_config *self);

/**
 * @brief Toma el valor del puerto del cpu guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Puerto del cpu guardado en la configuracion
 * 
 * @example char *puertoCpu = kernel_config_get_puerto_cpu(kernelConfig);
 */
char *kernel_config_get_puerto_cpu(t_kernel_config *self);

/**
 * @brief Toma el valor del puerto de escucha del servidor del kernel guardada en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Puerto del servidor del kernel guardado en la configuracion
 * 
 * @example char *puertoEscucha = kernel_config_get_puerto_escucha(kernelConfig);
 */
char *kernel_config_get_puerto_escucha(t_kernel_config *self);

/**
 * @brief Toma el valor de la ip de escucha del servidor del kernel guardada en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Ip del servidor del kernel guardado en la configuracion
 * 
 * @example char *ipEscucha = kernel_config_get_puerto_escucha(kernelConfig);
 */
char *kernel_config_get_ip_escucha(t_kernel_config *self);

/**
 * @brief Toma el valor del algoritmo de planificacion guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Algoritmo de planificacion guardado en la configuracion
 * 
 * @example char *algoritmoPlanificacion = kernel_config_get_algoritmo_planificacion(kernelConfig);
 */
char *kernel_config_get_algoritmo_planificacion(t_kernel_config *self);

/**
 * @brief Toma el valor de la estimacion inicial guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return double: Estimacion inicial guardada en la configuracion
 * 
 * @example double estimacionInicial = kernel_config_get_estimacion_inicial(kernelConfig);
 */
double kernel_config_get_estimacion_inicial(t_kernel_config *self);

/**
 * @brief Toma el valor del alfa para el HRRN guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Valor del alfa guardado en la configuracion
 * 
 * @example char *hrrnAlfa = kernel_config_get_hrrn_alfa(kernelConfig);
 */
double kernel_config_get_hrrn_alfa(t_kernel_config *self);

/**
 * @brief Toma el valor del grado maximo de multiprogramacion guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return char*: Grado maximo de multiprogramacion guardado en la configuracion
 * 
 * @example char *gradoMaxMultiprogramacion = kernel_config_get_grado_max_multiprogramacion(kernelConfig);
 */
uint32_t kernel_config_get_grado_max_multiprogramacion(t_kernel_config *self);

/**
 * @brief Toma el descriptor del socket de memoria guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return int: Descriptor del socket de memoria guardado en la configuracion
 * 
 * @example int socketMemoria = kernel_config_get_socket_memoria(kernelConfig);
 */
int kernel_config_get_socket_memoria(t_kernel_config *self);

/**
 * @brief Toma el descriptor del socket de cpu guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return int: Descriptor del socket de cpu guardado en la configuracion
 * 
 * @example int socketCpu = kernel_config_get_socket_cpu(kernelConfig);
 */
int kernel_config_get_socket_cpu(t_kernel_config *self);

/**
 * @brief Toma el descriptor del socket de filesystem guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @return int: Descriptor del socket de filesystem guardado en la configuracion
 * 
 * @example int socketFilesystem = kernel_config_get_socket_filesystem(kernelConfig);
 */
int kernel_config_get_socket_filesystem(t_kernel_config *self);

/**
 * @brief Toma el descriptor del socket de memoria guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @param socketMemoria: Descriptor del socket de la memoria a setear
 * 
 * @example kernel_config_set_socket_memoria(kernelConfig, socketMemoria);
 */
void kernel_config_set_socket_memoria(t_kernel_config *self, int socketMemoria);

/**
 * @brief Toma el descriptor del socket de cpu guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @param socketCpu: Descriptor del socket del cpu a setear
 * 
 * @example kernel_config_set_socket_cpu(kernelConfig, socketCpu);
 */
void kernel_config_set_socket_cpu(t_kernel_config *self, int socketCpu);

/**
 * @brief Toma el descriptor del socket de filesystem guardado en configuracion
 * 
 * @param self: Estructura de configuracion del kernel
 * @param socketFilesystem: Descriptor del socket del filesystem a setear
 * 
 * @example kernel_config_set_socket_filesystem(kernelConfig, socketFilesystem);
 */
void kernel_config_set_socket_filesystem(t_kernel_config *self, int socketFilesystem);

t_dictionary *kernel_config_get_diccionario_semaforos(t_kernel_config *self);

#endif