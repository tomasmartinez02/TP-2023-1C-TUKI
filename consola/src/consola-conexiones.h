// Biblioteca utilizada para cuestiones de conexion y mensajes con el kernel
#ifndef CONSOLA_CONEXIONES_H_
#define CONSOLA_CONEXIONES_H_

//Bibliotecas estandares
//#include <readline/readline.h>
#include <consola-parser.h>
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
 * @return int: Descriptor del socket creado con kernel
 * 
 * @example const int socketKernel = conectar_a_kernel();
 */
int conectar_a_kernel(void);

/**
 * @brief Envia el handshake inicial al kernel
 * 
 * @param socketKernel: Descriptor del socket del kernel 
 * @param consolaLogger: Logger de la consola
 * 
 * @example send_handshake_kernel(socketKernel, consolaLogger);
 */
void send_handshake_kernel(const int socketKernel, t_log *consolaLogger);

/**
 * @brief Espera y recibe la respuesta al handshake inicial del modulo kernel
 * 
 * @param socketKernel: Descriptor del socket del kernel 
 * @param consolaConfig: Estructura de configuracion de la consola
 * @param consolaLogger: Logger de la consola 
 * @param consolaDebuggingLogger: Logger de debugging de la consola
 * 
 * @example receive_handshake_kernel(socketKernel, consolaConfig, consolaLogger, consolaDebuggingLogger); 
 */
void receive_handshake_kernel(const int socketKernel, t_consola_config* consolaConfig, t_log* consolaLogger, t_log *consolaDebuggingLogger);

/**
 * @brief Parsea y envia las instrucciones leidas del archivo de instrucciones
 * 
 * @param pathArchivoInstrucciones: Path al archivo de instrucciones 
 */
void consola_enviar_instrucciones_a_kernel(const char *pathArchivoInstrucciones);

/**
 * @brief Recibe el pid del kernel
 *  
 * @return uint32_t: Pid de la consola 
 */
uint32_t receive_pid_kernel();

/**
 * @brief Espera el mensaje de finalizacion del kernel
 * 
 */
void wait_kernel_response();

#endif