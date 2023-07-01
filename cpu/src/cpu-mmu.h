#ifndef CPU_PCB_H_
#define CPU_PCB_H_

// Bibliotecas estandar
#include <stdint.h>
#include <math.h>
// Bibliotecas internas
#include <cpu-config.h>

// Prototipos

uint32_t obtener_offset(uint32_t dirLogica);
uint32_t obtener_numero_segmento(uint32_t dirLogica); 

#endif