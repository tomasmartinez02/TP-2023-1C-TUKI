#ifndef MEMORIA_ATENCION_KERNEL_H_
#define MEMORIA_ATENCION_KERNEL_H_

// Bibliotecas estandar
// Bibliotecas static utils
#include <serializacion/stream.h>
#include <utils/common-utils.h>
#include <serializacion/buffer.h>
#include <utils/tablas-pcb.h>
// Bibliotecas commons
// Bibliotecas internas modulo memoria
#include <memoria-config.h>
#include <memoria-manejo-tablas.h>
#include <memoria-adapter-kernel.h>


// Prototipos

void *atender_peticiones_kernel();

#endif