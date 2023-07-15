#include <cpu-ciclo-instrucciones.h>

// Funciones privadas

static void __enviar_pedido_lectura_a_memoria(uint32_t dirFisica, uint32_t tamanio, uint32_t pid)
{
    int socketMemoria = cpu_config_get_socket_memoria(cpuConfig); 
    t_buffer *bufferAEnviar = buffer_create();
    buffer_pack(bufferAEnviar, &dirFisica, sizeof(dirFisica));
    buffer_pack(bufferAEnviar, &tamanio, sizeof(tamanio));
    buffer_pack(bufferAEnviar, &pid, sizeof(pid));

    stream_send_buffer(socketMemoria, HEADER_solicitud_memoria_lectura, bufferAEnviar);
    buffer_destroy(bufferAEnviar);

    return;
}

static char* __recibir_valor_a_escribir(uint32_t tamanio, uint32_t pid, uint32_t idSegmento, uint32_t dirFisica)
{
    t_buffer* bufferRecibido = buffer_create();
    int socketMemoria = cpu_config_get_socket_memoria(cpuConfig);
    t_header header = stream_recv_header(socketMemoria);
    if (header != HEADER_memoria_confirmacion_lectura)
    {
        log_error(cpuLogger, "No se pudo recibir el valor a escribir");
        log_error(cpuDebuggingLogger, "No se pudo recibir el valor a escribir");
        exit(EXIT_FAILURE);
    }

    stream_recv_buffer(socketMemoria, bufferRecibido);

    void* valor = malloc(tamanio);
    buffer_unpack(bufferRecibido, valor, tamanio);

    char* valorEscritura = malloc(tamanio);
    memcpy(valorEscritura, valor, tamanio);

    log_acceso_a_memoria(pid, "LEER", idSegmento, dirFisica, valor, tamanio);

    free(valor);
    buffer_destroy(bufferRecibido);

    return valorEscritura;
}

/*static void __ejecutar_instruccion_movin(t_cpu_pcb *pcb, t_instruccion *siguienteInstruccion)
{
    t_registro registro = instruccion_get_registro1(siguienteInstruccion);
    uint32_t dirLogica = instruccion_get_operando2(siguienteInstruccion);
    uint32_t dirFisica = obtener_direccion_fisica(pcb, dirLogica);

    uint32_t tamanio = obtener_tamanio_segun_registro(registro);

    __enviar_pedido_lectura_a_memoria(dirFisica, tamanio, cpu_pcb_get_pid(pcb));
    char *valor = __recibir_valor_a_escribir(tamanio);
  
    set_registro_segun_tipo(pcb, registro, valor);

    return;
}*/

static void __enviar_pedido_escritura_a_memoria(uint32_t dirFisica, uint32_t tamanio, uint32_t pid, void* bytesAEnviar)
{
    int socketMemoria = cpu_config_get_socket_memoria(cpuConfig); 
    t_buffer *bufferAEnviar = buffer_create();
    buffer_pack(bufferAEnviar, &dirFisica, sizeof(dirFisica));
    buffer_pack(bufferAEnviar, &tamanio, sizeof(tamanio));
    buffer_pack(bufferAEnviar, &pid, sizeof(pid));
    buffer_pack(bufferAEnviar, bytesAEnviar, tamanio);

    stream_send_buffer(socketMemoria, HEADER_solicitud_memoria_escritura, bufferAEnviar);
    buffer_destroy(bufferAEnviar);

    return;
}

static void __recibir_confirmacion_escritura()
{
    int socketMemoria = cpu_config_get_socket_memoria(cpuConfig);
    t_header header = stream_recv_header(socketMemoria);
    stream_recv_empty_buffer(socketMemoria);

    if(header != HEADER_memoria_confirmacion_escritura)
    {
        log_error(cpuLogger, "No se pudo recibir la confirmacion de escritura");
        log_error(cpuDebuggingLogger, "No se pudo recibir la confirmacion de escritura");
        exit(EXIT_FAILURE);
    }

    return;
}

/*static void __ejecutar_instruccion_movout(t_cpu_pcb *pcb, t_instruccion *siguienteInstruccion)
{
    uint32_t dirLogica = instruccion_get_operando1(siguienteInstruccion);
    uint32_t dirFisica = obtener_direccion_fisica(pcb, dirLogica);
    
    t_registro registro = instruccion_get_registro2(siguienteInstruccion);

    uint32_t tamanio = obtener_tamanio_segun_registro(registro);

    t_registros_cpu *registrosCPU = cpu_pcb_get_registros(pcb);
    char* valorRegistro = obtener_valor_registro(registro, registrosCPU);

    void* bytesAEnviar = malloc(tamanio);
    memcpy(bytesAEnviar, valorRegistro, tamanio);

    __enviar_pedido_escritura_a_memoria(dirFisica, tamanio, cpu_pcb_get_pid(pcb), bytesAEnviar);
    __recibir_confirmacion_escritura(tamanio);
    free(bytesAEnviar);
    free(valorRegistro);

    return;
} */

static void __logear_segmentation_fault(uint32_t pid, uint32_t numSegmento, uint32_t offset, uint32_t tamanio)
{
    log_info(cpuLogger, "PID: <%u> - Error SEG_FAULT- Segmento: <%u> - Offset: <%u> - Tamaño: <%u>", pid, numSegmento, offset, tamanio);
    return;
}

// Funciones publicas

void cpu_decode_instruccion(t_instruccion *instruccion)
{
    // no es void, después lo cambio

}

bool cpu_ejecutar_siguiente_instruccion(t_cpu_pcb *pcb)
{
    t_instruccion *siguienteInstruccion = pcb_fetch_siguiente_instruccion(pcb);
    t_tipo_instruccion tipoInstruccion = instruccion_get_tipo_instruccion(siguienteInstruccion);

    bool terminarEjecucion = false;
    switch (tipoInstruccion)
    {
        case INSTRUCCION_set:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            
            uint32_t retardoInstruccion = cpu_config_get_retardo_instruccion(cpuConfig);
            intervalo_de_pausa(retardoInstruccion);
            
            
            t_registro registro = instruccion_get_registro1(siguienteInstruccion);
            char *valor = instruccion_get_valor_set(siguienteInstruccion);

            set_registro_segun_tipo(pcb, registro, valor);
            free(valor);

            incrementar_program_counter(pcb);
            break;
        }
        case INSTRUCCION_yield:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_yield();

            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_exit:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_exit();

            terminarEjecucion = true;

            break;
        }
        case INSTRUCCION_movin:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            uint32_t numeroSegmento, offset, tamanioSegmento;
            t_registro registro = instruccion_get_registro1(siguienteInstruccion);
            uint32_t dirLogica = instruccion_get_operando2(siguienteInstruccion);
            uint32_t dirFisica = obtener_direccion_fisica(pcb, dirLogica, &numeroSegmento, &offset, &tamanioSegmento);
            uint32_t tamanioALeer = obtener_tamanio_segun_registro(registro);

            if((tamanioALeer + offset) <= tamanioSegmento){
                __enviar_pedido_lectura_a_memoria(dirFisica, tamanioALeer, cpu_pcb_get_pid(pcb));
                char *valor = __recibir_valor_a_escribir(tamanioALeer, cpu_pcb_get_pid(pcb), numeroSegmento, dirFisica);
                set_registro_segun_tipo(pcb, registro, valor);
                incrementar_program_counter(pcb);
                free(valor);
            }else {
                incrementar_program_counter(pcb);
                enviar_pcb_desalojado_a_kernel(pcb);
                __logear_segmentation_fault(cpu_pcb_get_pid(pcb), numeroSegmento, offset, tamanioSegmento);
                enviar_motivo_desalojo_segmentation_fault();
                terminarEjecucion = true;
            }
            break;
        }
        case INSTRUCCION_movout:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            uint32_t numeroSegmento, offset, tamanioSegmento;
            uint32_t dirLogica = instruccion_get_operando1(siguienteInstruccion);
            t_registro registro = instruccion_get_registro2(siguienteInstruccion);
            uint32_t dirFisica = obtener_direccion_fisica(pcb, dirLogica, &numeroSegmento, &offset, &tamanioSegmento);
            uint32_t tamanioALeer = obtener_tamanio_segun_registro(registro);

            if((tamanioALeer + offset) <= tamanioSegmento){
                t_registros_cpu *registrosCPU = cpu_pcb_get_registros(pcb);
                char* valorRegistro = obtener_valor_registro(registro, registrosCPU);
                void* bytesAEnviar = malloc(tamanioALeer);
                memcpy(bytesAEnviar, valorRegistro, tamanioALeer);
                log_acceso_a_memoria(cpu_pcb_get_pid(pcb), "ESCRIBIR", numeroSegmento, dirFisica, bytesAEnviar,tamanioALeer);
                __enviar_pedido_escritura_a_memoria(dirFisica, tamanioALeer, cpu_pcb_get_pid(pcb), bytesAEnviar);
                __recibir_confirmacion_escritura();
                free(bytesAEnviar);
                free(valorRegistro);
                incrementar_program_counter(pcb);
            } else{
                incrementar_program_counter(pcb);
                enviar_pcb_desalojado_a_kernel(pcb);
                __logear_segmentation_fault(cpu_pcb_get_pid(pcb), numeroSegmento, offset, tamanioSegmento);
                enviar_motivo_desalojo_segmentation_fault();
                terminarEjecucion = true;
            }    
            
            break;
        }
        case INSTRUCCION_io:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_io(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_fopen:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_fopen(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_fclose:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_fclose(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_fseek:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_fseek(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_fread:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            uint32_t numeroSegmento, offset, tamanioSegmento;
            uint32_t dirLogica = instruccion_get_operando2(siguienteInstruccion);
            uint32_t tamanioALeer = instruccion_get_operando3(siguienteInstruccion);
            uint32_t dirFisica = obtener_direccion_fisica(pcb, dirLogica, &numeroSegmento, &offset, &tamanioSegmento);

            if((tamanioALeer + offset) <= tamanioSegmento){
                incrementar_program_counter(pcb);
                enviar_pcb_desalojado_a_kernel(pcb);
                enviar_motivo_desalojo_fread(siguienteInstruccion, dirFisica);
            }else {
                incrementar_program_counter(pcb);
                enviar_pcb_desalojado_a_kernel(pcb);
                __logear_segmentation_fault(cpu_pcb_get_pid(pcb), numeroSegmento, offset, tamanioSegmento);
                enviar_motivo_desalojo_segmentation_fault();
            }
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_fwrite:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            uint32_t numeroSegmento, offset, tamanioSegmento;
            uint32_t dirLogica = instruccion_get_operando2(siguienteInstruccion);
            uint32_t tamanioALeer = instruccion_get_operando3(siguienteInstruccion);
            uint32_t dirFisica = obtener_direccion_fisica(pcb, dirLogica, &numeroSegmento, &offset, &tamanioSegmento);

            if((tamanioALeer + offset) <= tamanioSegmento){
                incrementar_program_counter(pcb);
                enviar_pcb_desalojado_a_kernel(pcb);
                enviar_motivo_desalojo_fwrite(siguienteInstruccion, dirFisica);
            }else {
                incrementar_program_counter(pcb);
                enviar_pcb_desalojado_a_kernel(pcb);
                __logear_segmentation_fault(cpu_pcb_get_pid(pcb), numeroSegmento, offset, tamanioSegmento);
                enviar_motivo_desalojo_segmentation_fault();
            }
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_ftruncate:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_ftruncate(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_wait:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_wait(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_signal:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_signal(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_create_segment:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_create_segment(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_delete_segment:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_delete_segment(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        default:
        {
            char *instruccionString = instruccion_get_to_string(siguienteInstruccion);
            log_error(cpuLogger, "La instruccion %s no existe!", instruccionString);
            log_error(cpuDebuggingLogger, "La instruccion %s no existe!", instruccionString);
            free(instruccionString);
            exit(EXIT_FAILURE);
            break;
        }
    }

    return terminarEjecucion;
}
