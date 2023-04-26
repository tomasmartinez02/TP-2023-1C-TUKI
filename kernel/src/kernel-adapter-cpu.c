#include <kernel-adapter-cpu.h>

// Funciones privadas
static void __cargar_registros_en_buffer(t_buffer *bufferAEnviar, t_pcb *pcb)
{
    t_registros_cpu *registrosCpu = pcb_get_registros_cpu(pcb);
    // Registros 4 bytes
    char *registroAx = registros_cpu_get_registro_ax(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroAx);
    free(registroAx);

    char *registroBx = registros_cpu_get_registro_bx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroBx);
    free(registroBx);

    char *registroCx = registros_cpu_get_registro_cx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroCx);
    free(registroCx);

    char *registroDx = registros_cpu_get_registro_dx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroDx);
    free(registroDx);

    // Registros 8 bytes
    char *registroEax = registros_cpu_get_registro_eax(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroEax);
    free(registroEax);

    char *registroEbx = registros_cpu_get_registro_ebx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroEbx);
    free(registroEbx);

    char *registroEcx = registros_cpu_get_registro_ecx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroEcx);
    free(registroEcx);

    char *registroEdx = registros_cpu_get_registro_edx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroEdx);
    free(registroEdx);
    
    // Registros 16 bytes
    char *registroRax = registros_cpu_get_registro_rax(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroRax);
    free(registroRax);

    char *registroRbx = registros_cpu_get_registro_rbx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroRbx);
    free(registroRbx);

    char *registroRcx = registros_cpu_get_registro_rcx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroRcx);
    free(registroRcx);

    char *registroRdx = registros_cpu_get_registro_rdx(registrosCpu);
    buffer_pack_string(bufferAEnviar, registroRdx);
    free(registroRdx);

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
    t_buffer *bufferPcb = __serializar_pcb_para_ejecucion(pcbAEnviar);
    t_buffer *bufferInstrucciones = pcb_get_instrucciones(pcbAEnviar);

    int socketCpu = kernel_config_get_socket_cpu(kernelConfig);
    stream_send_buffer(socketCpu, HEADER_pcb_a_ejecutar, bufferPcb);
    stream_send_buffer(socketCpu, HEADER_lista_instrucciones, bufferInstrucciones);

    buffer_destroy(bufferPcb);
    buffer_destroy(bufferInstrucciones);
    
    log_info(kernelDebuggingLogger, "Se ha enviado el pcb con PID <%d> al modulo Cpu para su ejecucion", pcb_get_pid(pcbAEnviar));

    return;
}

t_pcb* recibir_pcb_de_cpu() // CHEQUEAR ESTA FUNCION!! LUCAS!!!!
{
    // Aca solo lo recibimos
    t_buffer *bufferProceso = NULL;
    t_pcb *pcbRecibido = NULL;

    bufferProceso = buffer_create();

    t_header headerProceso = stream_recv_header(socketCpu);
    stream_recv_buffer(socketCpu, bufferProceso);

    buffer_unpack(bufferProceso, pcbRecibido, sizeof(pcbRecibido));

    return pcbRecibido;
}

void ejecutar_proceso(t_pcb* pcbAEjecutar)
{
    __enviar_pcb_a_cpu(pcbAEjecutar);
}

void recibir_proceso_desajolado(t_pcb* pcb)
{
    return;
}