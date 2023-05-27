#ifndef KERNEL_ADAPTER_CPU_H_
#define KERNEL_ADAPTER_CPU_H_

// Bibliotecas internas
#include <kernel-estructuras.h>
#include <kernel-pcb.h>

//Bibliotecas commons
#include <commons/log.h>

// Bibliotecas static utils
#include <serializacion/stream.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>

void ejecutar_proceso(t_pcb* pcb);
void recibir_proceso_desajolado(t_pcb* pcb);
t_header recibir_motivo_desalojo(void);
void recibir_buffer_vacio_instruccion(void);
uint32_t recibir_buffer_instruccion_io(void);
char *recibir_buffer_instruccion_con_recurso(void);
char *recibir_buffer_instruccion_fopen(void);
char *recibir_buffer_instruccion_fclose(void);
void recibir_buffer_instruccion_fseek(char **nombreArchivo, uint32_t *ubicacionNueva);
void recibir_buffer_instruccion_ftruncate(char **nombreArchivo, uint32_t *tamanioNuevo);
void recibir_buffer_instruccion_fread(char **nombreArchivo, uint32_t *direccionLogica, uint32_t *cantidadBytes);
void recibir_buffer_instruccion_fwrite(char **nombreArchivo, uint32_t *direccionLogica, uint32_t *cantidadBytes);

void recibir_buffer_instruccion_create_segment(uint32_t *idSegmento, uint32_t *tamanio);
uint32_t recibir_buffer_instruccion_delete_segment();
void enviar_pcb_a_cpu(t_pcb* pcbAEnviar);

#endif