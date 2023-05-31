#ifndef CPU_ATENCION_KERNEL_H_
#define CPU_ATENCION_KERNEL_H_

// Bibliotecas estandar
// Bibliotecas static utils
// Bibliotecas commons
// Bibliotecas internas modulo cpu
#include <cpu-estructuras.h>
#include <cpu-pcb.h>
#include <cpu-adapter-kernel.h>
#include <cpu-ciclo-instrucciones.h>


// Prototipos

void atender_peticiones_kernel(void);

#endif