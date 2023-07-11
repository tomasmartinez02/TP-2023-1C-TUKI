#ifndef FILESYSTEM_FCB_H
#define FILESYSTEM_FCB_H

// BIBLIOTECAS UTILS
#include <utils/config-modulo.h>
// BIBLIOTECAS FILESYSTEM
#include <filesystem-estructuras.h>
#include <filesystem-config.h>
#include <filesystem-archivos.h>
#include <filesystem-utils.h>
// BIBLIOTECAS ESTANDAR
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
// BIBLIOTECAS COMMONS
#include <commons/string.h>
#include <commons/config.h>

// CREACIÓN Y DESTRUCCIÓN FCB
t_fcb *crear_nuevo_fcb(char *nombreArchivo);
void destruir_fcb(t_fcb *fcb);

// GETTERS Y SETTERS
char *fcb_get_nombre_archivo(t_fcb *fcb);
void fcb_set_nombre_archivo(t_fcb *fcb, char *nuevoNombreArchivo);
void fcb_set_tamanio_archivo(t_fcb *fcb, uint32_t nuevoTamanio);
uint32_t fcb_get_tamanio_archivo(t_fcb *fcb);
void fcb_set_tamanio_archivo(t_fcb *fcb, uint32_t nuevoTamanio);
uint32_t fcb_get_puntero_directo(t_fcb *fcb);
void fcb_set_puntero_directo(t_fcb *fcb, uint32_t nuevoPunteroDirecto);
uint32_t fcb_get_puntero_indirecto(t_fcb *fcb);
void fcb_set_puntero_indirecto(t_fcb *fcb, uint32_t nuevoPunteroIndirecto);
uint32_t fcb_get_cantidad_bloques_asignados(t_fcb *fcb);
void fcb_set_cantidad_bloques_asignados(t_fcb *fcb, uint32_t nuevaCantidadBloques);
void fcb_incrementar_cantidad_bloques_asignados(t_fcb *fcb);
void fcb_decrementar_cantidad_bloques_asignados(t_fcb *fcb);
void fcb_mostrar_por_pantalla(t_fcb* fcb);

// CREACIÓN DE ARCHIVOS DE MANEJO DE FCB
void fcb_decrementar_tamanio_en_bloque(t_fcb *fcb);
void fcb_incrementar_tamanio_en_bloque(t_fcb *fcb);
uint32_t obtener_cantidad_punteros_bloque_indirecto(t_fcb* fcb);
t_fcb* levantar_fcb(char *pathFcb);
bool persistir_fcb(t_fcb* fcb);
bool crear_archivo_nuevo_fcb(t_fcb *nuevoFcb);
void recorrer_directorio_fcbs(void);

#endif