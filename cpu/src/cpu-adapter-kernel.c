#include <cpu-adapter-kernel.h>

// Funciones privadas
static t_registros_cpu* __desempaquetar_registros_de_buffer(t_buffer *bufferRecibido)
{
    t_registros_cpu *registrosCpu;

    // Registros 4 bytes
    char* registroAx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_ax(registrosCpu, registroAx);

    char* registroBx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_bx(registrosCpu, registroBx);

    char* registroCx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_cx(registrosCpu, registroCx);

    char* registroDx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_dx(registrosCpu, registroDx);

    // Registros 8 bytes
    char* registroEax = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_eax(registrosCpu, registroEax);

    char* registroEbx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_ebx(registrosCpu, registroEbx);

    char* registroEcx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_ecx(registrosCpu, registroEcx);

    char* registroEdx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_edx(registrosCpu, registroEdx);
    
    // Registros 16 bytes
    char* registroRax = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_eax(registrosCpu, registroRax);

    char* registroRbx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_ebx(registrosCpu, registroRbx);

    char* registroRcx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_ecx(registrosCpu, registroRcx);

    char* registroRdx = buffer_unpack_string(bufferRecibido);
    registros_cpu_set_registro_edx(registrosCpu, registroRdx);

    return registrosCpu;
}

t_cpu_pcb* __crear_pcb(uint32_t pid, uint32_t programCounter, t_registros_cpu *registrosCpu, t_buffer* instrucciones)
{
    t_cpu_pcb *pcb = malloc(sizeof(*pcb));
    
    pcb->pid = pid;
    pcb->instrucciones = instrucciones;
    pcb->programCounter = programCounter;
    pcb->registrosCpu = registrosCpu;

    return pcb;
}

t_cpu_pcb* __desempaquetar_pcb(t_buffer* bufferPcb, t_buffer* bufferInstrucciones)
{   
    uint32_t pid, programCounter;
    t_registros_cpu *registrosCpu;

    buffer_unpack(bufferPcb, &pid, sizeof(pid));
    buffer_unpack(bufferPcb, &programCounter, sizeof(programCounter));
    registrosCpu = __desempaquetar_registros_de_buffer(bufferPcb);
    
    t_cpu_pcb* pcb = __crear_pcb(pid, programCounter, registrosCpu, bufferInstrucciones);
    return pcb;
}

// Funciones públicas
t_cpu_pcb* recibir_pcb_de_kernel() 
{      
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *bufferPcb, *bufferInstrucciones;

    t_header headerPcb = stream_recv_header(socketKernel);
    stream_recv_buffer(socketKernel, bufferPcb);

    t_header headerInstruccionesPcb = stream_recv_header(socketKernel);
    stream_recv_buffer(socketKernel, bufferInstrucciones);

    // Si los headers no son el pcb a ejecutar y las instrucciones recibio cualquier cosa
    if (headerPcb != HEADER_pcb_a_ejecutar && headerInstruccionesPcb != HEADER_lista_instrucciones)
    {
        //ERROR
    }

    t_cpu_pcb* pcb = __desempaquetar_pcb(bufferPcb, bufferInstrucciones);
    return pcb;
}