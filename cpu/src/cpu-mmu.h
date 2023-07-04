#ifndef CPU_MMU_H_
#define CPU_MMU_H_

// Bibliotecas estandar
#include <stdint.h>
// Bibliotecas internas
#include <cpu-config.h>

// Prototipos

uint32_t obtener_direccion_fisica(t_cpu_pcb *pcb,uint32_t dirLogica, uint32_t *numeroSegmento, uint32_t *offset, uint32_t *tamanioSegmento);
uint32_t obtener_tamanio_segun_registro(t_registro registro); 
char *obtener_valor_registro(t_registro registro, t_registros_cpu *registrosCpu);
#endif