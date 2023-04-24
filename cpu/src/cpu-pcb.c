#include <cpu-pcb.h>

t_cpu_pcb* crear_pcb(int pid, int programCounter, t_registros_cpu *registrosCpu, t_buffer* instrucciones)
{
    t_cpu_pcb *pcb = malloc(sizeof(*pcb));
    
    pcb->pid = pid;
    pcb->instrucciones = instrucciones;
    pcb->programCounter = programCounter;
    pcb->registrosCpu = registrosCpu;

    return pcb;
}

void cpu_pcb_destroy(t_cpu_pcb* self) 
{
    //destroy_instructions_list(self->instrucciones);
    free(self->registrosCpu);
    free(self);
}

int cpu_pcb_get_pid(t_cpu_pcb* self) 
{
    return self->pid;
}

int cpu_pcb_get_program_counter(t_cpu_pcb* self) 
{
    return self->programCounter;
}

void cpu_pcb_set_program_counter(t_cpu_pcb* self, int programCounter) 
{
    self->programCounter = programCounter;
}

t_list* cpu_pcb_get_instrucciones(t_cpu_pcb* self) 
{
    return self->instrucciones;
}

void cpu_pcb_set_instrucciones(t_cpu_pcb* self, t_list* instrucciones) 
{
    self->instrucciones = instrucciones;
}

t_registros_cpu* cpu_pcb_get_registros(t_cpu_pcb* self)
{
    return self->registrosCpu;
}

void cpu_pcb_set_registros(t_cpu_pcb* self, t_registros_cpu* registrosCpu)
{
    self->registrosCpu = registrosCpu;
}

char* cpu_pcb_get_registro_ax(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroAx;

}

char* cpu_pcb_get_registro_bx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroBx;
}

char* cpu_pcb_get_registro_cx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroCx;
}

char* cpu_pcb_get_registro_dx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroDx;
}

void cpu_pcb_set_registro_ax(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroAx = registro;
}

void cpu_pcb_set_registro_bx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroBx = registro;
}

void cpu_pcb_set_registro_cx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroCx = registro;
}

void cpu_pcb_set_registro_dx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroDx = registro;
}

char* cpu_pcb_get_registro_eax(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroEax;

}

char* cpu_pcb_get_registro_ebx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroEbx;
}

char* cpu_pcb_get_registro_ecx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroEcx;
}

char* cpu_pcb_get_registro_edx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroEdx;
}

void cpu_pcb_set_registro_eax(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroEax = registro;
}

void cpu_pcb_set_registro_ebx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroEbx = registro;
}

void cpu_pcb_set_registro_ecx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroEcx = registro;
}

void cpu_pcb_set_registro_edx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroEdx = registro;
}

char* cpu_pcb_get_registro_rax(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroRax;

}

char* cpu_pcb_get_registro_rbx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroRbx;
}

char* cpu_pcb_get_registro_rcx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroRcx;
}

char* cpu_pcb_get_registro_rdx(t_cpu_pcb* self)
{
    return cpu_pcb_get_registros(self)->registroRdx;
}

void cpu_pcb_set_registro_rax(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroRax = registro;
}

void cpu_pcb_set_registro_rbx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroRbx = registro;
}

void cpu_pcb_set_registro_rcx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroRcx = registro;
}

void cpu_pcb_set_registro_rdx(t_cpu_pcb* self, char* registro)
{
    cpu_pcb_get_registros(self)->registroRdx = registro;
}