#ifndef CPU_ADAPTER_KERNEL_H
#define CPU_ADAPTER_KERNEL_H

// Bibliotecas internas
#include <cpu-config.h>
#include <cpu-pcb.h>
#include <cpu-estructuras.h>
// Bibliotecas static utils
#include <utils/logging.h>
#include <utils/instrucciones.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
// Biblioteca commons
#include <commons/collections/list.h>


t_cpu_pcb* recibir_pcb_de_kernel();
void cpu_ejecutar_instruccion(t_instruccion *instruccion, t_cpu_pcb *pcb);
void cpu_decode_instruccion(t_instruccion *instruccion);
t_instruccion* cpu_fetch_instruccion(t_cpu_pcb *pcb);
void incrementar_program_counter(t_cpu_pcb* pcb);


#endif