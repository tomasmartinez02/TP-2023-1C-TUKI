#include <cpu-adapter-kernel.h>


// Funciones privadas
static t_registros_cpu* __desempaquetar_registros_de_buffer(t_buffer *bufferRecibido)
{
    t_registros_cpu *registrosCpu = NULL;

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

static void __cargar_registros_en_buffer(t_buffer *bufferAEnviar, t_cpu_pcb *pcb)
{
    // Registros 4 bytes
    char *registroAx = cpu_pcb_get_registro_ax(pcb);
    buffer_pack_string(bufferAEnviar, registroAx);
    free(registroAx);

    char *registroBx = cpu_pcb_get_registro_bx(pcb);
    buffer_pack_string(bufferAEnviar, registroBx);
    free(registroBx);

    char *registroCx = cpu_pcb_get_registro_cx(pcb);
    buffer_pack_string(bufferAEnviar, registroCx);
    free(registroCx);

    char *registroDx = cpu_pcb_get_registro_dx(pcb);
    buffer_pack_string(bufferAEnviar, registroDx);
    free(registroDx);

    // Registros 8 bytes
    char *registroEax = cpu_pcb_get_registro_eax(pcb);
    buffer_pack_string(bufferAEnviar, registroEax);
    free(registroEax);

    char *registroEbx = cpu_pcb_get_registro_ebx(pcb);
    buffer_pack_string(bufferAEnviar, registroEbx);
    free(registroEbx);

    char *registroEcx = cpu_pcb_get_registro_ecx(pcb);
    buffer_pack_string(bufferAEnviar, registroEcx);
    free(registroEcx);

    char *registroEdx = cpu_pcb_get_registro_edx(pcb);
    buffer_pack_string(bufferAEnviar, registroEdx);
    free(registroEdx);
    
    // Registros 16 bytes
    char *registroRax = cpu_pcb_get_registro_rax(pcb);
    buffer_pack_string(bufferAEnviar, registroRax);
    free(registroRax);

    char *registroRbx = cpu_pcb_get_registro_rbx(pcb);
    buffer_pack_string(bufferAEnviar, registroRbx);
    free(registroRbx);

    char *registroRcx = cpu_pcb_get_registro_rcx(pcb);
    buffer_pack_string(bufferAEnviar, registroRcx);
    free(registroRcx);

    char *registroRdx = cpu_pcb_get_registro_rdx(pcb);
    buffer_pack_string(bufferAEnviar, registroRdx);
    free(registroRdx);

    return;
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

t_buffer* __empaquetar_pcb(t_cpu_pcb* pcbAEmpaquetar)
{   
    t_buffer* bufferAEnviar = NULL;
    bufferAEnviar = buffer_create();

    uint32_t pid, programCounter;
    t_registros_cpu *registrosCpu;
    t_list *instrucciones;

    pid = cpu_pcb_get_pid(pcbAEmpaquetar);
    programCounter = cpu_pcb_get_program_counter(pcbAEmpaquetar);
    registrosCpu = cpu_pcb_get_registros(pcbAEmpaquetar);
    // Hay un problema con esta funcion, no la toma no se porque, tal vez no se puede asignar una lista asi
    instrucciones = cpu_pcb_get_instrucciones(pcbAEmpaquetar);
    
    buffer_pack(bufferAEnviar, &pid, sizeof(pid));
    buffer_pack(bufferAEnviar, &programCounter, sizeof(programCounter));
    buffer_pack(bufferAEnviar, &instrucciones, sizeof(instrucciones));

    __cargar_registros_en_buffer(bufferAEnviar, pcbAEmpaquetar);

    return bufferAEnviar;
}

char* get_registro_segun_tipo(t_registro tipoRegistro, t_cpu_pcb* pcb)
{
    switch (tipoRegistro)
    {
        case REGISTRO_ax:
            
            return cpu_pcb_get_registro_ax(pcb);
            break;

        case REGISTRO_bx:
            
            return cpu_pcb_get_registro_bx(pcb);
            break;

        case REGISTRO_cx:
            
            return cpu_pcb_get_registro_cx(pcb);
            break;

        case REGISTRO_dx:
            
            return cpu_pcb_get_registro_dx(pcb);
            break;
        case REGISTRO_eax:
            
            return cpu_pcb_get_registro_eax(pcb);
            break;

        case REGISTRO_ebx:
            
            return cpu_pcb_get_registro_ebx(pcb);
            break;

        case REGISTRO_ecx:
            
            return cpu_pcb_get_registro_ecx(pcb);
            break;

        case REGISTRO_edx:
            
            return cpu_pcb_get_registro_edx(pcb);
            break;
        case REGISTRO_rax:
            
            return cpu_pcb_get_registro_rax(pcb);
            break;

        case REGISTRO_rbx:
            
            return cpu_pcb_get_registro_rbx(pcb);
            break;

        case REGISTRO_rcx:
            
            return cpu_pcb_get_registro_rcx(pcb);
            break;

        case REGISTRO_rdx:
            
            return cpu_pcb_get_registro_rdx(pcb);
            break;

        default:
            return 0;
            break;
    }
}

static void set_registro_segun_tipo(t_registro tipoRegistro, char* valorASetear, t_cpu_pcb* pcb)
{
    switch (tipoRegistro)
    {
        case REGISTRO_ax:
            cpu_pcb_set_registro_ax(pcb, valorASetear);
            break;
        case REGISTRO_bx:
            cpu_pcb_set_registro_bx(pcb, valorASetear);
            break;
        case REGISTRO_cx:
            cpu_pcb_set_registro_cx(pcb, valorASetear);
            break;
        case REGISTRO_dx:
            cpu_pcb_set_registro_dx(pcb, valorASetear);
            break;
        case REGISTRO_eax:
            cpu_pcb_set_registro_eax(pcb, valorASetear);
            break;
        case REGISTRO_ebx:
            cpu_pcb_set_registro_ebx(pcb, valorASetear);
            break;
        case REGISTRO_ecx:
            cpu_pcb_set_registro_ecx(pcb, valorASetear);
            break;
        case REGISTRO_edx:
            cpu_pcb_set_registro_edx(pcb, valorASetear);
            break;
        case REGISTRO_rax:
            cpu_pcb_set_registro_rax(pcb, valorASetear);
            break;
        case REGISTRO_rbx:
            cpu_pcb_set_registro_rbx(pcb, valorASetear);
            break;
        case REGISTRO_rcx:
            cpu_pcb_set_registro_rcx(pcb, valorASetear);
            break;
        case REGISTRO_rdx:
            cpu_pcb_set_registro_rdx(pcb, valorASetear);
            break;        
        default:
            return ;
            break;
    }
    log_info(cpuLogger, "Registro %s seteado con valor: %s", __t_registro_to_char(tipoRegistro), get_registro_segun_tipo(tipoRegistro, pcb));
    log_info(cpuDebuggingLogger, "Registro %s seteado con valor: %s", __t_registro_to_char(tipoRegistro), get_registro_segun_tipo(tipoRegistro, pcb));
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

void incrementar_program_counter(t_cpu_pcb* pcb)
{   
    int programCounter = cpu_pcb_get_program_counter(pcb);
    cpu_pcb_set_program_counter(pcb, programCounter + 1);
}

void cpu_decode_instruccion(t_instruccion *instruccion)
{
    // no es void, después lo cambio

}

void cpu_ejecutar_instruccion(t_instruccion *instruccion, t_cpu_pcb *pcb)
{
    // tampoco es void, después lo cambio
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_tipo_instruccion tipoInstruccion = instruccion_get_tipo_instruccion(instruccion);

    switch (tipoInstruccion)
    {
    case INSTRUCCION_set:

        // todavía no se si esto se haría acá o en la parte de decode
        t_registro registro = instruccion_get_registro1(instruccion);
        char *valor = instruccion_get_valor_set(instruccion);

        uint32_t retardoInstruccion = cpu_config_get_retardo_instruccion(cpuConfig);

        intervalo_de_pausa(retardoInstruccion);
        // acá hay que hacer que setee el valor en el registro, supongo que necesitamos una funcion que se fije
        // que registro es con un switch como para saber que setter usar
        
        // No está tomando la funcion __t_registro_to_char de las static utils
        log_info(cpuLogger, "PID: <%d> - Ejecutando: <SET> - <%s> - <%s>", cpu_pcb_get_pid(pcb), __t_registro_to_char(registro), valor);
        log_info(cpuDebuggingLogger, "PID: <%d> - Ejecutando: <SET> - <%s> - <%s>", cpu_pcb_get_pid(pcb), __t_registro_to_char(registro), valor);

        set_registro_segun_tipo(registro, valor, pcb);
        incrementar_program_counter(pcb);

        break;
    case INSTRUCCION_yield:
        log_info(cpuLogger, "PID: <%d> - Ejecutando: <YIELD>", cpu_pcb_get_pid(pcb));
        log_info(cpuDebuggingLogger, "PID: <%d> - Ejecutando: <YIELD>", cpu_pcb_get_pid(pcb));

        t_buffer *bufferYield = __empaquetar_pcb(pcb);
        stream_send_buffer(socketKernel, HEADER_proceso_desalojado, bufferYield);
        buffer_destroy(bufferYield);
        break;

    case INSTRUCCION_exit:

        log_info(cpuLogger, "PID: <%d> - Ejecutando: <EXIT>", cpu_pcb_get_pid(pcb));
        log_info(cpuDebuggingLogger, "PID: <%d> - Ejecutando: <EXIT>", cpu_pcb_get_pid(pcb));

        t_buffer *bufferExit = __empaquetar_pcb(pcb);
        stream_send_buffer(socketKernel, HEADER_proceso_terminado, bufferExit);
        buffer_destroy(bufferExit);

        break;
    default:
        break;
    }
}