#ifndef CPU_H_
#define CPU_H_

// Defines logging
#define CPU_LOG_PATH "./logs/cpu-minimum.log"
#define CPU_LOG_PATH_DEBUGGING "./logs/cpu-debugging.log"

// Defines nombre modulo
#define CPU_MODULE_NAME "Cpu"

// Defines chequeo argumentos cpu
#define NUMBER_OF_ARGS_REQUIRED 2
#define ARGUMENTS_ERROR_MESSAGE "Argumentos: <pathArchivoConfiguracion> "

// Bibliotecas estandar
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Bibliotecas commons
#include <commons/string.h>

// Bibliotecas static utils
#include <utils/logging.h>
#include <utils/common-utils.h>

// Bibliotecas internas
#include <cpu-estructuras.h>
#include <cpu-config.h>
#include <cpu-conexiones.h>
#include <cpu-atencion-kernel.h>


#endif