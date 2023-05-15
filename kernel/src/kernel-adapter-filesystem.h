#ifndef KERNEL_ADAPTER_FILESYSTEM_
#define KERNEL_ADAPTER_FILESYSTEM_

// Bibliotecas
#include <pthread.h>
#include <semaphore.h>

#include <kernel-estructuras.h>
#include <kernel-pcb.h>

#include <serializacion/stream.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>

bool condicionBusqueda(void* elemento, void* elementoBuscado);
bool archivo_esta_abierto(char *nombreArchivo);

#endif