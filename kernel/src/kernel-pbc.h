#ifndef KERNEL_PCB_H
#define KERNEL_PCB_H

//Standard Libraries
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

//Static-utils libraries
#include <serializacion/buffer.h>

t_pcb* pcb_create(uint32_t pid);
t_pcb* pcb_destroy(t_pcb pcb);

#endif 