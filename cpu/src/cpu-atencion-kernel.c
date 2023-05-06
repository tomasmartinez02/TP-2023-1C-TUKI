#include <cpu-atencion-kernel.h>

// Funciones privadas

// Funciones publicas

void atender_peticiones_kernel(void)
{
    for (;;) {

        t_cpu_pcb *pcbAEjecutar = recibir_pcb_de_kernel();

        bool terminarEjecucion = false;

        while (!terminarEjecucion) {
            terminarEjecucion = cpu_ejecutar_siguiente_instruccion(pcbAEjecutar);
        }

        cpu_pcb_destroy(pcbAEjecutar);
    }

    return;
}