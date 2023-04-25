#ifndef CPU_ADAPTER_KERNEL_H
#define CPU_ADAPTER_KERNEL_H

#include <cpu-pcb.h>
#include <cpu-estructuras.h>

#include <utils/logging.h>
#include <utils/instrucciones.h>
#include <utils/common-utils.h>

#include <serializacion/buffer.h>
#include <serializacion/stream.h>

#include <cpu-config.h>

t_cpu_pcb* recibir_pcb_de_kernel();
void cpu_ejecutar_instruccion(t_instruccion *instruccion, t_cpu_pcb *pcb);
void cpu_decode_instruccion(t_instruccion *instruccion);
t_instruccion* cpu_fetch_instruccion(t_cpu_pcb *pcb);
void incrementar_program_counter(t_cpu_pcb* pcb);


#endif