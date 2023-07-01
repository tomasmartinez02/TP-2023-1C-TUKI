#include <cpu-mmu.h>

uint32_t obtener_numero_segmento(uint32_t dirLogica) 
{
    uint32_t tam_max_segmento;
    tam_max_segmento = cpu_config_get_tam_max_segmento(cpuConfig);
    return (uint32_t) floor(dirLogica / tam_max_segmento);
}

uint32_t obtener_offset(uint32_t dirLogica) 
{
    uint32_t tam_max_segmento;
    tam_max_segmento = cpu_config_get_tam_max_segmento(cpuConfig);
    return (uint32_t) dirLogica % tam_max_segmento;
}

