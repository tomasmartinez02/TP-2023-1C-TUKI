#ifndef CPU_CICLO_INSTRUCCIONES_H_
#define CPU_CICLO_INSTRUCCIONES_H_

// Bibliotecas estandar
// Bibliotecas static utils
#include <utils/instrucciones.h>
// Bibliotecas commons
// Bibliotecas internas modulo cpu
#include <cpu-utils.h>  
#include <cpu-adapter-kernel.h>
#include <cpu-pcb.h>
#include <cpu-mmu.h>
#include <cpu-estructuras.h>

// Prototipos
bool cpu_ejecutar_siguiente_instruccion(t_cpu_pcb *pcb);

#endif