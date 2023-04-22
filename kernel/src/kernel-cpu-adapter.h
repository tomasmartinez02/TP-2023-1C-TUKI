#ifndef KERNEL_CPU_ADAPTER_H
#define KERNEL_CPU_ADAPTER_H

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

#endif