// Biblioteca utilizada para cosas de conexion y mensajes con la memoria
#ifndef FILESYSTEM_CONEXIONES_H_
#define FILESYSTEM_CONEXIONES_H_

//Bibliotecas estandares
//Biblioteca Static Utils
#include <conexiones/conexiones.h>
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
#include <utils/common-utils.h>
//Bibliotecas internas modulo filesystem
#include <filesystem-config.h>
#include <filesystem-estructuras.h>
#include <filesystem-utils.h>

// Prototipos

/**
 * @brief Crea la conexion con la memoria
 * 
 * @return int: Descriptor del socket creado con memoria
 * 
 * @example const int socketMemoria = conectar_a_memoria("127.0.0.1", "127.0.0.1", filesystemConfig, filesystemLogger, filesystemDebuggingLogger);
 */
int conectar_a_memoria(void);

/**
 * @brief Inicia el servidor del Filesystem
 * 
 * @return int: Descriptor del socket del servidor del Filesystem
 * 
 * @example const int socketServidorFilesystem = inicializar_servidor_filesystem();
 */
int inicializar_servidor_filesystem(void);

/**
 * @brief Acepta la conexion por parte del Kernel
 * 
 * @param socketEscucha: Socket del servidor del Filesystem 
 */
void aceptar_conexion_kernel(const int socketEscucha);

#endif