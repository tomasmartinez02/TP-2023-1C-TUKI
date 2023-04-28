// Biblioteca utilizada para el manejo de tabla de segmentos y tabla archivos abiertos pcb
#ifndef TABLAS_PCB_H_
#define TABLAS_PCB_H_

// Bibliotecas estandar
#include <stdint.h>
#include <stdbool.h>
// Bibliotecas static utils
#include <serializacion/buffer.h>
// Bibliotecas commons
#include <commons/collections/list.h>
#include <commons/string.h>

// Estructuras

struct info_segmentos 
{
    uint32_t direccionBase;
    uint32_t tamanio;
};
typedef struct info_segmentos t_info_segmentos;

struct info_archivos
{
    char *nombreArchivo;
    uint32_t posicionPuntero;
};
typedef struct info_archivos t_info_archivos;

// Prototipos

// Info segmentos y tabla segmentos
t_info_segmentos *crear_info_segmentos(void);
void destruir_info_segmentos(t_info_segmentos *infoSegmentos);
t_buffer *empaquetar_tabla_segmentos(t_info_segmentos **tablaSegmentos);
t_info_segmentos **desempaquetar_tabla_segmentos(t_buffer *bufferTablaSegmentos, uint32_t tamanioTablaSegmentos);
void destruir_tabla_segmentos(t_info_segmentos **tablaSegmentos);
uint32_t tamanio_tabla_segmentos(t_info_segmentos **tablaSegmentos);
uint32_t info_segmentos_get_direccion_base(t_info_segmentos *infoSegmentos);
void info_segmentos_set_direccion_base(t_info_segmentos *infoSegmentos, uint32_t direccionBase);
uint32_t info_segmentos_get_tamanio(t_info_segmentos *infoSegmentos);
void info_segmentos_set_tamanio(t_info_segmentos *infoSegmentos, uint32_t tamanio);

// Info archivos y lista archivos

t_info_archivos *crear_info_archivos(void);
void destruir_info_archivos(t_info_archivos *infoArchivos);
char *info_archivos_get_nombre_archivo(t_info_archivos *infoArchivo);
void info_archivos_set_nombre_archivo(t_info_archivos *infoArchivo, char *nombreArchivo);
uint32_t info_archivos_get_posicion_puntero(t_info_archivos *infoArchivo);
void info_archivos_set_posicion_puntero(t_info_archivos *infoArchivo, uint32_t posicionPuntero);
void agregar_archivo_abierto(t_list *infoArchivos, t_info_archivos *archivoAbierto);
t_info_archivos *encontrar_archivo_por_nombre(t_list *infoArchivos, char *nombreArchivo);

#endif