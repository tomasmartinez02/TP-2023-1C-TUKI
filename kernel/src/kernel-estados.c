#include <kernel-estados.h>

t_estado* crear_estado(t_nombre_estado nombre) // A medida que avancemos le vamos a agregar cosas
{
    t_estado* estado = malloc(sizeof(*estado));
    estado->listaProcesos = list_create();
    estado->nombreEstado = nombre;

    estado->mutexEstado = malloc(sizeof(*(estado->mutexEstado)));
    pthread_mutex_init(estado->mutexEstado, NULL);

    return estado;
}

void destruir_estado(t_estado* estado) 
{
    if (list_is_empty(estado->listaProcesos)) {
        list_destroy(estado->listaProcesos);
    } 
    else {
        list_destroy_and_destroy_elements(estado->listaProcesos, (void*)destruir_pcb); 
    }

    pthread_mutex_destroy(estado->mutexEstado);
    free(estado->mutexEstado);
    // free(estado->nombreEstado); nos da error, creemos que no hay que hacerlo por ser un enum
    free(estado);
}

void inicializar_estructuras_estados(void)
{
    estadoNew = crear_estado(NEW);
    estadoReady = crear_estado(READY);
    estadoExecute = crear_estado(EXEC);
    estadoExit = crear_estado(EXIT);
    estadoBlocked = crear_estado(BLOCKED);
}

void estado_encolar_pcb(t_estado* estadoDestino, t_pcb* pcbAAgregar)
{
    pthread_mutex_lock(estado_get_mutex(estadoDestino));
    list_add(estado_get_list(estadoDestino), pcbAAgregar);
    pthread_mutex_unlock(estado_get_mutex(estadoDestino));
}

t_list* estado_get_list(t_estado* estado) 
{
    return estado->listaProcesos;
}

pthread_mutex_t* estado_get_mutex(t_estado* estado) 
{
    return estado->mutexEstado;
}
