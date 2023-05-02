#include <kernel-adapter-cpu.h>

// Funciones privadas
static void __cargar_registros_en_buffer(t_buffer *bufferAEnviar, t_pcb *pcb)
{
    t_registros_cpu *registrosCpu = pcb_get_registros_cpu(pcb);
    empaquetar_registros(bufferAEnviar, registrosCpu);
    return;
}

static t_buffer *__serializar_pcb_para_ejecucion(t_pcb *pcb)
{
    t_buffer *bufferAEnviar = buffer_create();
    uint32_t pid = pcb_get_pid(pcb);
    uint32_t programCounter = pcb_get_program_counter(pcb);

    buffer_pack(bufferAEnviar, &pid, sizeof(pid));
    buffer_pack(bufferAEnviar, &programCounter, sizeof(programCounter));

    __cargar_registros_en_buffer(bufferAEnviar, pcb);

    return bufferAEnviar;
}

static void __enviar_pcb_a_cpu(t_pcb* pcbAEnviar)
{
    int socketCpu = kernel_config_get_socket_cpu(kernelConfig);
    
    t_buffer *bufferPcb = __serializar_pcb_para_ejecucion(pcbAEnviar);
    stream_send_buffer(socketCpu, HEADER_pcb_a_ejecutar, bufferPcb);
    buffer_destroy(bufferPcb);

    t_buffer *bufferTablaSegmentos = pcb_get_tabla_segmentos(pcbAEnviar);
    stream_send_buffer(socketCpu, HEADER_tabla_segmentos, bufferTablaSegmentos);

    t_buffer *bufferInstrucciones = pcb_get_instrucciones(pcbAEnviar);
    stream_send_buffer(socketCpu, HEADER_lista_instrucciones, bufferInstrucciones);

    log_info(kernelDebuggingLogger, "Se ha enviado el pcb con PID <%d> al modulo Cpu para su ejecucion", pcb_get_pid(pcbAEnviar));

    return;
}

 t_header __recibir_pcb_de_cpu(t_pcb *pcbRecibido) // CHEQUEAR ESTA FUNCION!! LUCAS!!!! CREO QUE LA VAMOS A NECESITAR PARA EL PLANIFICADOR DE CORTO PLAZO!
{
    t_buffer *bufferProceso = NULL;
    uint32_t socketCpu = kernel_config_get_socket_cpu(kernelConfig);

    bufferProceso = buffer_create();

    t_header headerProceso = stream_recv_header(socketCpu);
    stream_recv_buffer(socketCpu, bufferProceso);

    buffer_unpack(bufferProceso, pcbRecibido, sizeof(pcbRecibido));

    return headerProceso;
} 

void ejecutar_proceso(t_pcb* pcbAEjecutar)
{
    __enviar_pcb_a_cpu(pcbAEjecutar);
}

t_header recibir_proceso_desajolado(t_pcb *pcbRecibido)
{   
    t_header headerProceso;
    headerProceso = __recibir_pcb_de_cpu(pcbRecibido);
    return headerProceso;
}