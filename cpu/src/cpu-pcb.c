#include <cpu-pcb.h>

t_cpu_pcb* crear_pcb(int pid, int programCounter, t_registros_cpu *registrosCpu, t_list* instrucciones)
{
    t_cpu_pcb *pcb = malloc(sizeof(*pcb));
    
    pcb->pid = pid;
    pcb->instrucciones = instrucciones;
    pcb->programCounter = programCounter;
    pcb->registrosCpu = registrosCpu;

    return pcb;
}

void cpu_pcb_destroy(t_cpu_pcb* pcb) 
{
    //destroy_instructions_list(pcb->instrucciones);
    free(pcb->registrosCpu);
    free(pcb);
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

char* cpu_pcb_get_registro_ax(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroAx;

}

char* cpu_pcb_get_registro_bx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroBx;
}

char* cpu_pcb_get_registro_cx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroCx;
}

char* cpu_pcb_get_registro_dx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroDx;
}

void cpu_pcb_set_registro_ax(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroAx = registro;
}

void cpu_pcb_set_registro_bx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroBx = registro;
}

void cpu_pcb_set_registro_cx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroCx = registro;
}

void cpu_pcb_set_registro_dx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroDx = registro;
}

char* cpu_pcb_get_registro_eax(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroEax;

}

char* cpu_pcb_get_registro_ebx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroEbx;
}

char* cpu_pcb_get_registro_ecx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroEcx;
}

char* cpu_pcb_get_registro_edx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroEdx;
}

void cpu_pcb_set_registro_eax(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroEax = registro;
}

void cpu_pcb_set_registro_ebx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroEbx = registro;
}

void cpu_pcb_set_registro_ecx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroEcx = registro;
}

void cpu_pcb_set_registro_edx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroEdx = registro;
}

char* cpu_pcb_get_registro_rax(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroRax;

}

char* cpu_pcb_get_registro_rbx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroRbx;
}

char* cpu_pcb_get_registro_rcx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroRcx;
}

char* cpu_pcb_get_registro_rdx(t_cpu_pcb* pcb)
{
    return cpu_pcb_get_registros(pcb)->registroRdx;
}

void cpu_pcb_set_registro_rax(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroRax = registro;
}

void cpu_pcb_set_registro_rbx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroRbx = registro;
}

void cpu_pcb_set_registro_rcx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroRcx = registro;
}

void cpu_pcb_set_registro_rdx(t_cpu_pcb* pcb, char* registro)
{
    cpu_pcb_get_registros(pcb)->registroRdx = registro;
}