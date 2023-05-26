#include <kernel-adapter-filesystem.h>
void __destruir_archivo_abierto(void *archivo)
{
    t_archivo *archivoAbierto = (t_archivo *) archivo;
    destruir_archivo(archivoAbierto);
}

 bool es_este_archivo_por_nombre(void *archivo) 
{
        t_archivo *archivoAbierto = (t_archivo*) archivo;
        return string_equals_ignore_case(nombre, archivoAbierto->nombre);
}

void agregar_archivo_abierto_a_tabla(t_list *archivosAbiertos, t_archivo *archivoAbierto)
{
    list_add(archivosAbiertos, archivoAbierto);
    return;
}

t_archivo *buscar_archivo_por_nombre(t_list *archivosAbiertos, char *nombreArchivo)
{
    return (t_archivo *) list_find(archivosAbiertos, es_este_archivo_por_nombre(nombreArchivo));
}

bool archivo_esta_abierto(t_archivo *archivo)
{   
    char* nombreArchivo = archivo_get_nombre(archivo);
    if (buscar_archivo_por_nombre(tablaArchivosAbiertos, nombreArchivo) != NULL)
    {
        return true;
    }
    return false;
}

void destruir_lista_archivos(t_list *archivosAbiertos)
{
    list_destroy_and_destroy_elements(archivosAbiertos, __destruir_archivo_abierto);
    return;
}