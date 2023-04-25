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

t_cpu_pcb* __desempaquetar_pcb(t_buffer* bufferPcb, t_buffer* bufferInstrucciones)
{   
    uint32_t pid, programCounter;
    t_registros_cpu *registrosCpu;
    t_list *instrucciones;

    buffer_unpack(bufferPcb, &pid, sizeof(pid));
    buffer_unpack(bufferPcb, &programCounter, sizeof(programCounter));
    registrosCpu = __desempaquetar_registros_de_buffer(bufferPcb);
    instrucciones = instruccion_list_create_from_buffer(bufferInstrucciones, cpuLogger);

    t_cpu_pcb* pcb = crear_pcb(pid, programCounter, registrosCpu, instrucciones);
    return pcb;
}

// Funciones públicas
t_cpu_pcb* recibir_pcb_de_kernel() 
{      
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *bufferPcb = NULL;
    t_buffer *bufferInstrucciones = NULL;

    bufferPcb = buffer_create();
    bufferInstrucciones = buffer_create();

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

    free(bufferPcb);
    free(bufferInstrucciones);

    return pcb;
}

t_instruccion* cpu_fetch_instruccion(t_cpu_pcb *pcb) 
{
    t_list *listaInstrucciones = cpu_pcb_get_instrucciones(pcb);
    uint32_t programCounter = cpu_pcb_get_program_counter(pcb);

    t_instruccion *proximaInstruccion = list_get(listaInstrucciones, programCounter);

    return proximaInstruccion;
}

void cpu_decode_instruccion(t_instruccion *instruccion)
{
    // no es void, después lo cambio

}

void cpu_ejecutar_instruccion(t_instruccion *instruccion, t_cpu_pcb *pcb)
{
    // tampoco es void, después lo cambio

    t_tipo_instruccion tipoInstruccion = instruccion_get_tipo_instruccion(instruccion);

    switch (tipoInstruccion)
    {
    case INSTRUCCION_set:

        // todavía no se si esto se haría acá o en la parte de decode
        t_registro registro = instruccion_get_registro1(instruccion);
        char *valor = instruccion_get_valor_set(instruccion);

        uint32_t retardoInstruccion = cpu_config_get_retardo_instruccion(cpuConfig);

        intervalo_de_pausa(retardoInstruccion);
        
        // hacer el set del valor en el registro [no existe t_Registro_to_Char]

        //log_info(cpuLogger, "PID: <%d> - Ejecutando: <SET> - <%s> - <%d>", cpu_pcb_get_pid(pcb), t_registro_to_char(registro), valor);
        //log_info(cpuDebuggingLogger, "PID: <%d> - Ejecutando: <SET> - <%s> - <%d>", cpu_pcb_get_pid(pcb), t_registro_to_char(registro), valor);

        incrementar_program_counter(pcb);

        break;
    case INSTRUCCION_yield:
        log_info(cpuLogger, "PID: <%d> - Ejecutando: <YIELD>", cpu_pcb_get_pid(pcb));
        log_info(cpuDebuggingLogger, "PID: <%d> - Ejecutando: <YIELD>", cpu_pcb_get_pid(pcb));

        t_buffer *bufferYield = buffer_create();
        buffer_destroy(bufferYield);
        break;

    case INSTRUCCION_exit:

        log_info(cpuLogger, "PID: <%d> - Ejecutando: <EXIT>", cpu_pcb_get_pid(pcb));
        log_info(cpuDebuggingLogger, "PID: <%d> - Ejecutando: <EXIT>", cpu_pcb_get_pid(pcb));

        t_buffer *bufferExit = buffer_create();
        buffer_destroy(bufferExit);

        break;
    default:
        break;
    }
}

void incrementar_program_counter(t_cpu_pcb* pcb)
{   
    int programCounter = cpu_pcb_get_program_counter(pcb);
    cpu_pcb_set_program_counter(pcb, programCounter + 1);
}