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
#include <memoria-utils.h>

// Prototipos

/**
 * @brief Inicializa el servidor de escucha de la Memoria para Cpu, Kernel y Filesystem
 * 
 * @return int: Descriptor del socket de escucha del servidor
 * 
 * @example inicializar_servidor_memoria();
 */
int inicializar_servidor_memoria(void);

/**
 * @brief Acepta conexiones de la Memoria con CPU, Kernel y Filesystem
 *
 * @param socketEscuchaCPU: Descriptor del socket de escucha del servidor
 * 
 * @example aceptar_conexiones(socketEscucha);
 */
void aceptar_conexiones(int socketEscucha);

#endif