#ifndef CPU_PCB_H
#define CPU_PCB_H

#include <stdint.h>
#include <cpu-estructuras.h>

t_cpu_pcb* crear_pcb(int pid, int programCounter, t_registros_cpu *registrosCpu, t_list* instrucciones);

void cpu_pcb_destroy(t_cpu_pcb* self);
int cpu_pcb_get_pid(t_cpu_pcb* self);
int cpu_pcb_get_program_counter(t_cpu_pcb* self);
void cpu_pcb_set_program_counter(t_cpu_pcb* self, int programCounter);
//t_list* cpu_pcb_get_instrucciones(t_cpu_pcb* self);
//void cpu_pcb_set_instrucciones(t_cpu_pcb* self, t_list* instrucciones);
t_registros_cpu* cpu_pcb_get_registros(t_cpu_pcb* self);
void cpu_pcb_set_registros(t_cpu_pcb* self, t_registros_cpu* registrosCpu);

char* cpu_pcb_get_registro_ax(t_cpu_pcb* self);
char* cpu_pcb_get_registro_bx(t_cpu_pcb* self);
char* cpu_pcb_get_registro_cx(t_cpu_pcb* self);
char* cpu_pcb_get_registro_dx(t_cpu_pcb* self);

void cpu_pcb_set_registro_ax(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_bx(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_cx(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_dx(t_cpu_pcb* self, char* registro);

char* cpu_pcb_get_registro_eax(t_cpu_pcb* self);
char* cpu_pcb_get_registro_ebx(t_cpu_pcb* self);
char* cpu_pcb_get_registro_ecx(t_cpu_pcb* self);
char* cpu_pcb_get_registro_edx(t_cpu_pcb* self);

void cpu_pcb_set_registro_eax(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_ebx(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_ecx(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_edx(t_cpu_pcb* self, char* registro);


char* cpu_pcb_get_registro_rax(t_cpu_pcb* self);
char* cpu_pcb_get_registro_rbx(t_cpu_pcb* self);
char* cpu_pcb_get_registro_rcx(t_cpu_pcb* self);
char* cpu_pcb_get_registro_rdx(t_cpu_pcb* self);

void cpu_pcb_set_registro_rax(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_rbx(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_rcx(t_cpu_pcb* self, char* registro);
void cpu_pcb_set_registro_rdx(t_cpu_pcb* self, char* registro);

#endif