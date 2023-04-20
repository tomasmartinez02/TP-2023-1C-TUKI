// Biblioteca utilizada para la instanciacion y la interfaz de configuracion de la consola
#ifndef KERNEL_CONEXIONES_H_
#define KERNEL_CONEXIONES_H_

//Bibliotecas estandares
#include <pthread.h>
#include <semaphore.h>

//Biblioteca Static Utils
#include <conexiones/conexiones.h>
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
#include <utils/common-utils.h>

//Bibliotecas internas modulo kernel
#include <kernel-config.h>
#include <kernel-estructuras.h>
#include <kernel-utils.h>
#include <kernel-estados.h>
#include <kernel-planificacion.h>

// Prototipos

/**
 * @brief Crea la conexion con la Cpu
 * 
 * @return int: Descriptor del socket creado con Cpu
 * 
 * @example const int socketCpu = conectar_a_cpu("192.168.1.1", "8000", kernelConfig, kernelLogger, kernelDebuggingLogger);
 */
int conectar_a_cpu(void);

/**
 * @brief Envia el handshake inicial a la cpu
 * 
 * @param socketCpu: Descriptor del socket de la CPU
 * @param kernelLogger: Logger del kernel
 * 
 * @example send_handshake_cpu(socketCpu, kernelLogger);
 */
void send_handshake_cpu(const int socketCpu, t_log *kernelLogger);

/**
 * @brief Espera y recibe la respuesta al handshake inicial del modulo Cpu
 * 
 * @param socketCpu: Descriptor del socket de la CPU
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * 
 * @example receive_handshake_cpu(socketCpu, kernelConfig, kernelLogger, kernelDebuggingLogger); 
 */
void receive_handshake_cpu(const int socketCpu, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger);

/**
 * @brief Crea la conexion con la memoria
 * 
 * @return int: Descriptor del socket creado con memoria
 * 
 * @example const int socketMemoria = conectar_a_CPU("192.168.1.1", "8000", kernelConfig, kernelLogger, kernelDebuggingLogger);
 */
int conectar_a_memoria(void);

/**
 * @brief Envia el handshake inicial a memoria
 * 
 * @param socketMemoria: Descriptor del socket de la memoria
 * @param kernelLogger: Logger del kernel
 * 
 * @example send_handshake_memoria(socketMemoria, kernelLogger);
 */
void send_handshake_memoria(const int socketMemoria, t_log *kernelLogger);

/**
 * @brief Espera y recibe la respuesta al handshake inicial del modulo memoria
 * 
 * @param socketMemoria: Descriptor del socket de la memoria
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * 
 * @example receive_handshake_memoria(socketMemoria, kernelConfig, kernelLogger, kernelDebuggingLogger); 
 */
void receive_handshake_memoria(const int socketMemoria, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger);

/**
 * @brief Crea la conexion con el filesystem
 * 
 * @return int: Descriptor del socket creado con filesystem
 * 
 * @example const int socketfileSystem = conectar_a_fileSystem("192.168.1.1", "8000", kernelConfig, kernelLogger, kernelDebuggingLogger);
 */
int conectar_a_filesystem(void);

/**
 * @brief Envia el handshake inicial a filesystem
 * 
 * @param socketFilesystem: Descriptor del socket con el filesystem
 * @param kernelLogger: Logger del kernel
 * 
 * @example send_handshake_filesystem(socketFilesystem, kernelLogger);
 */
void send_handshake_filesystem(const int socketFilesystem, t_log *kernelLogger);

/**
 * @brief Espera y recibe la respuesta al handshake inicial del modulo filesystem
 * 
 * @param socketFilesystem: Descriptor del socket del socket del filesystem 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * 
 * @example receive_handshake_fileSystem(socketFilesystem, kernelConfig, kernelLogger, kernelDebuggingLogger); 
 */
void receive_handshake_filesystem(const int socketFilesystem, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger);

int inicializar_servidor_kernel(void);

void aceptar_conexiones_kernel(const int socketEscucha);

#endif
