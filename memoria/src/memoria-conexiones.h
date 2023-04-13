// Biblioteca utilizada para la instanciacion y la interfaz de configuracion de la consola
#ifndef MEMORIA_CONEXIONES_H_
#define MEMORIA_CONEXIONES_H_

//Bibliotecas estandares
//Biblioteca Static Utils
#include <conexiones/conexiones.h>
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
#include <utils/common-utils.h>
//Bibliotecas internas modulo consola
#include <memoria-config.h>
#include <memoria-estructuras.h>

// Prototipos

/**
 * @brief Inicializa el servidor de escucha de Memoria para la CPU
 * 
 * @return int: Descriptor del socket de escucha para la CPU
 * 
 * @example inicializar_servidor_memoria_para_cpu();
 */
int inicializar_servidor_memoria_para_cpu(void);

/**
 * @brief Acepta conexiones de la memoria con la cpu
 *
 * @param socketEscuchaCPU: Descriptor del socket de escucha de la CPU
 * 
 * @example aceptar_conexiones_memoria_con_cpu(socketEscuchaCPU);
 */
void aceptar_conexiones_memoria_con_cpu(const int socketEscuchaCPU);

/**
 * @brief Inicializa el servidor de escucha de Memoria para el filesystem
 * 
 * @return int: Descriptor del socket de escucha para la CPU
 * 
 * @example inicializar_servidor_memoria_para_filesystem();
 */
int inicializar_servidor_memoria_para_filesystem(void);

/**
 * @brief Acepta conexiones de la memoria con el filesystem
 *
 * @param socketEscuchaFilesystem: Descriptor del socket de escucha del filesystem
 * 
 * @example aceptar_conexiones_memoria_con_filesystem(socketEscuchaCPU);
 */
void aceptar_conexiones_memoria_con_filesystem(const int socketEscuchaFilesystem) 

/**
 * @brief Inicializa el servidor de escucha de Memoria para el kernel
 * 
 * @return int: Descriptor del socket de escucha para el kernel
 * 
 * @example inicializar_servidor_memoria_para_kernel();
 */
int inicializar_servidor_memoria_para_kernel(void);

/**
 * @brief Acepta conexiones de la memoria con el kernel
 *
 * @param socketEscuchaKernel: Descriptor del socket de escucha de la CPU
 * 
 * @example aceptar_conexiones_memoria_con_filesystem(socketEscuchaKernel);
 */
void aceptar_conexiones_memoria_con_kernel(const int socketEscuchaKernel);

#endif