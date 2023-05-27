#include <kernel-tabla-archivos.h>

// Crea una instancia de semaforo recurso para el diccionario de archivos
t_semaforo_recurso *crear_semaforo_archivo()
{
    t_semaforo_recurso *semaforoRecurso = malloc(sizeof(*semaforoRecurso));
    
    semaforoRecurso->instancias = 0;
    semaforoRecurso->estadoRecurso = crear_estado(RECURSO);
    
    return semaforoRecurso;
}

// Manejo del diccionario de semaforos de los recursos
t_dictionary *crear_diccionario_semaforos_archivos()
{
    t_dictionary *diccionarioSemaforosRecursos = dictionary_create();
    return diccionarioSemaforosRecursos;
}

bool semaforo_archivo_debe_desbloquear_archivo(t_semaforo_recurso *self)
{
    uint32_t instancias = semaforo_recurso_get_instancias(self);
    return (semaforo_recurso_hay_procesos_bloqueados(self) && instancias == 1);
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
    if (!dictionary_has_key(nombreArchivo))
    {
        // error
    }
    dictionary_remove(archivosAbiertosEnPcb, nombreArchivo);

    t_semaforo_recurso semaforoArchivo = dictionary_get(tablaArchivosAbiertos, nombreArchivo);
    semaforo_recurso_post(semaforoArchivo);
    return;
}

void actualizar_puntero_archivo_en_tabla_de_pcb(t_pcb *pcbEnEjecucion, char *nombreArchivo, uint32_t ubicacionNueva)
{
    t_dictionary *archivosAbiertosEnPcb = pcb_get_archivos_abiertos(pcbEnEjecucion);
    if (!dictionary_has_key(nombreArchivo))
    {
        // error
    }
    dictionary_remove(archivosAbiertosEnPcb, nombreArchivo);
    dictionary_put(archivosAbiertosEnPcb, nombreArchivo, ubicacionNueva);
    return;
}

void abrir_archivo_globalmente(char *nombreArchivo)
{
    t_semaforo_recurso semaforoArchivo = crear_semaforo_archivo();
    // El archivo se crea con 0 instancias porque cuando se crea se abre.
    dictionary_put(tablaArchivosAbiertos, nombreArchivo, semaforoArchivo);
    return;
}

void cerrar_archivo_globalmente(char *nombreArchivo)
{   
    // Acá no se si deberia liberar alguna memoria antes de sacar la entrada de la tabla
    dictionary_remove(tablaArchivosAbiertos, nombreArchivo);
    return;
}


    

