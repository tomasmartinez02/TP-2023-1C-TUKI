#ifndef FILESYSTEM_ADAPTER_KERNEL_H
#define FILESYSTEM_ADAPTER_KERNEL_H

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

t_header recibir_header_peticion_de_kernel(void);
char *recibir_buffer_nombre_archivo(void);
void recibir_buffer_truncate_archivo(char **nombreArchivo, uint32_t *tamanioNuevo);
void recibir_buffer_lectura_archivo(char **nombreArchivo, uint32_t *puntero, uint32_t *direccionFisica, uint32_t *cantidadBytes, uint32_t *pid);
void recibir_buffer_escritura_archivo(char **nombreArchivo, uint32_t *puntero, uint32_t *direccionFisica, uint32_t *cantidadBytes, uint32_t *pid);
void enviar_confirmacion_existencia_archivo(void);
void enviar_confirmacion_no_existencia_archivo(void);
void enviar_confirmacion_archivo_creado(void);
void enviar_confirmacion_fwrite_finalizado(void);
void enviar_confirmacion_fread_finalizado(void);
void enviar_confirmacion_tamanio_archivo_modificado(void);

#endif
