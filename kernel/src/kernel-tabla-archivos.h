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

void abrir_archivo_globalmente(char *nombreArchivo);
void cerrar_archivo_globalmente(char *nombreArchivo);
t_semaforo_recurso *crear_semaforo_archivo(void);
bool semaforo_archivo_debe_desbloquear_archivo(t_semaforo_recurso *self);
t_dictionary *crear_diccionario_semaforos_archivos(void);
bool archivo_esta_abierto(char *archivo);
void abrir_archivo_en_tabla_de_pcb(t_pcb *pcbEnEjecucion, char *nombreArchivo);
void cerrar_archivo_en_tabla_de_pcb(t_pcb *pcbEnEjecucion, char *nombreArchivo);
void actualizar_puntero_archivo_en_tabla_de_pcb(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t ubicacionNueva);

#endif