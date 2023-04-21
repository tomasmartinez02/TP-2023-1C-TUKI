#include <kernel-cpu-adapter.h>

t_pcb_reducido* crear_pcb_reducido(t_pcb* pcb)
{
    t_pcb_reducido *pcbReducido = malloc(sizeof(*pcbReducido));

    pcbReducido->pid = pcb_get_pid(pcb);
    pcbReducido->instrucciones = pcb_get_instrucciones(pcb);
    pcbReducido->programCounter = pcb_get_program_counter(pcb);
    pcbReducido->registrosCpu = pcb_get_registros_cpu(pcb);
    
    return pcbReducido;
}

t_buffer *empaquetar_pcb_reducido(t_pcb_reducido* pcb) {
    t_buffer *bufferAEnviar = buffer_create();
    buffer_pack(bufferAEnviar, pcb, sizeof(pcb));
    return bufferAEnviar;
}

void enviar_pcb_a_cpu(t_pcb* pcbAEnviar, uint32_t socketCpu){
    t_pcb_reducido *pcbReducido = crear_pcb_reducido(pcbAEnviar); 
    t_buffer *bufferPcb = empaquetar_pcb_reducido(pcbReducido);
    stream_send_buffer(socketCpu, HEADER_pcb_a_ejecutar, bufferPcb);
    log_info(kernelLogger, "Se ha enviado el pcb al modulo Cpu");
}

void ejecutar_proceso(t_pcb* proceso,  uint32_t socketCpu){
 //no se si tiene mucho sentido
//enviar_pcb_a_cpu(t_pcb* pcbAEnviar, uint32_t socketCpu)
}