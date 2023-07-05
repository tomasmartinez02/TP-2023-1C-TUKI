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
#include <math.h>
//Bibliotecas commons
#include <commons/log.h>
#include <commons/bitarray.h>
//Bibliotecas static-utils
#include <utils/config-modulo.h>
//Bibliotecas internas modulo filesystem
#include <filesystem-estructuras.h>
#include <filesystem-config.h>
#include <filesystem-fcb.h>
#include <filesystem-utils.h>
#include <filesystem-manejo-bitmap.h>
#include <filesystem-archivos.h>

// Prototipos

void asignar_puntero_directo(t_fcb *fcbArchivo);

void asignar_puntero_indirecto(t_fcb *fcbArchivo);

void asignar_bloques_archivo_vacio(t_fcb *fcbArchivo, uint32_t tamanioNuevo);

void asignar_bloques_archivo_no_vacio(t_fcb *fcbArchivo, uint32_t tamanioNuevo);

void asignar_bloques(t_fcb *fcbArchivo, uint32_t cantidadBloques);

void desasignar_bloques(t_fcb *fcbArchivo, uint32_t cantidadBloquesDesasignar);

void desasignar_ultimo_bloque(t_fcb *fcbArchivo);

char* archivo_de_bloques_leer_bloque(uint32_t bloque);

int32_t archivo_de_bloques_leer_n_puntero_de_bloque_de_punteros(uint32_t bloque, uint32_t punteroN);

int32_t archivo_de_bloques_leer_primer_puntero_de_bloque_de_punteros(t_fcb* fcb);

int32_t leer_ultimo_puntero_de_bloque_de_punteros(t_fcb* fcb);

bool archivo_de_bloques_escribir_en_bloque(uint32_t bloque, char* informacion);

void desasignar_bloque_punteros(t_fcb *fcbArchivo);

void desasignar_puntero_directo(t_fcb *fcbArchivo);

void vaciar_archivo(t_fcb *fcbArchivo);

uint32_t obtener_bloque_absoluto(t_fcb* fcbArchivo, uint32_t punteroFseek);

uint32_t obtener_bloque_relativo(t_fcb* fcbArchivo, uint32_t punteroFseek);

uint32_t obtener_posicion_en_bloque(uint32_t punteroFseek);

uint32_t obtener_posicion_absoluta(t_fcb* fcbArchivo, uint32_t punteroFseek);

uint32_t espacio_disponible_en_bloque_desde_posicion(uint32_t punteroFseek);

uint32_t espacio_disponible_en_bloque(uint32_t posicion);

uint32_t buscar_siguiente_bloque(uint32_t bloqueActual, t_fcb *fcbArchivo);

#endif