// Biblioteca utilizada para la instanciacion y la interfaz de configuracion de la consola
#ifndef MEMORIA_CONFIG_H_
#define MEMORIA_CONFIG_H_

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
#include <memoria-estructuras.h>

// Prototipos

/**
 * @brief Crea una instancia de t_memoria_config y la instancia segun el archivo de configuracion pasado por path
 * 
 * @param memoriaConfigPath: Ruta desde el punto de ejecucion hacia el archivo de config 
 * @param memoriaLogger: Logger de la memoria
 * @return t_memoria_config*: instancia creada e inicalizada de la estructura de configuracion
 * 
 * @example t_memoria_config *memoriaConfig = memoria_config_create("./config/config-inicial.h", memoriaLogger);
 */
t_memoria_config *memoria_config_create(char *memoriaConfigPath, t_log *memoriaLogger);

/**
 * @brief Destruye (libera memoria) de la estructura de configuracion
 * 
 * @param memoriaConfig: Estructura de configuracion de la memoria
 * 
 * @example memoria_config_destroy(memoriaConfig); 
 */
void memoria_config_destroy(t_memoria_config *self);

// NO SABEMOS SI LAS IPS DE ESCUCHA Y LOS PUERTOS DEBERIAN VARIAR ENTRE CADA COMPONENTE QUE SE CONECTA A MEMORIA
char *memoria_config_get_ip_escucha(t_memoria_config *self) ;

/**
 * @brief Toma el valor del puerto de escucha guardada en configuracion
 * 
 * @param memoriaConfig: Estructura de configuracion de la memoria
 * @return char*: puerto de escucha guardado en la configuracion
 * 
 * @example char *ipMemoria = memoria_config_get_puerto_escucha(memoriaConfig);
 */
char *memoria_config_get_puerto_escucha(t_memoria_config *self);

/**
 * @brief Toma el valor del tamaño de memoria guardado en configuracion
 * 
 * @param memoriaConfig: Estructura de configuracion de la memoria 
 * @return uint32_t: tamaño de memoria guardado en la configuracion
 * 
 * @example uint32_t tamanioMemoria = memoria_config_get_tamanio_memoria(memoriaConfig);
 */
uint32_t memoria_config_get_tamanio_memoria(t_memoria_config *self);

/**
 * @brief Toma el valor del tamaño de segmento guardado en configuracion
 * 
 * @param memoriaConfig: Estructura de configuracion de la memoria 
 * @return uint32_t: tamaño de segmento guardado en la configuracion
 * 
 * @example uint32_t tamanio_segmento = memoria_config_get_tamanio_segmento_cero(memoriaConfig);
 */
uint32_t memoria_config_get_tamanio_segmento_cero(t_memoria_config *self);

/**
 * @brief Toma el valor de la cantidad de segmentos guardado en configuracion
 * 
 * @param memoriaConfig: Estructura de configuracion de la memoria 
 * @return uint32_t: cantidad de segmentos guardado en la configuracion
 * 
 * @example uint32_t cantidad_segmentos = memoria_config_get_cantidad_segmentos(memoriaConfig);
 */
uint32_t memoria_config_get_cantidad_segmentos(t_memoria_config *self);

/**
 * @brief Toma el valor del retardo de memoria guardado en configuracion
 * 
 * @param memoriaConfig: Estructura de configuracion de la memoria 
 * @return uint32_t: retardo de memoria guardado en la configuracion
 * 
 * @example uint32_t retardoMemoria= memoria_config_get_retardo_memoria(memoriaConfig);
 */
uint32_t memoria_config_get_retardo_memoria(t_memoria_config *self);

/**
 * @brief Toma el valor del retardo de compactacion guardado en configuracion
 * 
 * @param memoriaConfig: Estructura de configuracion de la memoria 
 * @return uint32_t: retardo de compactacion guardado en la configuracion
 * 
 * @example uint32_t retardoCompactacion = memoria_config_get_retardo_compactacion(memoriaConfig);
 */
uint32_t memoria_config_get_retardo_compactacion(t_memoria_config *self);

/**
 * @brief Toma el valor del algoritmo de asignacion guardado en configuracion
 * 
 * @param memoriaConfig: Estructura de configuracion de la memoria
 * @return char *algoritmoAsignacion: algoritmo de asignacion guardado en la configuracion
 * 
 * @example char *algoritmoAsignacion = memoria_config_get_algoritmo_asignacion(memoriaConfig);
 */
char *memoria_config_get_algoritmo_asignacion(t_memoria_config *self);

int memoria_config_get_socket_cpu(t_memoria_config *self);
int memoria_config_get_socket_kernel(t_memoria_config *self);
int memoria_config_get_socket_filesystem(t_memoria_config *self);

void memoria_config_set_socket_cpu(t_memoria_config *self, int socketCpu);
void memoria_config_set_socket_kernel(t_memoria_config *self, int socketKernel);
void memoria_config_set_socket_filesystem(t_memoria_config *self, int socketFilesystem);
#endif