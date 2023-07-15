#include <cpu-pcb.h>

t_cpu_pcb* crear_pcb(int pid, int programCounter, uint32_t tamanioTablaSegmentos)
{
    t_cpu_pcb *pcb = malloc(sizeof(*pcb));
    
    pcb->pid = pid;
    pcb->programCounter = programCounter;
    pcb->tamanioTablaSegmentos = tamanioTablaSegmentos;
    pcb->instrucciones = NULL;
    pcb->registrosCpu = NULL;
    pcb->tablaSegmentos = NULL;

    return pcb;
}

void cpu_pcb_destroy(t_cpu_pcb* pcb) 
{
    destroy_lista_instrucciones(pcb->instrucciones);
    registros_cpu_destroy(pcb->registrosCpu);
    destruir_tabla_segmentos(pcb->tablaSegmentos, pcb->tamanioTablaSegmentos);
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

void incrementar_program_counter(t_cpu_pcb* pcb)
{   
    int programCounter = cpu_pcb_get_program_counter(pcb);
    cpu_pcb_set_program_counter(pcb, programCounter + 1);
}

t_list* cpu_pcb_get_instrucciones(t_cpu_pcb* pcb) 
{
    return pcb->instrucciones;
}

void cpu_pcb_set_instrucciones(t_cpu_pcb* pcb, t_list* instrucciones) 
{
    pcb->instrucciones = instrucciones;
}

t_instruccion *pcb_fetch_siguiente_instruccion(t_cpu_pcb *pcb) 
{
    t_list *listaInstrucciones = cpu_pcb_get_instrucciones(pcb);
    uint32_t programCounter = cpu_pcb_get_program_counter(pcb);

    t_instruccion *siguienteInstruccion = list_get(listaInstrucciones, programCounter);

    return siguienteInstruccion;
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

void cpu_pcb_set_tabla_segmentos(t_cpu_pcb* pcb, t_info_segmentos **nuevaTablaSegmentos)
{
    if(pcb->tablaSegmentos != NULL){
    destruir_tabla_segmentos(pcb->tablaSegmentos, pcb->tamanioTablaSegmentos);
    }
    // Realizar una copia profunda de la tabla de segmentos
    pcb->tablaSegmentos = malloc(sizeof(t_info_segmentos *) * pcb->tamanioTablaSegmentos);
    for (int i = 0; i < pcb->tamanioTablaSegmentos; i++) {
        pcb->tablaSegmentos[i] = malloc(sizeof(t_info_segmentos));
        pcb->tablaSegmentos[i]->idSegmento = nuevaTablaSegmentos[i]->idSegmento;
        pcb->tablaSegmentos[i]->direccionBase = nuevaTablaSegmentos[i]->direccionBase;
        pcb->tablaSegmentos[i]->tamanio = nuevaTablaSegmentos[i]->tamanio;
    }

    // Liberar memoria para nuevaTablaSegmentos
    for (int i = 0; i < pcb->tamanioTablaSegmentos; i++) {
        free(nuevaTablaSegmentos[i]);
    }
    
    free(nuevaTablaSegmentos);
}

char *get_registro_segun_tipo(t_cpu_pcb *pcb, t_registro tipoRegistro)
{
    t_registros_cpu *registrosCpu = cpu_pcb_get_registros(pcb);
    switch (tipoRegistro)
    {
        case REGISTRO_ax:
            
            return registros_cpu_get_registro_ax(registrosCpu);
            break;

        case REGISTRO_bx:
            
            return registros_cpu_get_registro_bx(registrosCpu);
            break;

        case REGISTRO_cx:
            
            return registros_cpu_get_registro_cx(registrosCpu);
            break;

        case REGISTRO_dx:
            
            return registros_cpu_get_registro_dx(registrosCpu);
            break;

        case REGISTRO_eax:
            
            return registros_cpu_get_registro_eax(registrosCpu);
            break;

        case REGISTRO_ebx:
            
            return registros_cpu_get_registro_ebx(registrosCpu);
            break;

        case REGISTRO_ecx:
            
            return registros_cpu_get_registro_ecx(registrosCpu);
            break;

        case REGISTRO_edx:
            
            return registros_cpu_get_registro_edx(registrosCpu);
            break;

        case REGISTRO_rax:
            
            return registros_cpu_get_registro_rax(registrosCpu);
            break;

        case REGISTRO_rbx:
            
            return registros_cpu_get_registro_rbx(registrosCpu);
            break;

        case REGISTRO_rcx:
            
            return registros_cpu_get_registro_rcx(registrosCpu);
            break;

        case REGISTRO_rdx:
            
            return registros_cpu_get_registro_rdx(registrosCpu);
            break;

        default:
            return "NULL";
            break;
    }
}

void set_registro_segun_tipo(t_cpu_pcb *pcb, t_registro tipoRegistro, char *valorASetear)
{
    t_registros_cpu *registrosCpu = cpu_pcb_get_registros(pcb);
    switch (tipoRegistro)
    {
        case REGISTRO_ax:
            
            registros_cpu_set_registro_ax(registrosCpu, valorASetear);
            break;

        case REGISTRO_bx:
            
            registros_cpu_set_registro_bx(registrosCpu, valorASetear);
            break;

        case REGISTRO_cx:
            
            registros_cpu_set_registro_cx(registrosCpu, valorASetear);
            break;

        case REGISTRO_dx:
            
            registros_cpu_set_registro_dx(registrosCpu, valorASetear);
            break;

        case REGISTRO_eax:
            
            registros_cpu_set_registro_eax(registrosCpu, valorASetear);
            break;

        case REGISTRO_ebx:
            
            registros_cpu_set_registro_ebx(registrosCpu, valorASetear);
            break;

        case REGISTRO_ecx:
            
            registros_cpu_set_registro_ecx(registrosCpu, valorASetear);
            break;

        case REGISTRO_edx:
            
            registros_cpu_set_registro_edx(registrosCpu, valorASetear);
            break;

        case REGISTRO_rax:
            
            registros_cpu_set_registro_rax(registrosCpu, valorASetear);
            break;

        case REGISTRO_rbx:
            
            registros_cpu_set_registro_rbx(registrosCpu, valorASetear);
            break;

        case REGISTRO_rcx:
            
            registros_cpu_set_registro_rcx(registrosCpu, valorASetear);
            break;

        case REGISTRO_rdx:
            
            registros_cpu_set_registro_rdx(registrosCpu, valorASetear);
            break;

        default:
            break;
    }

    char *stringValorRegistro = get_registro_segun_tipo(pcb, tipoRegistro);
    free(stringValorRegistro);

    return;
}
