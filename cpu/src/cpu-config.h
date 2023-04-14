// Biblioteca utilizada para la instanciacion y la interfaz de configuracion de la consola
#ifndef CPU_CONFIG_H_
#define CPU_CONFIG_H_

//Bibliotecas estandar
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//Bibliotecas commons
#include <commons/log.h>
//Bibliotecas static-utils
#include <utils/config-modulo.h>
//Bibliotecas internas modulo cpu
#include <cpu-estructuras.h>

// Prototipos

/**
 * @brief Crea una instancia de t_cpu_config y la instancia segun el archivo de configuración pasado por path
 * 
 * @param cpuConfigPath: Ruta desde el punto de ejecucion hacia el archivo de config 
 * @param cpuLogger: Logger del cpu
 * @return t_cpu_config*: instancia creada e inicializada de la estructura de configuración
 * 
 * @example t_cpu_config *cpuconfig = cpu_config_create("./config/config-inicial.h", cpuLogger);
 */
t_cpu_config *cpu_config_create(char *cpuConfigPath, t_log *cpuLogger);

/**
 * @brief Destruye (libera memoria) de la estructura de configuracion
 * 
 * @param self: Estructura de configuracion del cpu
 * 
 * @example cpu_config_destroy(cpuConfig); 
 */
void cpu_config_destroy(t_cpu_config *self);

/**
 * @brief Toma el valor del retardo de la instrucción guardada en configuracion
 * 
 * @param self: Estructura de configuracion del cpu
 * @return int: Ip guardada en la configuracion
 * 
 * @example 
 */
uint32_t cpu_config_get_retardo_instruccion(t_cpu_config *self);

char *cpu_config_get_ip_memoria(t_cpu_config *self);

char *cpu_config_get_ip_escucha(t_cpu_config *self);

char *cpu_config_get_puerto_escucha(t_cpu_config *self);

char *cpu_config_get_puerto_memoria(t_cpu_config *self);

uint32_t cpu_config_get_tam_max_segmento(t_cpu_config *self);

int cpu_config_get_socket_memoria(t_cpu_config *self);

int cpu_config_get_socket_kernel(t_cpu_config *self);

void cpu_config_set_socket_memoria(t_cpu_config *self, int socketMemoria);

void cpu_config_set_socket_kernel(t_cpu_config *self, int socketKernel);

#endif