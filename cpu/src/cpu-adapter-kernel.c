#include <cpu-adapter-kernel.h>


// Funciones privadas

// Carga los registros en el buffer a enviar
static void __cargar_registros_en_buffer(t_buffer *bufferAEnviar, t_cpu_pcb *pcb)
{
    t_registros_cpu *registrosCpu = cpu_pcb_get_registros(pcb);
    empaquetar_registros(bufferAEnviar, registrosCpu);
    return;
}

// Desempaqueta pid, pc y registros enviados por kernel. Crea el pcb
static t_cpu_pcb *__desempaquetar_pcb()
{   
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);

    // Recibo header y chequeo que no haya error
    t_header headerPcb = stream_recv_header(socketKernel);
    if (headerPcb != HEADER_pcb_a_ejecutar) {
        log_error(cpuLogger, "Error al intentar recibir pcb a ejecutar del kernel");
        log_error(cpuDebuggingLogger, "Error al intentar recibir pcb a ejecutar del kernel");
        exit(EXIT_FAILURE);
    }
    t_buffer *bufferPcb = buffer_create();
    stream_recv_buffer(socketKernel, bufferPcb);
    
    // Desempaqueto pid y pc
    uint32_t pid, programCounter;
    buffer_unpack(bufferPcb, &pid, sizeof(pid));
    buffer_unpack(bufferPcb, &programCounter, sizeof(programCounter));

    // Desempaqueto registros
    t_registros_cpu *registrosCpu;
    registrosCpu = desempaquetar_registros(bufferPcb);
    buffer_destroy(bufferPcb);

    t_cpu_pcb *pcb = crear_pcb(pid, programCounter);
    cpu_pcb_set_registros(pcb, registrosCpu);

    return pcb;
}

// Desempaqueta tabla de segmentos enviada por kernel
static t_info_segmentos **__desempaquetar_tabla_segmentos()
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);

    // Recibo header y chequeo que no haya error
    t_header headerTablaSegmentos = stream_recv_header(socketKernel);
    if (headerTablaSegmentos != HEADER_tabla_segmentos) {
        log_error(cpuLogger, "Error al intentar recibir tabla de segmentos del pcb del kernel");
        log_error(cpuDebuggingLogger, "Error al intentar recibir tabla de segmentos del pcb del kernel");
        exit(EXIT_FAILURE);
    }
    
    t_buffer *bufferTablaSegmentos = buffer_create();
    stream_recv_buffer(socketKernel, bufferTablaSegmentos);

    // Desempaqueto tabla segmentos
    uint32_t tamanioTablaSegmentos;
    buffer_unpack(bufferTablaSegmentos, &tamanioTablaSegmentos, sizeof(tamanioTablaSegmentos));
    
    t_info_segmentos **tablaSegmentos = desempaquetar_tabla_segmentos(bufferTablaSegmentos, tamanioTablaSegmentos);
    buffer_destroy(bufferTablaSegmentos);

    return tablaSegmentos;
}

// Desempaqueta instrucciones enviadas por kernel
static t_list *__desempaquetar_instrucciones()
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);

    t_header headerInstruccionesPcb = stream_recv_header(socketKernel);
    if (headerInstruccionesPcb != HEADER_lista_instrucciones) {
        log_error(cpuLogger, "Error al intentar recibir lista de instrucciones del pcb a ejecutar del kernel");
        log_error(cpuDebuggingLogger, "Error al intentar recibir lista de instrucciones del pcb a ejecutar del kernel");
        exit(EXIT_FAILURE);
    }
    t_buffer *bufferInstrucciones = buffer_create();
    stream_recv_buffer(socketKernel, bufferInstrucciones);

    t_list *listaInstrucciones = instruccion_list_create_from_buffer(bufferInstrucciones, cpuDebuggingLogger);
    buffer_destroy(bufferInstrucciones);

    return listaInstrucciones;
}

t_buffer* __empaquetar_pcb(t_cpu_pcb* pcbAEmpaquetar)
{   
    t_buffer* bufferAEnviar = buffer_create();
    
    // Cargo pid y pc
    uint32_t pid = cpu_pcb_get_pid(pcbAEmpaquetar);
    buffer_pack(bufferAEnviar, &pid, sizeof(pid));
    uint32_t programCounter = cpu_pcb_get_program_counter(pcbAEmpaquetar);
    buffer_pack(bufferAEnviar, &programCounter, sizeof(programCounter));
    
    // Cargo registros
    __cargar_registros_en_buffer(bufferAEnviar, pcbAEmpaquetar);

    return bufferAEnviar;
}

// Funciones públicas
t_cpu_pcb *recibir_pcb_de_kernel() 
{   
    t_cpu_pcb *pcbRecibido = __desempaquetar_pcb();

    t_info_segmentos **tablaSegmentos = __desempaquetar_tabla_segmentos();
    cpu_pcb_set_tabla_segmentos(pcbRecibido, tablaSegmentos);
    
    t_list *listaInstrucciones = __desempaquetar_instrucciones();
    cpu_pcb_set_instrucciones(pcbRecibido, listaInstrucciones);

    return pcbRecibido;
}

void enviar_pcb_terminado_a_kernel(t_cpu_pcb *pcb)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);

    t_buffer *bufferPcbDesalojado = __empaquetar_pcb(pcb);
    stream_send_buffer(socketKernel, HEADER_instruccion_exit, bufferPcbDesalojado);
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
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_tipo_instruccion tipoInstruccion = instruccion_get_tipo_instruccion(instruccion);

    switch (tipoInstruccion)
    {
    case INSTRUCCION_set:
    {
        log_instruccion_ejecutada(pcb, instruccion);
        
        uint32_t retardoInstruccion = cpu_config_get_retardo_instruccion(cpuConfig);
        intervalo_de_pausa(retardoInstruccion);
        
        
        t_registro registro = instruccion_get_registro1(instruccion);
        char *valor = instruccion_get_valor_set(instruccion);

        set_registro_segun_tipo(pcb, registro, valor);
        free(valor);

        incrementar_program_counter(pcb);

        break;
    }

    case INSTRUCCION_yield:
    {
        log_instruccion_ejecutada(pcb, instruccion);

        t_buffer *bufferYield = __empaquetar_pcb(pcb);
        stream_send_buffer(socketKernel, HEADER_proceso_desalojado, bufferYield);
        buffer_destroy(bufferYield);
        break;
    }
    case INSTRUCCION_exit:
    {
        log_instruccion_ejecutada(pcb, instruccion);

        t_buffer *bufferExit = __empaquetar_pcb(pcb);
        stream_send_buffer(socketKernel, HEADER_proceso_terminado, bufferExit);
        buffer_destroy(bufferExit);

        break;
    }
    default:
        break;
    }
}