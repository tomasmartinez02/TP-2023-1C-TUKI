#ifndef FILESYSTEM_UTILS_H_
#define FILESYSTEM_UTILS_H_

// Bibliotecas static utils
#include <utils/common-utils.h>
// Bibliotecas internas
#include <filesystem-estructuras.h>
#include <filesystem-archivos.h>
#include <filesystem-config.h>

// Prototipos

/**
 * @brief Destruye las estructuras principales del filesystem
 * 
 * @param filesystemConfig: Estructura de configuracion de filesystem
 * @param filesystemLogger: Logger de filesystem
 * @param filesystemDebuggingLogger: Logger de debugging de filesystem
 * 
 * @example filesystem_destroy(filesystemConfig, filesystemLogger, filesystemDebuggingLogger);
 */
void filesystem_destroy(t_filesystem_config *filesystemConfig, t_log *filesystemLogger, t_log *filesystemDebuggingLogger);

void log_existe_archivo(char *nombreArchivo);
void log_no_existe_archivo(char *nombreArchivo);
void log_crear_archivo(char *nombreArchivo);
void log_abrir_archivo(char *nombreArchivo);
void log_truncar_archivo(char *nombreArchivo, uint32_t tamanio);
void log_acceso_bitmap(uint32_t numeroBloque, uint32_t estadoBloque);
void log_lectura_archivo(char *nombreArchivo, uint32_t puntero, uint32_t direccionMemoria, uint32_t tamanio);
void log_escritura_archivo(char *nombreArchivo, uint32_t puntero, uint32_t direccionMemoria, uint32_t tamanio);
void log_acceso_bloque_punteros(char *nombreArchivo, uint32_t numeroBloqueFs);
void log_bloque_asignado(char *nombreArchivo, uint32_t bloqueDatos);
void log_bloque_desasignado(char *nombreArchivo, uint32_t bloqueDatos);
void log_acceso_bloque(char *nombreArchivo, uint32_t numeroBloqueArchivo, uint32_t numeroBloqueFs);
uint32_t redondear_hacia_arriba(uint32_t nuevoTamanio, uint32_t tamanio);
uint32_t redondear_hacia_abajo(uint32_t numero, uint32_t tamanio);

#endif