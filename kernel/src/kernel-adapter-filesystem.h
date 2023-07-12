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
#include <kernel-estados.h>

uint32_t adapter_filesystem_existe_archivo(char *nombreArchivo);
void adapter_filesystem_pedir_creacion_archivo(char *nombreArchivo);
void adapter_filesystem_pedir_truncar_archivo(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t tamanio);
void adapter_filesystem_pedir_escribir_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pidProceso);
void adapter_filesystem_pedir_leer_archivo(t_pcb *pcbEnEjecucion, char* nombreArchivo, int32_t punteroArchivo, uint32_t direccionFisica, uint32_t cantidadBytes, uint32_t pidProceso);
void *hiloTruncate(void* arg);
void *hiloFwrite(void* arg);
void *hiloRead(void* arg);

#endif
