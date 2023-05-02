#include <cpu-pcb.h>

t_cpu_pcb* crear_pcb(int pid, int programCounter)
{
    t_cpu_pcb *pcb = malloc(sizeof(*pcb));
    
    pcb->pid = pid;
    pcb->programCounter = programCounter;
    pcb->instrucciones = NULL;
    pcb->registrosCpu = NULL;
    pcb->tablaSegmentos = NULL;

    return pcb;
}

void cpu_pcb_destroy(t_cpu_pcb* pcb) 
{
    destroy_lista_instrucciones(pcb->instrucciones);
    registros_cpu_destroy(pcb->registrosCpu);
    destruir_tabla_segmentos(pcb->tablaSegmentos);
    free(pcb);

    return;
}

int cpu_pcb_get_pid(t_cpu_pcb* pcb) 
{
    return pcb->pid;
}

int cpu_pcb_get_program_counter(t_cpu_pcb* pcb) 
{
    return pcb->programCounter;
}

void cpu_pcb_set_program_counter(t_cpu_pcb* pcb, int programCounter) 
{
    pcb->programCounter = programCounter;
}

t_list* cpu_pcb_get_instrucciones(t_cpu_pcb* pcb) 
{
    return pcb->instrucciones;
}

void cpu_pcb_set_instrucciones(t_cpu_pcb* pcb, t_list* instrucciones) 
{
    pcb->instrucciones = instrucciones;
}

t_registros_cpu* cpu_pcb_get_registros(t_cpu_pcb* pcb)
{
    return pcb->registrosCpu;
}

void cpu_pcb_set_registros(t_cpu_pcb* pcb, t_registros_cpu* registrosCpu)
{
    pcb->registrosCpu = registrosCpu;
}

t_info_segmentos **cpu_pcb_get_tabla_segmentos(t_cpu_pcb *pcb)
{
    return pcb->tablaSegmentos;
}

void cpu_pcb_set_tabla_segmentos(t_cpu_pcb* pcb, t_info_segmentos **tablaSegmentos)
{
    pcb->tablaSegmentos = tablaSegmentos;
}