// Biblioteca utilizada para la instanciacion y la interfaz de configuracion de la consola
#ifndef KERNEL_CONEXIONES_H_
#define KERNEL_CONEXIONES_H_

// Prototipos

/**
 * @brief Crea la conexion con la CPU
 * 
 * @param cpuIP: Ip de la CPU
 * @param cpuPort: Puerto de la CPU
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * @return int: Descriptor del socket creado con CPU
 * 
 * @example const int socketCPU = conectar_a_CPU("192.168.1.1", "8000", kernelConfig, kernelLogger, kernelDebuggingLogger);
 */
int conectar_a_CPU(char *cpuIP, char *cpuPort, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger);

/**
 * @brief Envia el handshake inicial a la cpu
 * 
 * @param cpuSocket: Descriptor del socket de la CPU
 * @param kernelLogger: Logger del kernel
 * 
 * @example send_handshake_cpu(cpuSocket, kernelLogger);
 */
void send_handshake_cpu(const int cpuSocket, t_log *kernelLogger);

/**
 * @brief Espera y recibe la respuesta al handshake inicial del modulo CPU
 * 
 * @param cpuSocket: Descriptor del socket de la CPU
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * 
 * @example receive_handshake_cpu(cpuSocket, kernelConfig, kernelLogger, kernelDebuggingLogger); 
 */
void receive_handshake_kernel(const int cpuSocket, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger);

/**
 * @brief Crea la conexion con la memoria
 * 
 * @param memoriaIP: Ip de la memoria
 * @param memoriaPort: Puerto de la memoria
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * @return int: Descriptor del socket creado con memoria
 * 
 * @example const int socketMemoria = conectar_a_CPU("192.168.1.1", "8000", kernelConfig, kernelLogger, kernelDebuggingLogger);
 */
int conectar_a_memoria(char *memoriaIP, char *memoriaPort, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger);

/**
 * @brief Envia el handshake inicial a memoria
 * 
 * @param memoriaSocket: Descriptor del socket de la memoria
 * @param kernelLogger: Logger del kernel
 * 
 * @example send_handshake_memoria(memoriaSocket, kernelLogger);
 */
void send_handshake_memoria(const int memoriaSocket, t_log *kernelLogger);

/**
 * @brief Espera y recibe la respuesta al handshake inicial del modulo memoria
 * 
 * @param memoriaSocket: Descriptor del socket de la memoria
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * 
 * @example receive_handshake_memoria(memoriaSocket, kernelConfig, kernelLogger, kernelDebuggingLogger); 
 */
void receive_handshake_memoria(const int memoriaSocket, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger);

/**
 * @brief Crea la conexion con el filesystem
 * 
 * @param fileSystemIP: Ip del fileSystem
 * @param fileSystemPort: Puerto del fileSystem
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * @return int: Descriptor del socket creado con fileSystem
 * 
 * @example const int socketfileSystem = conectar_a_fileSystem("192.168.1.1", "8000", kernelConfig, kernelLogger, kernelDebuggingLogger);
 */
int conectar_a_fileSystem(char *fileSystemIP, char *fileSystemPort, t_kernel_config *kernelConfig, t_log *kernelLogger, t_log *kernelDebuggingLogger);

/**
 * @brief Envia el handshake inicial a filesystem
 * 
 * @param fileSystemSocket: Descriptor del socket con el fileSystem
 * @param kernelLogger: Logger del kernel
 * 
 * @example send_handshake_fileSystem(fileSystemSocket, kernelLogger);
 */
void send_handshake_fileSystem(const int fileSystemSocket, t_log *kernelLogger);

/**
 * @brief Espera y recibe la respuesta al handshake inicial del modulo filesystem
 * 
 * @param fileSystemSocket: Descriptor del socket del fileSystemSocket 
 * @param kernelConfig: Estructura de configuracion del kernel
 * @param kernelLogger: Logger del kernel
 * @param kernelDebuggingLogger: Logger de debugging del kernel
 * 
 * @example receive_handshake_fileSystem(fileSystemSocket, kernelConfig, kernelLogger, kernelDebuggingLogger); 
 */
void receive_handshake_fileSystem(const int fileSystemSocket, t_kernel_config* kernelConfig, t_log* kernelLogger, t_log *kernelDebuggingLogger);

#endif
