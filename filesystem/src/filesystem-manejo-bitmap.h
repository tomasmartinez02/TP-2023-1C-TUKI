#ifndef FILESYSTEM_MANEJO_BITMAP_H_
#define FILESYSTEM_MANEJO_BITMAP_H_

//Bibliotecas estandar
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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

// Prototipos

int32_t bitmap_encontrar_bloque_libre(void);

void bitmap_mostrar_por_pantalla(void);

void bitmap_marcar_bloque_libre(uint32_t numeroBloque);

void bitmap_marcar_bloque_ocupado(uint32_t numeroBloque);




#endif