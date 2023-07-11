#ifndef FILESYSTEM_ARCHIVOS_H_
#define FILESYSTEM_ARCHIVOS_H_

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
#include <filesystem-manejo-bitmap.h>
#include <filesystem-manejo-bloques.h>
#include <filesystem-fcb.h>

// Prototipos

/**
 * @brief Crea una instancia de t_superbloque y la instancia segun el archivo de superbloque pasado por path
 * 
 * @param pathSuperbloque: Ruta desde el punto de ejecucion hacia el archivo de superbloque 
 * @return t_superbloque*: instancia creada e inicalizada de la estructura de superbloque
 * 
 * @example t_superbloque *superbloque = crear_superbloque("./archivos/superbloque.dat");
 */
t_superbloque *crear_superbloque (char *pathSuperbloque);

/**
 * @brief Destruye (libera la memoria) de la estructura del archivo de superbloque
 * 
 * @param superbloque: Estructura del superbloque 
 * 
 * @example destruir_superbloque(superbloque);
 */
void destruir_superbloque (t_superbloque *superbloque);

/**
 * @brief Obtiene el valor del tamanio de los bloques
 * 
 * @param superbloque: Estructura del superbloque 
 * @return uint32_t: tamanio de los bloques
 * 
 * @example uint32_t block_size = get_superbloque_block_size (superbloque);
 */
uint32_t get_superbloque_block_size (t_superbloque *superbloque);

/**
 * @brief Obtiene la cantidad de bloques que posee Filesystem
 * 
 * @param superbloque: Estructura del superbloque 
 * @return uint32_t: cantidad de bloques
 * 
 * @example uint32_t block_count = get_superbloque_block_count (superbloque);
 */
uint32_t get_superbloque_block_count(t_superbloque *superbloque);

void abrir_bitmap (char* pathBitmap, uint32_t blockCount);

void crear_bitmap(char *pathBitmap, uint32_t blockCount);

void destruir_bitmap (void);

FILE *abrir_archivo_de_bloques(void);

void crear_archivo_de_bloques(uint32_t blockCount, uint32_t blockSize);

#endif