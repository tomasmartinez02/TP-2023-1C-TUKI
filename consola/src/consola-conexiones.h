// Biblioteca utilizada para cuestiones de conexion y mensajes con el kernel
#ifndef CONSOLA_CONEXIONES_H_
#define CONSOLA_CONEXIONES_H_

//Bibliotecas estandares
//#include <readline/readline.h>
//#include <consola-parser.h>
//Biblioteca Static Utils
#include <conexiones/conexiones.h>
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
#include <utils/common-utils.h>
//Bibliotecas internas modulo consola
#include <consola-config.h>
#include <consola-estructuras.h>
#include <consola-utils.h>

// Prototipos

/**
 * @brief Crea la conexion con el kernel
 * 
 * @param kernelIP: Ip del kernel 
 * @param kernelPort: Puerto del kernel
 * @param consolaConfig: Estructura de configuracion de la consola 
 * @param consolaLogger: Logger de la consola
 * @param consolaDebuggingLogger: Logger de debugging de la consola 
 * @return int: Descriptor del socket creado con kernel
 * 
 * @example const int socketKernel = conectar_a_kernel("192.168.1.1", "8000", consolaConfig, consolaLogger, consolaDebuggingLogger);
 */
int conectar_a_kernel(char *kernelIP, char *kernelPort, t_consola_config *consolaConfig, t_log *consolaLogger, t_log *consolaDebuggingLogger);

/**
 * @brief Envia el handshake inicial al kernel
 * 
 * @param kernelSocket: Descriptor del socket del kernel 
 * @param consolaLogger: Logger de la consola
 * 
 * @example send_handshake_kernel(socketKernel, consolaLogger);
 */
void send_handshake_kernel(const int kernelSocket, t_log *consolaLogger);

/**
 * @brief Espera y recibe la respuesta al handshake inicial del modulo kernel
 * 
 * @param kernelSocket: Descriptor del socket del kernel 
 * @param consolaConfig: Estructura de configuracion de la consola
 * @param consolaLogger: Logger de la consola 
 * @param consolaDebuggingLogger: Logger de debugging de la consola
 * 
 * @example receive_handshake_kernel(socketKernel, consolaConfig, consolaLogger, consolaDebuggingLogger); 
 */
void receive_handshake_kernel(const int kernelSocket, t_consola_config* consolaConfig, t_log* consolaLogger, t_log *consolaDebuggingLogger);


//void consola_enviar_instrucciones_a_kernel(const char *pathArchivoInstrucciones, const int kernelSocket, t_consola_config* consolaConfig, t_log *consolaLogger);
//uint32_t receive_pid_kernel(const int kernelSocket, t_consola_config* consolaConfig, t_log* consolaLogger);
//void wait_kernel_response(int kernelSocket, uint32_t processId, t_consola_config* consolaConfig, t_log* consolaLogger);

#endif