#ifndef KERNEL_TABLA_ARCHIVOS_H_
#define KERNEL_TABLA_ARCHIVOS_H_

// Bibliotecas estandar
// Bibliotecas commons
#include <commons/collections/dictionary.h>
// Bibliotecas static utils
// Bibliotecas internas
#include <kernel.h>
#include <kernel-estructuras.h>
#include <kernel-estados.h>
#include <kernel-semaforos-recursos.h>
#include <commons/collections/dictionary.h>

void abrir_archivo(char *nombreArchivo);
static t_semaforo_recurso *crear_semaforo_archivo(void);
t_dictionary *crear_diccionario_semaforos_archivos(void);
bool archivo_esta_abierto(char *archivo);
void abrir_archivo_en_tabla_de_pcb(t_pcb *pcbEnEjecucion, char *nombreArchivo);
void cerrar_archivo_en_tabla_de_pcb(t_pcb *pcbEnEjecucion, char *nombreArchivo);

#ifndef