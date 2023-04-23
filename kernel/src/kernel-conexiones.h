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
 * @brief Crea la conexion con la memoria
 * 
 * @return int: Descriptor del socket creado con memoria
 * 
 * @example const int socketMemoria = conectar_a_CPU("192.168.1.1", "8000", kernelConfig, kernelLogger, kernelDebuggingLogger);
 */
int conectar_a_memoria(void);

/**
 * @brief Crea la conexion con el filesystem
 * 
 * @return int: Descriptor del socket creado con filesystem
 * 
 * @example const int socketfileSystem = conectar_a_fileSystem("192.168.1.1", "8000", kernelConfig, kernelLogger, kernelDebuggingLogger);
 */
int conectar_a_filesystem(void);

int inicializar_servidor_kernel(void);

void aceptar_conexiones_kernel(const int socketEscucha);

#endif
