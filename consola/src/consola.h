#ifndef CONSOLA_H_
#define CONSOLA_H_

// Defines logging
#define CONSOLA_LOG_PATH "./logs/consola-minimum.log"
#define CONSOLA_LOG_PATH_DEBUGGING "./logs/consola-debugging.log"
// Defines nombre modulo
#define CONSOLA_MODULE_NAME "Consola"
// Defines chequeo argumentos consola
#define NUMBER_OF_ARGS_REQUIRED 3
#define ARGUMENTS_ERROR_MESSAGE "Argumentos: <pathArchivoConfiguracion> <pathInstrucciones>"

// Bibliotecas estandar
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
// Bibliotecas commons
#include <commons/string.h>
// Bibliotecas static utils
#include <utils/logging.h>
#include <utils/common-utils.h>
// Biblioteca internas
#include <consola-estructuras.h>
#include <consola-config.h>
#include <consola-conexiones.h>

#endif