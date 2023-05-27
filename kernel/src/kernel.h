#ifndef KERNEL_H_
#define KERNEL_H_

// Defines logging
#define KERNEL_LOG_PATH "./logs/kernel-minimum.log"
#define KERNEL_LOG_PATH_DEBUGGING "./logs/kernel-debugging.log"
// Defines nombre modulo
#define KERNEL_MODULE_NAME "Kernel"
// Defines chequeo argumentos consola
#define NUMBER_OF_ARGS_REQUIRED 2
#define ARGUMENTS_ERROR_MESSAGE "Argumentos: <pathArchivoConfiguracion>"

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
#include <kernel-estructuras.h>
#include <kernel-config.h>
#include <kernel-conexiones.h>
#include <kernel-estructuras.h>
#include <kernel-estados.h>
#include <kernel-pcb.h>
#include <kernel-planificacion.h>
#include <kernel-instrucciones.h>
#include <kernel-semaforos-recursos.h>
#include <kernel-adapter-cpu.h>
#include <kernel-adapter-filesystem.h>
#include <kernel-adapter-memoria.h>
#include <kernel-tabla-archivos.h>
// Estructuras (TADS)


// Prototipos

// Variables globales

#endif