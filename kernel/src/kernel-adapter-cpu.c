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

static void __recibir_pcb_de_cpu(t_pcb *pcbRecibido) // CHEQUEAR ESTA FUNCION!! LUCAS!!!! CREO QUE LA VAMOS A NECESITAR PARA EL PLANIFICADOR DE CORTO PLAZO!
{  
    uint32_t socketCpu = kernel_config_get_socket_cpu(kernelConfig);

    // Recibimos header proceso y checkeamos que sea correcto
    t_header headerProceso = stream_recv_header(socketCpu);

    if(headerProceso != HEADER_proceso_desalojado) {
        log_error(kernelLogger,"El proceso desalojado por la CPU no se pudo recibir correctamente");
        log_error(kernelDebuggingLogger,"El proceso desalojado por la CPU no se pudo recibir correctamente");
        exit(EXIT_FAILURE);
    }

    t_buffer *bufferProceso = buffer_create();
    stream_recv_buffer(socketCpu, bufferProceso);

    // Recibimos el pid y checkeamos que coincida con el proceso en ejecucion
    uint32_t pidDesalojado;
    uint32_t pidPcbRecibido = pcb_get_pid(pcbRecibido);

    buffer_unpack(bufferProceso,&pidDesalojado,sizeof(pidDesalojado));
    if(pidDesalojado!=pidPcbRecibido) {
        log_error(kernelLogger,"El PID: %d del proceso desalojado no coincide con el proceso en ejecución con PID: %d",pidDesalojado,pidPcbRecibido);
        log_error(kernelDebuggingLogger,"El PID: %d del proceso desalojado no coincide con el proceso en ejecución con PID: %d",pidDesalojado,pidPcbRecibido);
        exit(EXIT_FAILURE);
    }
    log_info(kernelDebuggingLogger,"Se recibio correctamente el pid: %d del proceso desalojado",pidDesalojado);

    // Recibimos program counter y lo actualizamos en el pcb
    uint32_t programCounterDesalojado;
    buffer_unpack(bufferProceso,&programCounterDesalojado,sizeof(programCounterDesalojado));
    pcb_set_program_counter(pcbRecibido,programCounterDesalojado);
    log_info(kernelDebuggingLogger,"El program counter del proceso desalojado se actualizo con el valor %d",programCounterDesalojado);

    // Recibimos los registros y los actualizamos
    t_registros_cpu *registrosActuales = pcb_get_registros_cpu(pcbRecibido);
    registros_cpu_destroy(registrosActuales);

    t_registros_cpu *registrosDesalojados = desempaquetar_registros(bufferProceso);
    pcb_set_registros_cpu(pcbRecibido,registrosDesalojados);
    log_info(kernelDebuggingLogger,"Los registros del proceso desalojado se actualizaron correctamente");

    return;
} 
t_header __recibir_header_instruccion_de_cpu()
{
    uint32_t socketCpu = kernel_config_get_socket_cpu(kernelConfig);
   
    t_header headerInstruccion = stream_recv_header(socketCpu);
    stream_recv_empty_buffer(socketCpu);

    return headerInstruccion;
}
void ejecutar_proceso(t_pcb* pcbAEjecutar)
{
    __enviar_pcb_a_cpu(pcbAEjecutar);
}

void recibir_proceso_desajolado(t_pcb *pcbRecibido)
{   
    __recibir_pcb_de_cpu(pcbRecibido);
    return;
}

t_header recibir_motivo_desalojo(void)
{
    return __recibir_header_instruccion_de_cpu();
}