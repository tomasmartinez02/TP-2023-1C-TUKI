#include <kernel-estados.h>

// Funciones privadas

// Encola un pcb en el estado correspondiente
static void __estado_encolar_pcb(t_estado *estadoDestino, t_pcb *pcbAEncolar)
{
    list_add(estado_get_list(estadoDestino), pcbAEncolar);

    if (estado_get_nombre_estado(estadoDestino) == READY) {
        log_ingreso_cola_ready(estadoDestino);
        pcb_set_tiempo_llegada_ready(pcbAEncolar);
    }
}

// Desencola el primer pcb de un estado
static t_pcb *__estado_desencolar_primer_pcb(t_estado *self) 
{
    return list_remove(estado_get_list(self), 0);
}

// Remueve un pcb de un estado
static t_pcb *__estado_remover_pcb_de_cola(t_estado *self, t_pcb *pcbADesencolar) 
{
    uint32_t index = list_get_index(estado_get_list(self), pcb_es_este_pcb_por_pid, pcbADesencolar);
    
    t_pcb *pcb = NULL;
    if (index != -1) {
        pcb = (t_pcb *) list_remove(estado_get_list(self), index);
    }

    return pcb;
}

// Retorna si un pcb se encuentra o no en el estado

static bool __estado_contiene_pcb(t_estado *self, t_pcb *pcbBuscado)
{
    bool contienePcb = false;
    
    uint32_t index = list_get_index(estado_get_list(self), pcb_es_este_pcb_por_pid, pcbBuscado);
    if (index != -1) {

        contienePcb = true;
    }

    return contienePcb;
}

// Obtiene el pcb con el maximo hrrn
t_pcb *__estado_obtener_pcb_segun_maximo_hrrn(t_estado *estado)
{
    t_pcb *pcbSeleccionado = (t_pcb *) list_get_maximum(estado_get_list(estado), comparar_pcb_segun_hrrn);

    return pcbSeleccionado;
}

// Obtiene el pcb con el maximo hrrn de forma atomica
static t_pcb *__estado_obtener_pcb_segun_maximo_hrrn_atomic(t_estado * estado)
{
    sem_wait(estado_get_semaforo(estado));
    pthread_mutex_lock(estado_get_mutex(estado));
    t_pcb *pcbSeleccionado = __estado_obtener_pcb_segun_maximo_hrrn(estado);
    pthread_mutex_unlock(estado_get_mutex(estado));
    sem_post(estado_get_semaforo(estado));

    return pcbSeleccionado;
}

// Retorna si el estado contiene pcbs en su lista
static bool __estado_contiene_pcbs(t_estado *estado)
{
    t_list *listaEstado = estado_get_list(estado);

    return !list_is_empty(listaEstado);
}

// Funciones publicas

t_estado *crear_estado(t_nombre_estado nombreEstado) // A medida que avancemos le vamos a agregar cosas
{
    t_estado *estado = malloc(sizeof(*estado));
    
    // Creo y seteo el nombre estado y la lista para guardar procesos
    estado->nombreEstado = nombreEstado;
    estado->listaProcesos = list_create();

    // Creo y seteo el semaforo estado
    sem_t *semaforoEstado = malloc(sizeof(*semaforoEstado));
    sem_init(semaforoEstado, 0, 0);
    estado->semaforoEstado = semaforoEstado;

    // Creo y seteo el mutex del estado
    pthread_mutex_t *mutexEstado = malloc(sizeof(*(mutexEstado)));
    pthread_mutex_init(mutexEstado, NULL);
    estado->mutexEstado = mutexEstado;

    return estado;
}

void destruir_estado(t_estado *self) 
{
    if (list_is_empty(self->listaProcesos)) {
        list_destroy(self->listaProcesos);
    } 
    else {
        list_destroy_and_destroy_elements(self->listaProcesos, (void*) destruir_pcb); 
    }

    sem_destroy(self->semaforoEstado);
    free(self->semaforoEstado);
    
    pthread_mutex_destroy(self->mutexEstado);
    free(self->mutexEstado);
    
    free(self);
}

void estado_encolar_pcb_atomic(t_estado *estadoDestino, t_pcb *pcbAEncolar)
{
    pthread_mutex_lock(estado_get_mutex(estadoDestino));
    __estado_encolar_pcb(estadoDestino, pcbAEncolar);
    pthread_mutex_unlock(estado_get_mutex(estadoDestino));
    sem_post(estado_get_semaforo(estadoDestino));
}

t_pcb *estado_desencolar_primer_pcb_atomic(t_estado *self) 
{
    sem_wait(estado_get_semaforo(self));
    pthread_mutex_lock(estado_get_mutex(self));
    t_pcb *pcb = __estado_desencolar_primer_pcb(self);
    pthread_mutex_unlock(estado_get_mutex(self));
    
    return pcb;
}

t_pcb *estado_remover_pcb_de_cola_atomic(t_estado *self, t_pcb *pcbADesencolar) 
{
    sem_wait(estado_get_semaforo(self));
    pthread_mutex_lock(estado_get_mutex(self));
    t_pcb *pcb = __estado_remover_pcb_de_cola(self, pcbADesencolar);
    pthread_mutex_unlock(estado_get_mutex(self));
    
    return pcb;
}

bool estado_contiene_pcb_atomic(t_estado *self, t_pcb *pcbBuscado) 
{
    pthread_mutex_lock(estado_get_mutex(self));
    bool contienePcb = __estado_contiene_pcb(self, pcbBuscado);
    pthread_mutex_unlock(estado_get_mutex(self));
    
    return contienePcb;
}

t_pcb *estado_remover_pcb_segun_maximo_hrrn_atomic(t_estado *estado)
{
    t_pcb *pcbObtenido = __estado_obtener_pcb_segun_maximo_hrrn_atomic(estado);
    t_pcb *pcbMaximoHrrn = estado_remover_pcb_de_cola_atomic(estado, pcbObtenido);

    return pcbMaximoHrrn;
}

bool estado_contiene_pcbs_atomic(t_estado *estado)
{
    pthread_mutex_lock(estado_get_mutex(estado));
    bool contienePcbs = __estado_contiene_pcbs(estado);
    pthread_mutex_unlock(estado_get_mutex(estado));

    return contienePcbs;
}

t_nombre_estado estado_get_nombre_estado(t_estado *self)
{
    return self->nombreEstado;
}

t_list *estado_get_list(t_estado *self) 
{
    return self->listaProcesos;
}

sem_t *estado_get_semaforo(t_estado *self)
{
    return self->semaforoEstado;
}

pthread_mutex_t *estado_get_mutex(t_estado *self) 
{
    return self->mutexEstado;
}
