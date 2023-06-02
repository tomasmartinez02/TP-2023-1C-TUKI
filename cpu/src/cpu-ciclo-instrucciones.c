#include <cpu-ciclo-instrucciones.h>

// Funciones privadas

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
            incrementar_program_counter(pcb);
            break;
        }
        case INSTRUCCION_movout:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            break;
        }
        case INSTRUCCION_io:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            log_info(cpuLogger, "Se incrementa el pc");
            enviar_pcb_desalojado_a_kernel(pcb);
            log_info(cpuLogger, "Envia el pcb");
            enviar_motivo_desalojo_io(siguienteInstruccion);
            log_info(cpuLogger, "Envia el desalojo");
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
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_fread(siguienteInstruccion);
            terminarEjecucion = true;
            break;
        }
        case INSTRUCCION_fwrite:
        {
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            incrementar_program_counter(pcb);
            enviar_pcb_desalojado_a_kernel(pcb);
            enviar_motivo_desalojo_fwrite(siguienteInstruccion);
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
            log_info(cpuLogger, "Entra a wait");
            log_instruccion_ejecutada(pcb, siguienteInstruccion);
            log_info(cpuLogger, "Ejecuta wait");
            incrementar_program_counter(pcb);
            log_info(cpuLogger, "Incrementa pc");
            enviar_pcb_desalojado_a_kernel(pcb);
            log_info(cpuLogger, "Envia pcb");
            enviar_motivo_desalojo_wait(siguienteInstruccion);
            log_info(cpuLogger, "Envia desalojo");
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