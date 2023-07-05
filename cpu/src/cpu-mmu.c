#include <cpu-mmu.h>

uint32_t __redondear_para_abajo(uint32_t numero, uint32_t tamanio)
{
    uint32_t resultado = (numero / tamanio); //Esto anda bien?
    if ((numero % tamanio) == 0)
    {
        return resultado;
    }
    return resultado;
}

static uint32_t __obtener_numero_segmento(uint32_t dirLogica) 
{
    uint32_t tam_max_segmento;
    tam_max_segmento = cpu_config_get_tam_max_segmento(cpuConfig);
    return __redondear_para_abajo(dirLogica,tam_max_segmento);
}

static uint32_t __obtener_offset(uint32_t dirLogica) 
{
    uint32_t tam_max_segmento;
    tam_max_segmento = cpu_config_get_tam_max_segmento(cpuConfig);
    return (uint32_t) dirLogica % tam_max_segmento;
}

static uint32_t __obtener_base_segmento(t_cpu_pcb *pcb, uint32_t numeroSegmento,  uint32_t *tamanio)
{
    uint32_t base;
    uint32_t indice = 0;

    while (pcb->tablaSegmentos[indice]->idSegmento != numeroSegmento) {
        indice = indice + 1;
    }

    base = pcb->tablaSegmentos[indice]->direccionBase;
    *tamanio = pcb->tablaSegmentos[indice]->tamanio;

    return base;
}

// Funciones publicas

uint32_t obtener_direccion_fisica(t_cpu_pcb *pcb,uint32_t dirLogica, uint32_t *numeroSegmento, uint32_t *offset, uint32_t *tamanioSegmento)
{
    *numeroSegmento = __obtener_numero_segmento(dirLogica);
    *offset = __obtener_offset(dirLogica);
    uint32_t base = __obtener_base_segmento(pcb, *numeroSegmento, tamanioSegmento);
    uint32_t direccionFisica = base + *offset;
    return direccionFisica;
}

uint32_t obtener_tamanio_segun_registro(t_registro registro)
{
    uint32_t tamanio = 0;
    
    switch(registro){
        case REGISTRO_ax:
        case REGISTRO_bx:
        case REGISTRO_cx:
        case REGISTRO_dx:
            tamanio = 4;
            break;
        case REGISTRO_eax:
        case REGISTRO_ebx:
        case REGISTRO_ecx:
        case REGISTRO_edx:
            tamanio = 8;
            break;
        case REGISTRO_rax:
        case REGISTRO_rbx:
        case REGISTRO_rcx:
        case REGISTRO_rdx:
            tamanio = 16;
            break;
        default:
        log_error(cpuLogger,"Registro no valido");
        break;    
    }

    return tamanio;
}

char *obtener_valor_registro(t_registro registro, t_registros_cpu *registrosCpu)
{
    char* valor;
    switch(registro){
        case REGISTRO_ax:
        valor = registros_cpu_get_registro_ax(registrosCpu);
        break;
        case REGISTRO_bx:
        valor = registros_cpu_get_registro_bx(registrosCpu);
        break;
        case REGISTRO_cx:
        valor = registros_cpu_get_registro_cx(registrosCpu);
        break;
        case REGISTRO_dx:
        valor = registros_cpu_get_registro_dx(registrosCpu);
        break;
        case REGISTRO_eax:
        valor = registros_cpu_get_registro_eax(registrosCpu);
        break;
        case REGISTRO_ebx:
        valor = registros_cpu_get_registro_ebx(registrosCpu);
        break;
        case REGISTRO_ecx:
        valor = registros_cpu_get_registro_ecx(registrosCpu);
        break;
        case REGISTRO_edx:
        valor = registros_cpu_get_registro_edx(registrosCpu);
        break;
        case REGISTRO_rax:
        valor = registros_cpu_get_registro_rax(registrosCpu);
        break;
        case REGISTRO_rbx:
        valor = registros_cpu_get_registro_rbx(registrosCpu);
        break;
        case REGISTRO_rcx:
        valor = registros_cpu_get_registro_rcx(registrosCpu);
        break;
        case REGISTRO_rdx:
        valor = registros_cpu_get_registro_rdx(registrosCpu);
        break;
        default:
        log_error(cpuLogger,"Registro no valido");
        break;    
    }

    return valor;
}