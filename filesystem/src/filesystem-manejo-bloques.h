#ifndef FILESYSTEM_MANEJO_BLOQUES_H_
#define FILESYSTEM_MANEJO_BLOQUES_H_

//Bibliotecas estandar
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
//Bibliotecas commons
#include <commons/log.h>
#include <commons/bitarray.h>
//Bibliotecas static-utils
#include <utils/config-modulo.h>
//Bibliotecas internas modulo filesystem
#include <filesystem-estructuras.h>
#include <filesystem-config.h>
#include <filesystem-fcb.h>
#include <filesystem-manejo-bitmap.h>
#include <filesystem-archivos.h>

// Prototipos
void asignar_puntero_directo(t_fcb *fcbArchivo);

void asignar_puntero_indirecto(t_fcb *fcbArchivo, uint32_t cantidadPunteros);

void asignar_bloques_archivo_vacio(t_fcb *fcbArchivo,uint32_t tamanioNuevo);

void desasignar_bloques(t_fcb *fcbArchivo, uint32_t cantidadBloquesDesasignar);

void desasignar_ultimo_bloque(t_fcb *fcbArchivo);

#endif