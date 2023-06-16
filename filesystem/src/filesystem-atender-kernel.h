#ifndef FILESYSTEM_ATENDER_KERNEL_H
#define FILESYSTEM_ATENDER_KERNEL_H

//Bibliotecas estandar
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
#include <filesystem-utils.h>
#include <filesystem-fcb.h>
#include <filesystem-manejo-bloques.h>

void atender_peticiones_kernel(void);

#endif