#ifndef KERNEL_ADAPTER_FILESYSTEM_
#define KERNEL_ADAPTER_FILESYSTEM_

// Bibliotecas
#include <pthread.h>
#include <semaphore.h>

#include <kernel.h>
#include <kernel-estructuras.h>
#include <kernel-pcb.h>
#include <kernel-archivos.h>

#include <utils/common-utils.h>
#include <commons/collections/list.h>
#include <commons/string.h>

bool archivo_existe_en_filesystem(char *nombreArchivo);
void pedir_creacion_archivo_a_filesystem(char *nombreArchivo);

#endif