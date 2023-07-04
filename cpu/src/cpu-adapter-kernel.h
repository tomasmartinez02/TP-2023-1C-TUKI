#ifndef CPU_ADAPTER_KERNEL_H_
#define CPU_ADAPTER_KERNEL_H_

// Bibliotecas internas
#include <cpu-config.h>
#include <cpu-pcb.h>
#include <cpu-estructuras.h>
#include <cpu-utils.h>
// Bibliotecas static utils
#include <utils/logging.h>
#include <utils/instrucciones.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>
#include <serializacion/stream.h>
// Biblioteca commons
#include <commons/collections/list.h>

// Prototipos

t_cpu_pcb* recibir_pcb_de_kernel();
void enviar_pcb_desalojado_a_kernel(t_cpu_pcb *pcb);
void enviar_motivo_desalojo_exit(void);
void enviar_motivo_desalojo_yield(void);
void enviar_motivo_desalojo_io(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_signal(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_wait(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_create_segment(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_delete_segment(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_fopen(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_fclose(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_fseek(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_ftruncate(t_instruccion *siguienteInstruccion);
void enviar_motivo_desalojo_fwrite(t_instruccion *siguienteInstruccion, uint32_t direccionFisica);
void enviar_motivo_desalojo_fread(t_instruccion *siguienteInstruccion, uint32_t direccionFisica);
void enviar_motivo_desalojo_segmentation_fault();

#endif