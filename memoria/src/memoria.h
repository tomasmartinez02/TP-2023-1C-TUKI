#ifndef MEMORIA_H_
#define MEMORIA_H_

// Defines logging
#define MEMORIA_LOG_PATH "./logs/memoria-minimum.log"
#define MEMORIA_LOG_PATH_DEBUGGING "./logs/memoria-debugging.log"
// Defines nombre modulo
#define MEMORIA_MODULE_NAME "Memoria"
// Defines chequeo argumentos consola
#define NUMBER_OF_ARGS_REQUIRED 2
#define ARGUMENTS_ERROR_MESSAGE "Argumentos: <pathArchivoConfiguracion>" //No dice nada en el enunciado asi que por el momento dejo 2

// Bibliotecas estandar
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
// Bibliotecas commons
#include <commons/string.h>
// Biblioteca internas
#include <memoria-estructuras.h>
#include <memoria-config.h>
#include <memoria-conexiones.h>
#include <memoria-utils.h>
#include <memoria-administracion.h>
// Bibliotecas static utils
#include <utils/logging.h>
#include <utils/common-utils.h>
// Estructuras (TADS)


// Prototipos

// Variables globales
#endif