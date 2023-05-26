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

void __destruir_archivo_abierto(void *archivo);
bool es_este_archivo_por_nombre(void *archivo);

void agregar_archivo_abierto_a_tabla(t_list *archivosAbiertos, t_archivo *archivoAbierto);
t_archivo *buscar_archivo_por_nombre(t_list *archivosAbiertos, char *nombreArchivo);
void destruir_lista_archivos(t_list *archivosAbiertos);
bool archivo_esta_abierto(t_archivo *archivo);

#endif