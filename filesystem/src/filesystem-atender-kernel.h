#ifndef FILESYSTEM_ATENDER_KERNEL_H
#define FILESYSTEM_ATENDER_KERNEL_H

//Bibliotecas estandar
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
/*#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>*/
//Bibliotecas commons
#include <commons/log.h>
//#include <commons/bitarray.h>
#include <commons/collections/dictionary.h>
//Bibliotecas static-utils
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
//Bibliotecas internas modulo filesystem
#include <filesystem-estructuras.h>
#include <filesystem-archivos.h>
#include <filesystem.h>
#include <filesystem-config.h>
#include <filesystem-adapter-kernel.h>
#include <filesystem-adapter-memoria.h>
#include <filesystem-utils.h>
#include <filesystem-fcb.h>
#include <filesystem-manejo-bloques.h>
#include <filesystem-manejo-bitmap.h>

void verificar_existencia_archivo(char *nombreArchivo);

t_fcb *crear_archivo(char *nombreArchivo);

void truncar_archivo(char *nombreArchivo, uint32_t tamanioNuevo);

void leer_archivo(char *nombreArchivo, uint32_t punteroProceso, uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pidProceso);

void escribir_archivo(char *nombreArchivo, uint32_t punteroProceso, uint32_t direccionFisica, uint32_t cantidadBytesAEscribir, uint32_t pidProceso);

void atender_peticiones_kernel(void);

void ampliar_archivo(t_fcb *fcbArchivo, uint32_t tamanioNuevo);

void reducir_archivo(t_fcb *fcbArchivo, uint32_t tamanioNuevo);

void escribir_en_bloque(uint32_t posicion,uint32_t cantidadBytesAEscribir,char *informacionAEscribir);

#endif