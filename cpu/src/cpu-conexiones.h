#ifndef CPU_CONEXIONES_H_
#define CPU_CONEXIONES_H_

//Biblioteca Static Utils
#include <conexiones/conexiones.h>
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
#include <utils/common-utils.h>

//Bibliotecas internas modulo consola
#include <cpu-config.h>
#include <cpu-estructuras.h>
#include <cpu-utils.h>

// Prototipos

/**
 * @brief Crea la conexion con la memoria
 * 
 * @param memoriaIP: Ip de la memoria 
 * @param memoriaPort: Puerto de la memoria
 * @param cpuConfig: Estructura de configuracion del cpu 
 * @param cpuLogger: Logger del cpu
 * @param cpuDebuggingLogger: Logger de debugging del cpu
 * @return int: Descriptor del socket creado con memoria
 * 
 * @example const int socketMemoria = conectar_a_memoria("192.168.1.1", "8000", cpuConfig, cpuLogger, cpuDebuggingLogger);
 */
int conectar_a_memoria(void);

int inicializar_servidor_cpu(void);

void aceptar_conexion_kernel(int socketEscucha);



#endif