#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

// Defines logging
#define FILESYSTEM_LOG_PATH "./logs/filesystem-minimum.log"
#define FILESYSTEM_LOG_PATH_DEBUGGING "./logs/filesystem-debugging.log"
// Defines nombre modulo
#define FILESYSTEM_MODULE_NAME "Filesystem"
// Defines chequeo argumentos filesystem
#define NUMBER_OF_ARGS_REQUIRED 2
#define ARGUMENTS_ERROR_MESSAGE "Argumentos: <pathArchivoConfiguracion>"

// Bibliotecas estandar
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
// Bibliotecas commons
#include <commons/collections/dictionary.h>
#include <commons/string.h>
// Bibliotecas static utils
#include <utils/logging.h>
#include <utils/common-utils.h>
// Bibliotecas internas
#include <filesystem-estructuras.h>
#include <filesystem-config.h>
#include <filesystem-conexiones.h>
#include <filesystem-archivos.h>
#include <filesystem-adapter-kernel.h>
#include <filesystem-atender-kernel.h>
#include <filesystem-fcb.h>

// Funciones publicas   
void inicializar_estructuras(void);

#endif
