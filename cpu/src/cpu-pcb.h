#ifndef CPU_PCB_H_
#define CPU_PCB_H_

// Bibliotecas estandar
#include <stdint.h>
// Bibliotecas internas
#include <cpu-estructuras.h>
// Bibliotecas static utils
#include <utils/instrucciones.h>

// Prototipos

t_cpu_pcb* crear_pcb(int pid, int programCounter, uint32_t tamanioTablaSegmentos);
void cpu_pcb_destroy(t_cpu_pcb* self);
int cpu_pcb_get_pid(t_cpu_pcb* self);
int cpu_pcb_get_program_counter(t_cpu_pcb* self);
void cpu_pcb_set_program_counter(t_cpu_pcb* self, int programCounter);
void incrementar_program_counter(t_cpu_pcb* pcb);
t_list* cpu_pcb_get_instrucciones(t_cpu_pcb* self);
void cpu_pcb_set_instrucciones(t_cpu_pcb* self, t_list* instrucciones);
t_instruccion *pcb_fetch_siguiente_instruccion(t_cpu_pcb *pcb);
t_registros_cpu* cpu_pcb_get_registros(t_cpu_pcb* self);
void cpu_pcb_set_registros(t_cpu_pcb* self, t_registros_cpu* registrosCpu);
t_info_segmentos **cpu_pcb_get_tabla_segmentos(t_cpu_pcb *pcb);
void cpu_pcb_set_tabla_segmentos(t_cpu_pcb* pcb, t_info_segmentos **tablaSegmentos);
char *get_registro_segun_tipo(t_cpu_pcb *pcb, t_registro tipoRegistro);
void set_registro_segun_tipo(t_cpu_pcb *pcb, t_registro tipoRegistro, char *valorASetear);

#endif