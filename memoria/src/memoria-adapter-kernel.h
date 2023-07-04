#ifndef MEMORIA_ADAPTER_KERNEL_H
#define MEMORIA_ADAPTER_KERNEL_H

//bibliotecas
#include <memoria-config.h>
#include <serializacion/stream.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>
#include <memoria-administracion.h>

void adapter_kernel_enviar_tabla(t_info_segmentos** tablaCreada, t_header headerAEnviar);
uint32_t adapter_kernel_recibir_pid(t_buffer* bufferRecibido);
t_info_segmentos* adapter_kernel_recibir_segmento_a_crear(uint32_t socketKernel, t_buffer* bufferRecibido);
void adapter_kernel_enviar_direccion_base(uint32_t socketKernel, uint32_t baseSegmento);
void adapter_kernel_solicitar_compactacion (uint32_t socketKernel);
void adapter_kernel_error_out_of_memory (uint32_t socketKernel);
uint32_t adapter_kernel_recibir_id_segmento_a_eliminar(uint32_t socketKernel, t_buffer* bufferRecibido);
void adapter_kernel_enviar_eliminacion_segmento(uint32_t socketKernel, uint32_t pid); 
void adapter_kernel_confirmar_finalizacion_proceso(uint32_t socketKernel, uint32_t pid);
void adapter_kernel_confirmar_compactacion_memoria(uint32_t socketKernel);

#endif