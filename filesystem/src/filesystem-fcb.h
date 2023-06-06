#ifndef FILESYSTEM_FCB_H
#define FILESYSTEM_FCB_H

#include <filesystem-estructuras.h>
#include <string.h>
#include <stdlib.h>
#include <commons/string.h>

// CREACIÓN Y DESTRUCCIÓN FCB
t_fcb *crear_fcb(char *nombreArchivo);
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

#endif