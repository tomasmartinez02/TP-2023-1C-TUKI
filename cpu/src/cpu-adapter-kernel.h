#ifndef CPU_ADAPTER_KERNEL_H
#define CPU_ADAPTER_KERNEL_H

#include <cpu-pcb.h>
#include <cpu-estructuras.h>

#include <utils/logging.h>
#include <utils/instrucciones.h>

#include <serializacion/buffer.h>
#include <serializacion/stream.h>

#include <cpu-config.h>

t_cpu_pcb* recibir_pcb_de_kernel();
void ejecutar_programa(t_cpu_pcb* pcb);
void incrementar_program_counter(t_cpu_pcb* pcb);


#endif