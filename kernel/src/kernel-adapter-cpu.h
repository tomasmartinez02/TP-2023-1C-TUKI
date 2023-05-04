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

#endif