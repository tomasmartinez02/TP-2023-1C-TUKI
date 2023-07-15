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
    uint32_t pid, programCounter, tamanioTablaSegmentos;
    buffer_unpack(bufferPcb, &pid, sizeof(pid));
    buffer_unpack(bufferPcb, &programCounter, sizeof(programCounter));
    buffer_unpack(bufferPcb, &tamanioTablaSegmentos, sizeof(tamanioTablaSegmentos));
    t_info_segmentos **tablaSegmentos = desempaquetar_tabla_segmentos(bufferPcb, tamanioTablaSegmentos);

    // Desempaqueto registros
    t_registros_cpu *registrosCpu;
    registrosCpu = desempaquetar_registros(bufferPcb);
    buffer_destroy(bufferPcb);

    t_cpu_pcb *pcb = crear_pcb(pid, programCounter, tamanioTablaSegmentos);
    cpu_pcb_set_tabla_segmentos(pcb, tablaSegmentos);
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
        log_error(cpuLogger, "Error al intentar recibir tabla de segmentos del pcb del kernel %d", headerTablaSegmentos);
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

// Empaqueta un pcb desalojado
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

// Envio un motivo de desalojo sin un buffer
void __enviar_motivo_desalojo_vacio(t_header headerMotivoDesalojo)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    stream_send_empty_buffer(socketKernel, headerMotivoDesalojo);
    return;
}

// Funciones públicas
t_cpu_pcb *recibir_pcb_de_kernel() 
{   
    t_cpu_pcb *pcbRecibido = __desempaquetar_pcb();

    //t_info_segmentos **tablaSegmentos = __desempaquetar_tabla_segmentos();
    //cpu_pcb_set_tabla_segmentos(pcbRecibido, tablaSegmentos);
    
    t_list *listaInstrucciones = __desempaquetar_instrucciones();
    cpu_pcb_set_instrucciones(pcbRecibido, listaInstrucciones);

    return pcbRecibido;
}

void enviar_pcb_desalojado_a_kernel(t_cpu_pcb *pcb)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);

    t_buffer *bufferPcbDesalojado = __empaquetar_pcb(pcb);
    stream_send_buffer(socketKernel, HEADER_proceso_desalojado, bufferPcbDesalojado);
    buffer_destroy(bufferPcbDesalojado);

    return;
}

void enviar_motivo_desalojo_exit(void)
{
    __enviar_motivo_desalojo_vacio(HEADER_instruccion_exit);
    return;
}

void enviar_motivo_desalojo_yield(void)
{
    __enviar_motivo_desalojo_vacio(HEADER_instruccion_yield);
    return;
}

void enviar_motivo_desalojo_segmentation_fault()
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoSegFault = buffer_create();
    stream_send_empty_buffer(socketKernel, HEADER_segmentation_fault);
    buffer_destroy(desalojoSegFault);
    return;
}

void enviar_motivo_desalojo_io(t_instruccion *siguienteInstruccion)
{   
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoIo = buffer_create();
    uint32_t tiempoIo = instruccion_get_operando1(siguienteInstruccion);
    buffer_pack(desalojoIo, &tiempoIo, sizeof(tiempoIo));
    stream_send_buffer(socketKernel, HEADER_instruccion_io, desalojoIo);
    buffer_destroy(desalojoIo);
}

void enviar_motivo_desalojo_signal(t_instruccion *siguienteInstruccion)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoSignal = buffer_create();
    char* dispositivoIo = instruccion_get_dispositivo_io(siguienteInstruccion);
    buffer_pack_string(desalojoSignal, dispositivoIo);
    stream_send_buffer(socketKernel, HEADER_instruccion_signal, desalojoSignal);
    buffer_destroy(desalojoSignal);
    free(dispositivoIo); // no se si tengo que hacerlo
}

void enviar_motivo_desalojo_wait(t_instruccion *siguienteInstruccion)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoSignal = buffer_create();
    char* dispositivoIo = instruccion_get_dispositivo_io(siguienteInstruccion);
    buffer_pack_string(desalojoSignal, dispositivoIo);
    stream_send_buffer(socketKernel, HEADER_instruccion_wait, desalojoSignal);
    buffer_destroy(desalojoSignal);
    free(dispositivoIo); // no se si tengo que hacerlo
}

void enviar_motivo_desalojo_create_segment(t_instruccion *siguienteInstruccion)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoCreateSegment = buffer_create();
    uint32_t idSegmento = instruccion_get_operando1(siguienteInstruccion);
    buffer_pack(desalojoCreateSegment, &idSegmento, sizeof(idSegmento));
    uint32_t tamanio = instruccion_get_operando2(siguienteInstruccion); 
    buffer_pack(desalojoCreateSegment, &tamanio, sizeof(tamanio));
    stream_send_buffer(socketKernel, HEADER_instruccion_create_segment, desalojoCreateSegment); // Revisar header
    buffer_destroy(desalojoCreateSegment);
}

void enviar_motivo_desalojo_delete_segment(t_instruccion *siguienteInstruccion)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoDeleteSegment = buffer_create();
    uint32_t idSegmentoAEliminar = instruccion_get_operando1(siguienteInstruccion);
    buffer_pack(desalojoDeleteSegment, &idSegmentoAEliminar, sizeof(idSegmentoAEliminar));
    stream_send_buffer(socketKernel, HEADER_instruccion_delete_segment, desalojoDeleteSegment); // Revisar header
    buffer_destroy(desalojoDeleteSegment);
}

void enviar_motivo_desalojo_fopen(t_instruccion *siguienteInstruccion)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoArchivo = buffer_create();
    char* nombreArchivo = instruccion_get_nombre_archivo(siguienteInstruccion);
    buffer_pack_string(desalojoArchivo, nombreArchivo);
    stream_send_buffer(socketKernel, HEADER_instruccion_fopen, desalojoArchivo);
    buffer_destroy(desalojoArchivo);
    free(nombreArchivo);
}

void enviar_motivo_desalojo_fclose(t_instruccion *siguienteInstruccion)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoArchivo = buffer_create();
    char* nombreArchivo = instruccion_get_nombre_archivo(siguienteInstruccion);
    buffer_pack_string(desalojoArchivo, nombreArchivo);
    stream_send_buffer(socketKernel, HEADER_instruccion_fclose, desalojoArchivo);
    buffer_destroy(desalojoArchivo);
    free(nombreArchivo);
}

void enviar_motivo_desalojo_fseek(t_instruccion *siguienteInstruccion)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoArchivo = buffer_create();
    char* nombreArchivo = instruccion_get_nombre_archivo(siguienteInstruccion);
    uint32_t ubicacion = instruccion_get_operando2(siguienteInstruccion);
    buffer_pack_string(desalojoArchivo, nombreArchivo);
    buffer_pack(desalojoArchivo, &ubicacion, sizeof(ubicacion));
    stream_send_buffer(socketKernel, HEADER_instruccion_fseek, desalojoArchivo);
    buffer_destroy(desalojoArchivo);
    free(nombreArchivo);
}

void enviar_motivo_desalojo_ftruncate(t_instruccion *siguienteInstruccion)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoArchivo = buffer_create();
    char* nombreArchivo = instruccion_get_nombre_archivo(siguienteInstruccion);
    uint32_t tamanio = instruccion_get_operando2(siguienteInstruccion);
    buffer_pack_string(desalojoArchivo, nombreArchivo);
    buffer_pack(desalojoArchivo, &tamanio, sizeof(tamanio));
    stream_send_buffer(socketKernel, HEADER_instruccion_ftruncate, desalojoArchivo);
    buffer_destroy(desalojoArchivo);
    free(nombreArchivo);
}

void enviar_motivo_desalojo_fwrite(t_instruccion *siguienteInstruccion, uint32_t direccionFisica)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoArchivo = buffer_create();
    char* nombreArchivo = instruccion_get_nombre_archivo(siguienteInstruccion);
    //uint32_t direccionLogica = instruccion_get_operando2(siguienteInstruccion);
    uint32_t bytes = instruccion_get_operando3(siguienteInstruccion);
    buffer_pack_string(desalojoArchivo, nombreArchivo);
    buffer_pack(desalojoArchivo, &direccionFisica, sizeof(direccionFisica));
    buffer_pack(desalojoArchivo, &bytes, sizeof(bytes));
    stream_send_buffer(socketKernel, HEADER_instruccion_fwrite, desalojoArchivo);
    buffer_destroy(desalojoArchivo);
    free(nombreArchivo);
}

void enviar_motivo_desalojo_fread(t_instruccion *siguienteInstruccion, uint32_t direccionFisica)
{
    int socketKernel = cpu_config_get_socket_kernel(cpuConfig);
    t_buffer *desalojoArchivo = buffer_create();
    char* nombreArchivo = instruccion_get_nombre_archivo(siguienteInstruccion);
    //uint32_t direccionLogica = instruccion_get_operando2(siguienteInstruccion);
    uint32_t bytes = instruccion_get_operando3(siguienteInstruccion);
    buffer_pack_string(desalojoArchivo, nombreArchivo);
    buffer_pack(desalojoArchivo, &direccionFisica, sizeof(direccionFisica));
    buffer_pack(desalojoArchivo, &bytes, sizeof(bytes));
    stream_send_buffer(socketKernel, HEADER_instruccion_fread, desalojoArchivo);
    buffer_destroy(desalojoArchivo);
    free(nombreArchivo);
}



