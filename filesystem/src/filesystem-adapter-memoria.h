#ifndef FILESYSTEM_ADAPTER_MEMORIA_H
#define FILESYSTEM_ADAPTER_MEMORIA_H

//Bibliotecas estandar
/*#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>*/
//Bibliotecas commons
#include <commons/log.h>
//#include <commons/bitarray.h>
//Bibliotecas static-utils
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
//Bibliotecas internas modulo filesystem
#include <filesystem-estructuras.h>
#include <filesystem-config.h>

// FREAD
void solicitar_escritura_memoria(uint32_t direccionFisica, uint32_t cantidadBytes, char* informacion);
bool recibir_buffer_confirmacion_escritura_memoria();

// FWRITE
void solicitar_informacion_memoria(uint32_t direccionFisica, uint32_t cantidadBytes);
char* recibir_buffer_informacion_memoria(uint32_t cantidadBytes);

#endif
