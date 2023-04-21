#ifndef KERNEL_CPU_ADAPTER_H
#define KERNEL_CPU_ADAPTER_H

//Static-utils libraries
#include <serializacion/buffer.h>

// Bibliotecas internas
#include <kernel-estructuras.h>
#include <kernel-pcb.h>

//Bibliotecas commons
#include <commons/log.h>

// Bibliotecas static utils
#include <serializacion/stream.h>
#include <utils/common-utils.h>

void enviar_pcb_a_cpu(t_pcb* pcbAEnviar, uint32_t socketCpu);
t_buffer *empaquetar_pcb(t_pcb* pcb);
t_pcb_reducido* crear_pcb_reducido(t_pcb* pcb);
void ejecutar_instruccion();

#endif