#include <kernel-tabla-archivos.h>

// Crea una instancia de semaforo recurso para el diccionario de archivos
static t_semaforo_recurso *crear_semaforo_archivo()
{
    t_semaforo_recurso *semaforoRecurso = malloc(sizeof(*semaforoRecurso));
    
    semaforoRecurso->instancias = 1;
    semaforoRecurso->estadoRecurso = crear_estado(RECURSO);
    
    return semaforoRecurso;
}

// Manejo del diccionario de semaforos de los recursos
t_dictionary *crear_diccionario_semaforos_archivos()
{
    t_dictionary *diccionarioSemaforosRecursos = dictionary_create();
    return diccionarioSemaforosRecursos;
}

bool archivo_esta_abierto(char *nombreArchivo)
{
    return diccionario_semaforos_recursos_existe_recurso(tablaArchivosAbiertos, nombreArchivo);
}

void abrir_archivo_en_tabla_de_pcb(t_pcb *pcbEnEjecucion, char *nombreArchivo)
{
    t_dictionary *archivosAbiertosEnPcb = pcb_get_archivos_abiertos(pcbEnEjecucion);
    dictionary_put(archivosAbiertosEnPcb, nombreArchivo, 0);
    return;
}

void cerrar_archivo_en_tabla_de_pcb(t_pcb *pcbEnEjecucion, char *nombreArchivo)
{
    t_dictionary *archivosAbiertosEnPcb = pcb_get_archivos_abiertos(pcbEnEjecucion);
    // Acá no se si deberia liberar alguna memoria antes de secar la entrada de la tabla
    dictionary_remove(archivosAbiertosEnPcb, nombreArchivo);
    return;
}
    

