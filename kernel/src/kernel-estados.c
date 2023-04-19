#include <kernel-estados.h>

t_estado* crear_estado(t_nombre_estado nombre) // A medida que avancemos le vamos a agregar cosas
{
    t_estado* estado = malloc(sizeof(*estado));
    estado->listaProcesos = list_create();
    estado->nombreEstado = nombre;

    return self;
}

void destruir_estado(t_estado* estado) 
{
    if (list_is_empty(estado->listaProcesos)) {
        list_destroy(estado->listaProcesos);
    } 
    else {
        // list_destroy_and_destroy_elements(self->listaProcesos, (void*)destruir_pcb); // habria que hacer la funcion destruir_pcb!
    }

    free(estado->nombreEstado);
    free(estado);
}

void* encolar_en_new_a_nuevo_pcb_entrante(void *arg) 
{
    return ; // Para poder seguir con esto, necesitamos la estructura del PCB
}