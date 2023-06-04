#ifndef FILESYSTEM_CONFIG_H_
#define FILESYSTEM_CONFIG_H_

//Bibliotecas estandar
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//Bibliotecas commons
#include <commons/log.h>
//Bibliotecas static-utils
#include <utils/config-modulo.h>
//Bibliotecas internas modulo filesystem
//#include <filesystem-utils.h>
#include <filesystem-estructuras.h>

// Prototipos

/**
 * @brief Crea una instancia de t_filesystemconfig y la instancia segun el archivo de configuracion pasado por path
 * 
 * @param filesystemConfigPath: Ruta desde el punto de ejecucion hacia el archivo de config 
 * @param filesystemLogger: Logger de filesystem
 * @return t_filesystem_config*: instancia creada e inicalizada de la estructura de configuracion
 * 
 * @example t_filesystem_config *filesystemConfig = filesystem_config_create("./config/config-inicial.config", filesystemLogger);
 */
t_filesystem_config *filesystem_config_create(char *filesystemConfigPath, t_log *filesystemLogger);

/**
 * @brief Destruye (libera memoria) de la estructura de configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * 
 * @example filesystem_config_destroy(filesystemConfig); 
 */
void filesystem_config_destroy(t_filesystem_config *self);

/**
 * @brief Toma el valor de la ip guardada en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return char*: Ip guardada en la configuracion
 * 
 * @example char *ipMemoria = filesystem_config_get_memory_ip(filesystemConfig);
 */
char *filesystem_config_get_memory_ip(t_filesystem_config *self);

/**
 * @brief Toma el valor del puerto de memoria guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return char*: Puerto de memoria guardado en la configuracion
 * 
 * @example char *puertoMemoria = filesystem_config_get_puerto_memoria(filesystemConfig);
 */
char *filesystem_config_get_puerto_memoria(t_filesystem_config *self);

/**
 * @brief Toma el valor de la ip de escucha del servidor de Filesystem guardada en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return char*: Ip del servidor de filesystem guardado en la configuracion
 * 
 * @example char *ipEscucha = filesystem_config_get_puerto_escucha(filesystemConfig);
 */
char *filesystem_config_get_ip_escucha(t_filesystem_config *self);

/**
 * @brief Toma el valor del puerto de escucha guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return char*: Puerto de escucha guardado en la configuracion
 * 
 * @example char *puertoEscucha = filesystem_config_get_puerto_escucha(filesystemConfig);
 */
char *filesystem_config_get_puerto_escucha(t_filesystem_config *self);

/**
 * @brief Toma el valor del path del superbloque guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return char*: Path del superbloque guardado en la configuracion
 * 
 * @example char *pathSuperbloque = filesystem_config_get_path_superbloque(filesystemConfig);
 */
char *filesystem_config_get_path_superbloque(t_filesystem_config *self);

/**
 * @brief Toma el valor del path del bitmap guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return char*: Path del bitmap guardado en la configuracion
 * 
 * @example char *pathBitmap = filesystem_config_get_path_bitmap(filesystemConfig);
 */
char *filesystem_config_get_path_bitmap(t_filesystem_config *self);

/**
 * @brief Toma el valor del path de los bloques guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return char*: Path de los bloques guardado en la configuracion
 * 
 * @example char *pathBloque = filesystem_config_get_path_bloques(filesystemConfig);
 */
char *filesystem_config_get_path_bloques(t_filesystem_config *self);

/**
 * @brief Toma el valor del path del fcb guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return char*: Path del fcb guardado en la configuracion
 * 
 * @example char *pathFcb = filesystem_config_get_path_fcb(filesystemConfig);
 */
char *filesystem_config_get_path_fcb(t_filesystem_config *self);

/**
 * @brief Toma el valor del retardo de acceso al bloque guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return uint32_t: Retardo de acceso al bloque guardado en la configuracion
 * 
 * @example uint32_t retardoAccesoBloque = filesystem_config_get_path_retardo(filesystemConfig);
 */
uint32_t filesystem_config_get_retardo_acceso_bloque(t_filesystem_config *filesystemConfig);

/**
 * @brief Toma el descriptor del socket de memoria guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return int: Socket de memoria guardado en la configuracion
 * 
 * @example int socketMemoria = filesystem_config_get_socket_memoria(filesystemConfig);
 */
int filesystem_config_get_socket_memoria(t_filesystem_config *self);

/**
 * @brief Toma el descriptor del socket de kernel guardado en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @return int: Socket de kernel guardado en la configuracion
 * 
 * @example int socketKernel = filesystem_config_get_socket_kernel(filesystemConfig);
 */
int filesystem_config_get_socket_kernel(t_filesystem_config *self);

/**
 * @brief Setea el socket del modulo memoria en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @param socketMemoria: Descriptor del socket del modulo memoria
 * 
 * @example filesystem_config_set_socket_memoria(filesystemConfig, socketMemoria);
 */
void filesystem_config_set_socket_memoria(t_filesystem_config *self, int socketMemoria);

/**
 * @brief Setea el socket del modulo kernel en configuracion
 * 
 * @param self: Estructura de configuracion de filesystem
 * @param socketKernel: Descriptor del socket del modulo kernel
 * 
 * @example filesystem_config_set_socket_kernel(filesystemConfig, socketKernel);
 */
void filesystem_config_set_socket_kernel(t_filesystem_config *self, int socketKernel);

#endif