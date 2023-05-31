#ifndef FILESYSTEM_UTILS_H_
#define FILESYSTEM_UTILS_H_

// Bibliotecas static utils
#include <utils/common-utils.h>
// Bibliotecas internas
#include <filesystem-estructuras.h>
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

#endif