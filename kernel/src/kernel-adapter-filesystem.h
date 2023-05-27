#ifndef KERNEL_ADAPTER_FILESYSTEM_
#define KERNEL_ADAPTER_FILESYSTEM_

// Bibliotecas
#include <pthread.h>
#include <semaphore.h>

#include <kernel.h>
#include <kernel-estructuras.h>
#include <kernel-pcb.h>
#include <utils/common-utils.h>
#include <commons/collections/list.h>
#include <commons/string.h>

bool adapter_filesystem_existe_archivo(char *nombreArchivo);
void adapter_filesystem_pedir_creacion_archivo(char *nombreArchivo);

#endif