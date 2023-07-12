#include <kernel-semaforos-recursos.h>

// Funciones privadas

// Crea una instancia de semaforo recurso para el diccionario de recursos
static t_semaforo_recurso *__crear_semaforo_recurso(int32_t instancias)
{
    t_semaforo_recurso *semaforoRecurso = malloc(sizeof(*semaforoRecurso));
    
    semaforoRecurso->instancias = instancias;
    semaforoRecurso->estadoRecurso = crear_estado(RECURSO);
    
    return semaforoRecurso;
}

// Destruye una instancia de semaforo de recurso
static void __destruir_semaforo_recurso(void *semaforoRecursoVoid)
{
    t_semaforo_recurso *semaforoRecurso = (t_semaforo_recurso *) semaforoRecursoVoid;

    destruir_estado(semaforoRecurso->estadoRecurso);
    free(semaforoRecurso);
}

// Funciones publicas

// Getters y setters semaforo recurso
int32_t semaforo_recurso_get_instancias(t_semaforo_recurso *self)
{
    return self->instancias;
}

t_estado *semaforo_recurso_get_estado_recurso(t_semaforo_recurso *self)
{
    return self->estadoRecurso;
}

void semaforo_recurso_wait(t_semaforo_recurso *self)
{
    self->instancias--;
}

void semaforo_recurso_post(t_semaforo_recurso *self)
{
    self->instancias++;
}

bool semaforo_recurso_puedo_utilizar_recurso(t_semaforo_recurso *self)
{
    int32_t instancias = semaforo_recurso_get_instancias(self);
    return instancias >= 0;
}

bool semaforo_recurso_debe_desbloquear_recurso(t_semaforo_recurso *self)
{
    int32_t instancias = semaforo_recurso_get_instancias(self);
    return (semaforo_recurso_hay_procesos_bloqueados(self) && instancias == 0);
}

bool semaforo_recurso_debe_bloquear_proceso(t_semaforo_recurso *self)
{
    return !semaforo_recurso_puedo_utilizar_recurso(self);
}

bool semaforo_recurso_hay_procesos_bloqueados(t_semaforo_recurso *self)
{
    t_estado *colaBloqueados = semaforo_recurso_get_estado_recurso(self);
    return estado_contiene_pcbs_atomic(colaBloqueados);
}

void semaforo_recurso_bloquear_proceso(t_semaforo_recurso *self, t_pcb *pcbBloqueado)
{
    t_estado *colaBloqueados = semaforo_recurso_get_estado_recurso(self);
    estado_encolar_pcb_atomic(colaBloqueados, pcbBloqueado);
    return;
}

t_pcb *semaforo_recurso_desbloquear_primer_proceso_bloqueado(t_semaforo_recurso *self)
{
    t_estado *colaBloqueados = semaforo_recurso_get_estado_recurso(self);
    t_pcb* pcb = estado_remover_pcb_de_cola_atomic(estadoBlocked, estado_desencolar_primer_pcb_atomic(colaBloqueados)); // Chequear esto
    return pcb;
}

// Manejo del diccionario de semaforos de los recursos

t_dictionary *crear_diccionario_semaforos_recursos(char **recursos, char **instanciasRecursos)
{
    t_dictionary *diccionarioSemaforosRecursos = dictionary_create();

    for (int i = 0; recursos[i] != NULL && instanciasRecursos[i] != NULL; i++) {
        int32_t instancias = (uint32_t) atoi(instanciasRecursos[i]);
        t_semaforo_recurso *semaforoRecurso = __crear_semaforo_recurso(instancias);

        char *nombreRecurso = recursos[i];
        dictionary_put(diccionarioSemaforosRecursos, nombreRecurso, (void *) semaforoRecurso);
    }

    return diccionarioSemaforosRecursos;
}

void destruir_diccionario_semaforos_recursos(t_dictionary *diccionarioSemaforosRecursos)
{
    dictionary_destroy_and_destroy_elements(diccionarioSemaforosRecursos, __destruir_semaforo_recurso);
    return;
}

bool diccionario_semaforos_recursos_existe_recurso(t_dictionary *diccionarioSemaforosRecursos, char *nombreRecurso)
{
    return dictionary_has_key(diccionarioSemaforosRecursos, nombreRecurso);
}

t_semaforo_recurso *diccionario_semaforos_recursos_get_semaforo_recurso(t_dictionary *diccionarioSemaforosRecursos, char *nombreRecurso)
{
    return (t_semaforo_recurso *) dictionary_get(diccionarioSemaforosRecursos, nombreRecurso);
}
