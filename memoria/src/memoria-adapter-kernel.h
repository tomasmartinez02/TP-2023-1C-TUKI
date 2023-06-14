#ifndef MEMORIA_ADAPTER_KERNEL_H
#define MEMORIA_ADAPTER_KERNEL_H

//bibliotecas
#include <memoria-config.h>
#include <serializacion/stream.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>
#include <memoria-administracion.h>

void adapter_kernel_enviar_tabla(t_info_segmentos** tablaCreada, t_header headerAEnviar);
uint32_t adapter_kernel_recibir_pid(uint32_t socketKernel, t_buffer* bufferRecibido);
t_info_segmentos* adapter_kernel_recibir_segmento_a_crear(uint32_t socketKernel, t_buffer* bufferRecibido);
void adapter_kernel_enviar_direccion_base(uint32_t socketKernel, uint32_t baseSegmento);

#endif