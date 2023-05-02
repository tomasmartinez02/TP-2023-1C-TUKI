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
t_header recibir_proceso_desajolado(t_pcb* pcb);
t_header recibir_motivo_desalojo(void);

#endif