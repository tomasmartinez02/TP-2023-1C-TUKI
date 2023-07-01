// Modulo con funciones utilitarias del modulo memoria
#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H

// Bibliotecas estandard
#include <stdint.h>
// Bibliotecas static utils
#include <utils/common-utils.h>
#include <serializacion/stream.h>
#include <serializacion/buffer.h>
// Bibliotecas internas
#include <memoria-estructuras.h>
#include <memoria-config.h>

// Prototipos

/**
 * @brief Destruye las estructuras principales de la memoria
 * 
 * @param memoriaConfig: Estructura de configuracion de la Memoria
 * @param memoriaLogger: Logger de la Memoria
 * @param memoriaDebuggingLogger: Logger de debugging de la Memoria
 * 
 * @example memoria_destroy(memoriaConfig, memoriaLogger, memoriaDebuggingLogger); 
 */

void memoria_destroy(t_memoria_config *memoriaConfig, t_log *memoriaLogger, t_log *memoriaDebuggingLogger);
void* obtener_valor_memoria(uint32_t dirFisica, uint32_t tamanio);
void escribir_valor_en_memoria(uint32_t dirFisica, void* bytesRecibidos, uint32_t tamanio);
t_buffer* serializar_bytes_leidos(int socketModulo, uint32_t dirFisica, uint32_t bytesALeer);
uint32_t obtener_base_segmento(uint32_t idSegmento);
void* atender_modulo(void* args);
t_buffer* memoria_recibir_buffer_solicitud(int socketModulo, uint32_t *dirFisica, uint32_t *tamanio, uint32_t *pid);
void log_lectura_escritura_memoria(char *nombreModulo, uint32_t pid, uint32_t dirFisica, uint32_t tamanio, char *accion);

#endif