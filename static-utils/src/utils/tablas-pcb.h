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
    uint32_t idSegmento;
    uint32_t direccionBase;
    uint32_t tamanio;
};
typedef struct info_segmentos t_info_segmentos;

// Prototipos

// Info segmentos y tabla segmentos
t_info_segmentos *crear_info_segmentos(uint32_t idSegmento, uint32_t base, uint32_t tamanio);
void destruir_info_segmentos(t_info_segmentos *infoSegmentos);
t_buffer *empaquetar_tabla_segmentos(t_info_segmentos **tablaSegmentos, uint32_t tamanioTablaSegmentos);
t_info_segmentos **desempaquetar_tabla_segmentos(t_buffer *bufferTablaSegmentos, uint32_t tamanioTablaSegmentos);
void destruir_tabla_segmentos(t_info_segmentos **tablaSegmentos, uint32_t tamanioTablaSegmentos);
// uint32_t tamanio_tabla_segmentos(t_info_segmentos **tablaSegmentos);

uint32_t info_segmentos_get_idSegmento(t_info_segmentos *infoSegmentos);
void info_segmentos_set_idSegmento(uint32_t idSegmento, t_info_segmentos *infoSegmentos);
uint32_t info_segmentos_get_direccion_base(t_info_segmentos *infoSegmentos);
void info_segmentos_set_direccion_base(t_info_segmentos *infoSegmentos, uint32_t direccionBase);
uint32_t info_segmentos_get_tamanio(t_info_segmentos *infoSegmentos);
void info_segmentos_set_tamanio(t_info_segmentos *infoSegmentos, uint32_t tamanio);

#endif